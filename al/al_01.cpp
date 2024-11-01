#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief 面试经典150题 -> 88.合并两个有序数组
 * @details 易错点在于 1.数组越界问题  2.边界问题  
 */
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        vector<int> temp(m+n);
        int j = 0;
        int k = 0;
        for (size_t i = 0; i < nums1.size(); i++)
        {
            if(m==0 && n!=0){
                nums1 = nums2;
                return;
            }
            if (n==0)
            {
                return;
            }
            
            if ( j >=n || (k < m && nums1[k] <= nums2[j])){
            temp[i] = nums1[k];
            ++k;
            }else{
            temp[i] = nums2[j];
            ++j;
            }
        }
        nums1 = temp;
    }
};


int main (int argc ,char**argv){
    Solution s;
    vector<int> nums1 = {2,0};
    vector<int> nums2 = {1};
    s.merge(nums1,1,nums2,nums2.size());
    for (auto &i : nums1)
    {
        std::cout << i << std::endl;
    }
    
    return 0;
}
