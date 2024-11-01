#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 面试经典150题 -> 122. 买卖股票的最佳时机 II
 * @details 积累总结 不同类型的通用解法 以及探索的思路路径，
 * 
 */

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int total = 0;
        for (size_t i = 1; i < prices.size(); i++)
        {
            if (prices[i] - prices[i-1]> 0)
            {
                total += prices[i] - prices[i-1];
            }
            
        }
        return total;
        
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {7,1,5,3,6,4};
    int max = s.maxProfit(nums);  //expect： 7
    std::cout << "maxProfit: " << max << std::endl;
    for (auto &i : nums)    
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
