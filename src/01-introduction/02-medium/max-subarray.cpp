#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (KADANE'S ALGORITHM):
- We maintain two scalar variables while traversing the array: `currentMax` (the maximum sum ending at the current position) and `globalMax` (the overall maximum subarray sum found so far).
- For each element `nums[i]`, we decide whether to add it to the existing subarray sequence or start a fresh subarray at `nums[i]`. This local decision is expressed as: `currentMax = max(nums[i], currentMax + nums[i])`.
- After updating `currentMax`, we update `globalMax` if the newly computed local sequence sum surpasses our historical maximum: `globalMax = max(globalMax, currentMax)`.
- Initializing `globalMax` to the first element guarantees correct handling of arrays containing only negative values.

TIME COMPLEXITY: O(N) - We perform a single linear traversal over the vector layout of size N.
SPACE COMPLEXITY: O(1) - The algorithm operates in-place using only two tracking scalar variables.
*/
int maxSubArray(const vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int currentMax = nums[0];
    int globalMax = nums[0];
    
    for (size_t i = 1; i < nums.size(); ++i) {
        currentMax = max(nums[i], currentMax + nums[i]);
        globalMax = max(globalMax, currentMax);
    }
    
    return globalMax;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " integers (can include negative values) separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    int result = maxSubArray(nums);
    cout << "\nThe maximum contiguous subarray sum is: " << result << endl;
    
    return 0;
}