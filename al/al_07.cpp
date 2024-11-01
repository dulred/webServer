#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 面试经典150题 -> 121. 买卖股票的最佳时机
 * @details 数学特征题,画图容易理解，最低点买入，最高点卖出，找到一类题的解决思路，
 * 
 */

// 逐个对比找出最大值，但是时间复杂度过高 o(n2)
// class Solution {
// public:
//    int maxProfit(vector<int>& prices) {
//         int max = 0;
//         for (size_t i = 1; i < prices.size(); i++)
//         {
//             for (size_t j = 0; j < i; j++)
//             {
//                 if (prices[i] - prices[j] > max)
//                 {
//                     max = prices[i] - prices[j];
//                 }
//             }
            
//         }
//         return max;
        
//     }
// };

// 减少次要的价格计算次数，有些一看就不用计算，找最小的算，就行
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int inf = 1e9;
        int minprice = inf, maxprofit = 0;
        for (int price: prices) {
            maxprofit = max(maxprofit, price - minprice);
            minprice = min(price, minprice);
        }
        return maxprofit;
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {7,1,5,3,6,4};
    int max = s.maxProfit(nums);  //expect：5 最大利润 = 6-1 = 5
    std::cout << "maxProfit: " << max << std::endl;
    for (auto &i : nums)    
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
