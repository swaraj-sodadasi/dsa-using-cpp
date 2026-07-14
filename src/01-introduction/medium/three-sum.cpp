#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (FIXED TARGET + TWO-POINTER CONVERGENCE WITH DUPLICATE AVOIDANCE):
- To find all unique triplets that sum to zero, we first sort the array. This allows us to use a two-pointer approach and easily skip duplicate elements.
- We iterate through the array using a fixed pointer 'i' from index 0 up to 'size - 3'. 'nums[i]' serves as the anchor value.
- If 'nums[i] > 0', we can break early because the array is sorted, and any subsequent elements will also be greater than 0, making a zero sum impossible.
- To avoid duplicate triplets, if 'i > 0' and 'nums[i] == nums[i - 1]', we skip the current iteration.
- For each fixed 'i', we initialize a 'left' pointer at 'i + 1' and a 'right' pointer at 'size - 1'.
- We calculate the current sum: `sum = nums[i] + nums[left] + nums[right]`.
  - If `sum == 0`, we found a valid triplet. We save it, and then advance both 'left' and 'right' pointers past any identical elements to avoid duplicates.
  - If `sum < 0`, we need a larger value, so we increment 'left'.
  - If `sum > 0`, we need a smaller value, so we decrement 'right'.

TIME COMPLEXITY: O(N^2) - Sorting takes O(N log N). The nested loops execute an O(N) two-pointer traversal for each of the N elements.
SPACE COMPLEXITY: O(1) or O(N) - Depends on the implementation of the sorting algorithm. No extra auxiliary heap space is allocated beyond the required output storage.
*/
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> result;
    if (nums.size() < 3) return result;

    sort(nums.begin(), nums.end());

    for (size_t i = 0; i < nums.size() - 2; ++i) {
        if (nums[i] > 0) break;
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        size_t left = i + 1;
        size_t right = nums.size() - 1;

        while (left < right) {
            int currentSum = nums[i] + nums[left] + nums[right];

            if (currentSum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});
                
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                
                left++;
                right--;
            } else if (currentSum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    return result;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    vector<vector<int>> result = threeSum(nums);

    cout << "\nUnique triplets that sum to zero:\n";
    if (result.empty()) {
        cout << "No matching triplets found." << endl;
    } else {
        for (const auto& triplet : result) {
            cout << "[" << triplet[0] << ", " << triplet[1] << ", " << triplet[2] << "]\n";
        }
    }

    return 0;
}