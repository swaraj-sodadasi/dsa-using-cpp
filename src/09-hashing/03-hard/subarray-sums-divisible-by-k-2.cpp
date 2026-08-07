#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: NEGATIVE MODULO-SAFE PREFIX HASH MAPS
     * -------------------------------------------------------------------------------------
     * The objective is to find the total number of continuous subarrays whose sum is 
     * perfectly divisible by k (i.e., SubarraySum % k == 0).
     * 
     * 1. THE DIVISIBILITY CRITERION (PREFIX REMAINDR EQUIVALENCE):
     *    Let `prefixSum[i]` be the cumulative sum of elements from index 0 to i.
     *    The sum of a subarray spanning from index `j + 1` to `i` is given by:
     *    `SubarraySum = prefixSum[i] - prefixSum[j]`
     *    We want `SubarraySum % k == 0`, which translates to:
     *    `(prefixSum[i] - prefixSum[j]) % k == 0`  ==>  `prefixSum[i] % k == prefixSum[j] % k`
     *    Therefore, if we encounter the same prefix sum remainder at two different indices, 
     *    the elements between them form a subarray whose sum is divisible by k.
     * 
     * 2. CRITICAL NEGATIVE MODULO STANDARD RESILIENCE:
     *    In C++, the modulo operator `%` preserves the sign of the numerator. For example, 
     *    `-5 % 3` yields `-2`. However, mathematically, a remainder of `-2` under divisor 3 
     *    is equivalent to a positive remainder of `1` (since $-2 + 3 = 1$).
     *    To map equivalents correctly, we normalize every raw remainder using the formula:
     *    `normalizedRemainder = ((rawRemainder % k) + k) % k`
     *    This shifts negative remainders into a uniform positive range [0, k - 1].
     * 
     * 3. FREQUENCY MAP LOOKUP ACCUMULATION:
     *    We maintain an `unordered_map<int, int>` to track the historical counts of each 
     *    remainder (`remainder -> frequency`). For every element processed:
     *      - We update the `runningPrefixSum` and compute its `normalizedRemainder`.
     *      - If this remainder has been seen before, it means there are `frequencyMap[normalizedRemainder]` 
     *        historical starting points that form a divisible subarray ending at the current index. 
     *        We add this count to our global total.
     *      - We then increment the remainder's frequency in the map.
     * 
     * 4. INITIALIZATION REMAINDER INVARIANT BASE CASE:
     *    We initialize the map with `{0: 1}`. This ensures that if a prefix sum starting exactly 
     *    from index 0 is perfectly divisible by k (`normalizedRemainder == 0`), it is counted correctly.
     */
    int subarraysDivByK(vector<int>& nums, int k) {
        // Map stores: {Normalized_Modulo_Remainder -> Occurrence_Count}
        unordered_map<int, int> remainderFreqMap;
        
        // Base case: Remainder 0 is registered once before processing any elements
        remainderFreqMap[0] = 1;

        int runningPrefixSum = 0;
        int totalSubarraysCount = 0;

        for (int num : nums) {
            runningPrefixSum += num;
            
            // Calculate and normalize the remainder to handle negative integers safely
            int normalizedRemainder = runningPrefixSum % k;
            if (normalizedRemainder < 0) {
                normalizedRemainder += k;
            }

            // If the same remainder exists historically, accumulate its occurrence frequency
            if (remainderFreqMap.find(normalizedRemainder) != remainderFreqMap.end()) {
                totalSubarraysCount += remainderFreqMap[normalizedRemainder];
            }

            // Increment the frequency registry for the current remainder configuration
            remainderFreqMap[normalizedRemainder]++;
        }

        return totalSubarraysCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The single pass loop processes each element exactly once, performing hash map 
 *                  lookups and insertions in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(min(N, k)) Auxiliary - The dictionary registry map grows proportionally 
 *                   to the length of the array N, or caps at size k since there are at most k 
 *                   unique remainders possible.
 */
int main() {
    int totalElementsCount, k;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> totalElementsCount) || totalElementsCount <= 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(totalElementsCount));
    cout << "Enter the array elements consecutively (can include negative integers):\n";
    for (int i = 0; i < totalElementsCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the target divisor base value (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid divisor target constraint configured. k must be greater than 0." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nLaunching negative modulo-safe prefix sum hashing sweeps..." << endl;
    int resultingDivisibleCount = solver.subarraysDivByK(nums, k);

    cout << "Total continuous subarrays divisible by k count result: " << resultingDivisibleCount << endl;

    return 0;
}