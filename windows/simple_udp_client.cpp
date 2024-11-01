#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

void start_udp_client() {
    boost::asio::io_context io_context;
    udp::socket socket(io_context);
    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "127.0.0.1", "8080").begin();

    std::string message = "Hello from UDP client!\n";
    socket.open(udp::v4());
    socket.send_to(boost::asio::buffer(message), receiver_endpoint);

    char reply[1024];
    udp::endpoint sender_endpoint;
    size_t length = socket.receive_from(boost::asio::buffer(reply), sender_endpoint);
    
    std::cout << "Response: " << std::string(reply, length) << std::endl;
}

int main() {
    try {
        start_udp_client();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
