#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: PREFIX SUM FREQUENCY MAP ISOLATION
     * -------------------------------------------------------------------------------------
     * The objective is to find the total number of continuous subarrays whose sum equals k.
     * 
     * 1. THE MATHEMATICAL FOUNDATION (PREFIX SUMS):
     *    Let `prefixSum[i]` be the cumulative sum of elements from index 0 to i.
     *    The sum of a subarray spanning from index `j + 1` to `i` is calculated as:
     *    `SubarraySum = prefixSum[i] - prefixSum[j]`
     *    We want this `SubarraySum` to equal `k`. Rearranging the equation gives:
     *    `prefixSum[j] = prefixSum[i] - k`
     *    This means if the value `prefixSum[i] - k` has been encountered previously at an 
     *    earlier index `j`, the subarray between `j + 1` and `i` sums exactly to `k`.
     * 
     * 2. SINGLE-PASS FREQUENCY MAPPING:
     *    Instead of executing nested loops to scan all historical positions—which costs O(N^2) 
     *    time—we maintain an `unordered_map` to store the frequencies of all prefix sums seen 
     *    so far (`prefixSum -> occurrenceCount`).
     *    As we loop through the array, we track the current cumulative `runningPrefixSum`. At each 
     *    step, we calculate the required historical complement: `complement = runningPrefixSum - k`.
     *    If this complement exists in our map, we add its frequency count directly to our total.
     * 
     * 3. CRITICAL BASE CASE SAFETY NET:
     *    Before starting the loop, we initialize the map with `{0: 1}`. This handles the case where 
     *    a subarray starting exactly at index 0 sums to `k` (since `runningPrefixSum - k == 0`).
     */
    int subarraySum(vector<int>& nums, int k) {
        // Map stores: {Historical_Prefix_Sum -> Occurrence_Count}
        unordered_map<int, int> prefixSumFreqMap;
        
        // Base case: A prefix sum of 0 has occurred exactly once (before any elements are added)
        prefixSumFreqMap[0] = 1;

        int runningPrefixSum = 0;
        int totalSubarraysCount = 0;

        for (int num : nums) {
            runningPrefixSum += num;
            int requiredComplement = runningPrefixSum - k;

            // Check if the required complement prefix sum has been seen before
            if (prefixSumFreqMap.find(requiredComplement) != prefixSumFreqMap.end()) {
                totalSubarraysCount += prefixSumFreqMap[requiredComplement];
            }

            // Register the current running prefix sum into the lookup table
            prefixSumFreqMap[runningPrefixSum]++;
        }

        return totalSubarraysCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The loop parses each element exactly once, performing hash map insertions 
 *                  and search lookups in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - In the worst-case scenario where every continuous prefix sum 
 *                   is distinct, the hash map stores up to N distinct key-value pairs.
 */
int main() {
    int count, k;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> count) || count <= 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(count));
    cout << "Enter the array elements sequentially:\n";
    for (int i = 0; i < count; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the target sum value (k): ";
    cin >> k;

    Solution solver;
    cout << "\nExecuting single-pass prefix sum frequency mapping..." << endl;
    int matchesCount = solver.subarraySum(nums, k);

    cout << "Total continuous subarrays matching target sum count: " << matchesCount << endl;

    return 0;
}