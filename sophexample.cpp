/*
 * given an array of integers, return a new array such that
 * each number at index i of the new array is the product
 * of all the numbers in original array except the one at i
 *
*/
#include <iostream>
#include <vector>
#include "zolomon.hpp"

std::vector<int> prodVectorNoDivision(std::vector<int> &nums)
{
    std::vector<long int> prefix(nums.size());
    std::vector<long int> suffix(nums.size());
    std::vector<int> result(nums.size());

    prefix[0] = 1;
    suffix.back() = 1;

    for (int i = 1; i < nums.size(); i++) {
        prefix[i] = nums[i - 1] * prefix[i - 1];
    }
    for (int i = nums.size() - 2; i >= 0; i--) {
        suffix[i] = nums[i + 1] * suffix[i + 1];
    }

    for (int i = 0; i < result.size(); i++) {
        result[i] = prefix[i] * suffix[i];
    }

    return result;
}

int main()
{
    std::vector<int> nums1 = {1, 2, 2, 3, 1};
    std::vector<int> nums2 = {0, 1, 2, 1, 2};

    zoln::TestCollection Test1("Sample Test", {
        zoln::TestCase("Basic Test", prodVectorNoDivision(nums1), std::vector<int>({12, 6, 6, 4, 12}), true),
        zoln::TestCase("Handling Zeroes", prodVectorNoDivision(nums2), std::vector<int>({4, 0, 0, 0, 0}), true)
    });

    Test1.run_collection("-p -d");

    return 0;
}