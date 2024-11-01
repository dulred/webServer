#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

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
