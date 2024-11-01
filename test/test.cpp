#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

int main()
{
    // 获取开始时间
    auto start = std::chrono::high_resolution_clock::now();

    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 获取结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算时间差
    auto duration = end - start;

    // 打印时间差，以毫秒和微秒为单位
    std::cout << "Duration: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " milliseconds, "
              << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << " microseconds" << std::endl;


    // std::time_t now = time(0);
    // std::ctime(&now);
    
    

    return 0;
}
