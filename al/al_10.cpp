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
    int jump(vector<int>& nums) {
        int step = 0;
        if (nums.size() == 1)
        {
            return 0;
        }
        if (nums[0] >= nums.size()-1)
        {
            return 1;
        }
        step = 1;
        for (int i = 0; i < nums.size() - 1;)
        {
            int max_step = nums[i+1] + i+1;
            i = i+1;
            int temp = i;
            for (int j = temp; j < nums[temp-1]+temp; j++)
            {
                if ((nums[j] + j)>= nums.size()-1)
                {
                    return ++step;
                }
                
                if ((nums[j] + j ) > max_step)
                {
                    max_step = (nums[j] + j);
                    i = j;      
                }  
            }
            ++step;
        }

        return step;
        
    }
};

int main (int argc ,char**argv){    
    Solution s;
    vector<int> nums = {3,1,1,1,1}; 
    int max = s.jump(nums);  //expect：  nums = [2,3,1,1,4]  2
    std::cout << "maxProfit: " << max << std::endl;
    for (auto &i : nums)    
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
