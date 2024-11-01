#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 面试经典150题 -> 169. 多数元素
 * @details 目标的转化，看似最简单的方法就是统计每个数字出现的字数，次数最多大于2/n就是，但是可以用数学代数特征来快速解决，将数据排序后，取中位数即可
 * 
 */

class Solution {
public:
    int majorityElement(vector<int>& nums) {

        // 默认升序排序
        std::sort(nums.begin(), nums.end());
        return nums[nums.size()/2];

    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {2,2,1,1,1,2,2};
    int n =  s.majorityElement(nums);  //expect: 2
    std::cout << "size: " << n  << std::endl;
    for (auto &i : nums)
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
