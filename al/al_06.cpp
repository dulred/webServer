#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 面试经典150题 -> 189. 轮转数组
 * @details 通用思路，中间人思路 不在原来的区域取进行，另外创建一个空间取按照输出想要的信息去填充，然后返回
 * 
 */

// 
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        k = k % nums.size();
        vector<int> temp(nums.size());
        int j = nums.size() - k;
        for (size_t i = 0; i < nums.size(); i++)
        {
            if (j == nums.size()){
                j=0;
            }
            temp[i] = nums[j++];
        }
        nums = temp;
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {1,2,3,4,5,6,7};
    s.rotate(nums,3);  //expect：k=3 [5,6,7,1,2,3,4] 
    for (auto &i : nums)
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
