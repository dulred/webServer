#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

// g++ -std=c++11 -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86 udpClient.cpp -o udpClient -lws2_32
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
      *resolver.resolve(udp::v4(), argv[1], "daytime").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::array<char, 1> send_buf  = {{ 0 }};
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    std::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint);

    std::cout.write(recv_buf.data(), len);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}