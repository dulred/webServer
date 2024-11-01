#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
// g++ simple_tcp_server.cpp -o simple_tcp_server -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86  -lws2_32
void start_server() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        
        std::string message = "Hello from server!\n";
        boost::asio::write(socket, boost::asio::buffer(message));

        // 读取请求数据
        // boost::asio::streambuf request;
        // boost::asio::read_until(socket, request, "\r\n\r\n");

        // // 打印接收到的请求数据
        // std::istream request_stream(&request);
        // std::string request_line;
        // while (std::getline(request_stream, request_line)) {
        //     std::cout << request_line << std::endl;
        // }
        // std::cout << "--------------------------------" << std::endl;

        // // 你也可以打印原始的二进制数据
        // std::cout << "Raw request data: ";
        // std::cout.write(boost::asio::buffer_cast<const char*>(request.data()), request.size());
        // std::cout << std::endl;

    }
}

int main() {
    try {
        start_server();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
