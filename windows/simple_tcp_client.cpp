#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// g++ simple_tcp_client.cpp -o simple_tcp_client -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86  -lws2_32
void start_client() {
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "8080");
    tcp::socket socket(io_context);
    
    boost::asio::connect(socket, endpoints);

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, '\n');

    std::cout << "Response: " << &response << std::endl;
}

int main() {
    try {
        start_client();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
