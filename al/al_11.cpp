#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 面试经典150题 -> 45. 跳跃游戏 II
 * @details 本质是寻找计算题。循环的i ，状态一定是不要变，要不然变的话，你排查也很乱
 * 
 */

// 重新定义什么是最大，找到最大，就能包含所有情况
class Solution {
public:
    int hIndex(vector<int>& citations) {
        
    }
};

int main (int argc ,char**argv){    
    Solution s;
    vector<int> nums = {3,0,6,1,5}; 
    int max = s.hIndex(nums);  //expect：  nums = [2,3,1,1,4]  2
    std::cout << "maxProfit: " << max << std::endl;
    for (auto &i : nums)    
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
