#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
using boost::asio::ip::tcp;

// g++ -std=c++11 -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86 testServer2.cpp -o server2 -lws2_32
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    for (;;)
    {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

      // Read data from the client
      std::array<char, 128> buf;
      boost::system::error_code error;
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error)
      {
          std::cerr << "Read error: " << error.message() << std::endl;
          continue;
      }

      // Extract client message
      std::string client_message(buf.data(), len);

      // Create the response message
      std::string response_message = "Received: " + client_message;

      std::this_thread::sleep_for(std::chrono::seconds(2));

      // Send the response back to the client
      boost::asio::write(socket, boost::asio::buffer(response_message), error);

    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}