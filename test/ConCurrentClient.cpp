#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <mutex>
using boost::asio::ip::tcp;

// Global mutex for thread-safe console output
std::mutex output_mutex;
void run_client(const std::string& host, int id)
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(host, "daytime");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Send the client ID to the server
        std::ostringstream oss;
        oss << "ClientID:" << id;
        std::string client_id = oss.str();

        boost::asio::write(socket, boost::asio::buffer(client_id + "\n"));

        std::array<char, 128> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof)
        {
            std::lock_guard<std::mutex> guard(output_mutex);
            std::cout << "Client " << id << ": Connection closed by server." << std::endl;
        }
        else if (error)
        {
            throw boost::system::system_error(error); // Handle any other error
        }

        std::time_t now = time(0);
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "Client " << id << " received: " << std::string(buf.data(), len) << "time :" << std::ctime(&now)<< std::endl;
    }
    catch (std::exception& e)
    {
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cerr << "Client " << id << " error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <number_of_clients>" << std::endl;
        return 1;
    }

    std::string host = argv[1];
    int num_clients = std::stoi(argv[2]);

    std::vector<std::thread> threads;

    // 创建多个线程，每个线程都运行一个客户端
    for (int i = 0; i < num_clients; ++i)
    {
        threads.emplace_back(run_client, host, i + 1);
    }

    // 等待所有线程完成
    for (auto& t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return 0;
}
