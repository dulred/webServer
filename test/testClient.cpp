#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <thread> // For std::this_thread::sleep_for
using boost::asio::ip::tcp;

// g++ -std=c++11 -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86 testServer2.cpp -o server2  
// client.exe 127.0.0.1   两个参数
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

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;)
    {
      std::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
      std::cout << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
