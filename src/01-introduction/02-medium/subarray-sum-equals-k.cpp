#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/*
CORE LOGIC (PREFIX SUM OPTIMIZATION USING HASH MAP):
- We maintain a running `currentPrefixSum` as we iterate through the array.
- A subarray summing to `k` exists between indices `j` and `i` (where `j < i`) if the difference between their prefix sums is exactly `k`: `currentPrefixSum - prefixSum[j] = k`.
- Rearranging this gives: `prefixSum[j] = currentPrefixSum - k`.
- Therefore, at each element, we check if `currentPrefixSum - k` has been seen before. If it has, we add its frequency count to our total `count`.
- We use an `unordered_map` to store the frequency of each prefix sum encountered so far.
- The map is initialized with `{0, 1}` because a prefix sum of 0 has occurred once implicitly before processing any elements (handling subarrays starting from index 0).

TIME COMPLEXITY: O(N) - We perform a single linear traversal through the array of size N. Hash map operations take O(1) average time.
SPACE COMPLEXITY: O(N) - In the worst case, the hash map will store up to N distinct prefix sums.
*/
int subarraySum(const vector<int>& nums, int k) {
    unordered_map<int, int> prefixSumCounts;
    prefixSumCounts[0] = 1;
    
    int currentPrefixSum = 0;
    int count = 0;
    
    for (int num : nums) {
        currentPrefixSum += num;
        int targetPrefix = currentPrefixSum - k;
        
        if (prefixSumCounts.count(targetPrefix)) {
            count += prefixSumCounts[targetPrefix];
        }
        
        prefixSumCounts[currentPrefixSum]++;
    }
    
    return count;
}

int main() {
    int n, k;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    cout << "Enter the target sum integer k: ";
    cin >> k;
    
    int result = subarraySum(nums, k);
    cout << "\nTotal number of subarrays that sum to k: " << result << endl;
    
    return 0;
}