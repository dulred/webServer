#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief 面试经典150题 -> 26.删除有序数组中的重复项
 * @details 默认是单指针，扩展成多指针  或者  中间人思路
 * 
 */
//中间人思路
// class Solution {
// public:
//     int removeDuplicates(vector<int>& nums) {
//         vector<int> temp(nums.size());
//         int count = 0;
//         int j = 0;
//         for (size_t i = 0; i < nums.size(); ++i)
//         {
//             if ( i>=1 && nums[i] == nums[i-1])
//             {
//                 continue;
//             }
//             temp[j] = nums[i];
//             ++j;
//             ++count;
            
//         }
//         nums = temp;  // 注意：返回值是count，所以返回后需要将nums数组修改
//         return count;
//     }
// };

//双指针思路
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size()==0)
        {
            return;
        }
        
        int count = 1;
        int j = 1;
        for (size_t i = 1; i < nums.size(); i++)
        {
            if (nums[j-1] != nums[i])
            {
                nums[j] = nums[i];
                ++j;
            }
            
            count = j;
        }
        
        return count;
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {0,0,1,1,1,2,2,3,3,4};
    int n =  s.removeDuplicates(nums);  //expect: 5, 原数组前五个元素为，nums = [0,1,2,3,4] ，后面无所谓
    std::cout << "size: " << n << std::endl;
    for (auto &i : nums)
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
