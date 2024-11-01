#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 面试经典150题 -> 122. 买卖股票的最佳时机 II
 * @details 本质要探索数学规律特征（信息点），然后转化目标,数学特征题,拍平数据，然后判断有无0(除了最后一位外)，如果没有就true，有的话就false  总体特征->局部特征的思考和探索的路径,不一定要完美.
 * 
 */

// 找到每个数字对应的最大值，既不漏每个的信息，又能达成目的
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if (nums.size() == 1) return true;
        int max_reach = 0;
        for (int i = 0; i < nums.size() - 1; i++) {
            max_reach = max(max_reach, i + nums[i]);
            if (i == max_reach) return false;
        }
        return true;  // 最后一位一定可以到达
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {3,2,1,0,4};
    bool max = s.canJump(nums);  //expect： true   nums = [3,2,1,0,4]  false
    std::cout << "maxProfit: " << max << std::endl;
    for (auto &i : nums)    
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
