#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <nlohmann/json.hpp>  // Include nlohmann/json header

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using json = nlohmann::json;  // Use nlohmann JSON namespace

// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();

    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string
path_cat(
    beast::string_view base,
    beast::string_view path)
{
    if(base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

// Return a JSON response
json
create_json_response(int code, const std::string& msg, const json& data = json{})
{
    json response;
    response["code"] = code;
    response["msg"] = msg;
    response["data"] = data;
    return response;
}

// Return a response for the given request.
template <class Body, class Allocator>
http::message_generator
handle_request(
    beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>>&& req)
{
    // Handle methods
    if (req.method() != http::verb::get && req.method() != http::verb::head)
        return http::response<http::string_body>{
            http::status::bad_request, req.version(),
            create_json_response(400, "Unknown HTTP-method").dump()
        };

    // Request path must be absolute and not contain "..".
    if (req.target().empty() || req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return http::response<http::string_body>{
            http::status::bad_request, req.version(),
            create_json_response(400, "Illegal request-target").dump()
        };

    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if (req.target().back() == '/')
        path.append("index.html");

    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // Handle errors
    if (ec == beast::errc::no_such_file_or_directory)
        return http::response<http::string_body>{
            http::status::not_found, req.version(),
            create_json_response(404, "The resource was not found").dump()
        };

    if (ec)
        return http::response<http::string_body>{
            http::status::internal_server_error, req.version(),
            create_json_response(500, "An error occurred").dump()
        };

    auto const size = body.size();

    // Respond to HEAD request
    if (req.method() == http::verb::head)
    {
        http::response<http::empty_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return res;
    }

    // Respond to GET request
    http::response<http::file_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version())
    };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, mime_type(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return res;
}

//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_;
    http::request<http::string_body> req_;

public:
    // Take ownership of the stream
    session(
        tcp::socket&& socket,
        std::shared_ptr<std::string const> const& doc_root)
        : stream_(std::move(socket))
        , doc_root_(doc_root)
    {
    }

    // Start the asynchronous operation
    void
    run()
    {
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                          &session::do_read,
                          shared_from_this()));
    }

    void
    do_read()
    {
        req_ = {};
        stream_.expires_after(std::chrono::seconds(30));
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec == http::error::end_of_stream)
            return do_close();

        if (ec)
            return fail(ec, "read");

        send_response(
            handle_request(*doc_root_, std::move(req_)));
    }

    void
    send_response(http::message_generator&& msg)
    {
        bool keep_alive = msg.keep_alive();
        beast::async_write(
            stream_,
            std::move(msg),
            beast::bind_front_handler(
                &session::on_write, shared_from_this(), keep_alive));
    }

    void
    on_write(
        bool keep_alive,
        beast::error_code

 ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "write");

        if (!keep_alive)
            return do_close();

        do_read();
    }

    void
    do_close()
    {
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;

public:
    listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root)
        : ioc_(ioc)
        , acceptor_(ioc)
        , doc_root_(doc_root)
    {
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        if (ec)
        {
            fail(ec, "open");
            return;
        }

        acceptor_.set_option(net::socket_base::reuse_address(true));
        acceptor_.bind(endpoint, ec);
        if (ec)
        {
            fail(ec, "bind");
            return;
        }

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    void
    run()
    {
        do_accept();
    }

private:
    void
    do_accept()
    {
        acceptor_.async_accept(
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            fail(ec, "accept");
        }
        else
        {
            std::make_shared<session>(std::move(socket), doc_root_)->run();
        }

        do_accept();
    }
};

//------------------------------------------------------------------------------

// Main function
int
main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr <<
            "Usage: http-server-sync <address> <port>\n" <<
            "Example:\n" <<
            "    http-server-sync 0.0.0.0 8080\n";
        return EXIT_FAILURE;
    }

    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));

    net::io_context ioc{1};

    std::shared_ptr<std::string const> doc_root =
        std::make_shared<std::string>(".");

    std::make_shared<listener>(ioc, tcp::endpoint{address, port}, doc_root)->run();

    ioc.run();

    return EXIT_SUCCESS;
}