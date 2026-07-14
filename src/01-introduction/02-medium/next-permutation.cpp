#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (LEXICOGRAPHICAL SEQUENCE GENERATION):
- The algorithm processes the sequence from right to left to find the first element that breaks the decreasing order.
- Step 1: Find the first pair of adjacent elements `nums[i]` and `nums[i+1]` from the right such that `nums[i] < nums[i+1]`. The index `i` is the breakdown point (pivot).
- Step 2: If no such linear index is found, the entire array is sorted in descending order (largest possible permutation). We simply reverse the whole array to get the smallest possible permutation.
- Step 3: If a valid pivot index `i` is found, find the element `nums[j]` to the right of `i` that is strictly greater than `nums[i]` but as close to it as possible (the smallest element greater than `nums[i]` in the suffix).
- Step 4: Swap `nums[i]` and `nums[j]`.
- Step 5: Reverse the sequence following the index `i` to restore it to the lowest possible lexicographical order for that suffix.

TIME COMPLEXITY: O(N) - In the worst case, we perform a constant number of linear scans over an array of size N.
SPACE COMPLEXITY: O(1) - The permutation mutation is performed completely in-place, relying only on pointer variables.
*/
void nextPermutation(vector<int>& nums) {
    if (nums.size() <= 1) return;
    
    int i = static_cast<int>(nums.size()) - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    }
    
    if (i >= 0) {
        int j = static_cast<int>(nums.size()) - 1;
        while (nums[j] <= nums[i]) {
            j--;
        }
        swap(nums[i], nums[j]);
    }
    
    reverse(nums.begin() + i + 1, nums.end());
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
    
    nextPermutation(nums);
    
    cout << "\nThe next lexicographical permutation is: ";
    for (int i = 0; i < n; ++i) {
        cout << nums[i] << " ";
    }
    cout << endl;
    
    return 0;
}