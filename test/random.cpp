#include <iostream>
#include <random>

int main() {
    // 创建随机数生成器，并使用当前时间作为种子
    std::random_device rd;  // 获取真实随机数种子
    std::mt19937 gen(rd()); // 以真实种子初始化 Mersenne Twister 随机数生成器
    std::uniform_int_distribution<> dis(0, 100); // 定义范围 [0, 32767]

      // 输出 5 个随机数
    for (int i = 0; i < 5; ++i) {
        std::cout << dis(gen) << std::endl;
    }

    return 0;
}