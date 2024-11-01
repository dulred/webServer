#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief 面试经典150题 -> 27.移除元素
 * @details 通用解法是 中间人思路 ，看清楚是否遍历的索引  跟 插入的索引  共用一个i  不然就另外定义一个j k 之类的
 * 
 */
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        vector<int> temp(nums.size());
        int count = 0;
        int j = 0;
        for (size_t i = 0; i < nums.size(); i++)
        {
            if (nums[i]!=val)
            {
                temp[j] = nums[i];
                ++j;
                ++count;
            }
        }  
        nums = temp;
        return count;
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {3,2,2,3};
    int n =  s.removeElement(nums,3);
    std::cout << "size: " << n << std::endl;
    for (auto &i : nums)
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
