#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief 面试经典150题 -> 80. 删除有序数组中的重复项 II
 * @details 默认是单指针，扩展成多指针  或者  中间人思路
 * 易错点: 如果是原地修改思路，防止前面修改后的数据对后续流程都产生影响， 如果有影响这样必然错误
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
//             if ( i>=2 && nums[i] == nums[i-2])
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

// 双指针思路
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size() == 0 || nums.size() == 1){
            return nums.size();  // 特判：如果数组为空或者只有一个元素，返回数组本身的长度
        }
        int count = 2;
        int j = 2;
        for (size_t i = 2; i < nums.size(); i++)
        {
            if (nums[i] != nums[j-2])
            {
                nums[j++] = nums[i];
                continue;
            }
        }
        count = j;
        return count;
    }
};
int main (int argc ,char**argv){
    Solution s;
    vector<int> nums = {1,1,1,2,2,3};
    int n =  s.removeDuplicates(nums);  //expect: 7, 前7个元素nums = [0,0,1,1,2,3,3]
    std::cout << "size: " << n << std::endl;
    for (auto &i : nums)
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
