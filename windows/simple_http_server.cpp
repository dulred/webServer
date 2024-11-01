
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp> // 用于解析 JSON
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>



namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;   // from <boost/asio/ip/tcp.hpp

// g++ simple_http_server.cpp -o simple_http_server -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86 -ID:\dulred\dev\3rdlibs\done\json-3.11.3\include -lws2_32 
// curl -X POST http://localhost:8080/submit -H "Content-Type: application/json" -d "{\"name\":\"Alice\",\"age\":30}"
// curl http://localhost:8080/params/someValue
// curl http://localhost:8080/
// res.body() = R"({"cecoS":{"addr":"附1号","area":"532125","cecoS":"5321xxx04021729","edus":["小学教育"],"name":"李xx","nati":"汉族","ofaddr":"云南省xxx县","qqAccount":"237xx5712","sex":"女","telnum":["138xxxxx103"],"vehicleinfo":[{"_id":"dff2e75385734f8f35d06f09ecf2dfeb","veco":"云ASxx2","vhcl":"C","vsbr":"大众x"},{"_id":"8284080b23cf90ea95ffd26fcd1778e2","veco":"云Sxxx","vhcl":"C","vsbr":"xx车有限公司"}],"weiXinAccount":"10xxxx0"}})";


std::map<std::string, std::string> parse_query(const std::string& query) {
    std::map<std::string, std::string> params;
    std::vector<std::string> pairs;
    boost::algorithm::split(pairs, query, boost::algorithm::is_any_of("&"));
    
    for (const auto& pair : pairs) {
        auto pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            params[key] = value;
        }
    }
    return params;
}

void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    if (req.method() == http::verb::get) {
        std::string target = std::string(req.target().data(), req.target().size());
        std::string query;

        // 检查是否有查询字符串
        auto pos = target.find('?');
        if (pos != std::string::npos) {
            query = target.substr(pos + 1);
            target = target.substr(0, pos);
        }

        // 解析查询参数
        auto params = parse_query(query);

        // 处理请求
        if (target == "/") {
            res.result(http::status::ok);
            res.set(http::field::server, "Boost.Beast HTTP Server");
            res.set(http::field::content_type, "application/json; charset=UTF-8");

            // 示例：可以根据参数构建响应
            std::string response_body = R"({"message":"Hello,)";
            if (params.count("cecoS")) {
                response_body += " " + params["cecoS"];
            }
            response_body += R"(!"})";
            std::cout << response_body<<std::endl;
            res.body() = R"({"cecoS":{"addr":"附1号","area":"532125","cecoS":"5321xxx04021729","edus":["小学教育"],"name":"李xx","nati":"汉族","ofaddr":"云南省xxx县","qqAccount":"237xx5712","sex":"女","telnum":["138xxxxx103"],"vehicleinfo":[{"_id":"dff2e75385734f8f35d06f09ecf2dfeb","veco":"云ASxx2","vhcl":"C","vsbr":"大众x"},{"_id":"8284080b23cf90ea95ffd26fcd1778e2","veco":"云Sxxx","vhcl":"C","vsbr":"xx车有限公司"}],"weiXinAccount":"10xxxx0"}})";
        }
    } else {
        res.result(http::status::bad_request);
        res.body() = "Invalid Request";
    }
}

int main() {
    try {
        net::io_context ioc;
        tcp::acceptor acceptor{ioc, tcp::endpoint{tcp::v4(), 8080}};
        std::cout << "Server is running on http://localhost:8080" << std::endl;

        for (;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            http::response<http::string_body> res;
            handle_request(req, res);

            http::write(socket, res);
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
