#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MODULAR PREFIX SUM REMAINDER MATCHING
     * -------------------------------------------------------------------------------------
     * The objective is to determine if the array contains a continuous subarray of size 
     * AT LEAST TWO elements whose sum is a multiple of k (i.e., SubarraySum % k == 0).
     * 
     * 1. THE MATHEMATICAL CRITERION (PREFIX REMAINDR EQUIVALENCE):
     *    Let `prefixSum[i]` be the cumulative sum of elements from index 0 to i.
     *    The sum of a subarray spanning from index `j + 1` to `i` is given by:
     *    `SubarraySum = prefixSum[i] - prefixSum[j]`
     *    We want `SubarraySum % k == 0`, which means:
     *    `(prefixSum[i] - prefixSum[j]) % k == 0`  ==>  `prefixSum[i] % k == prefixSum[j] % k`
     *    Therefore, if we encounter the same prefix sum remainder at two different indices, 
     *    the elements between them form a subarray whose sum is a multiple of k.
     * 
     * 2. SINGLE-PASS REMAINDER FACTORIZATION REGISTER:
     *    Instead of checking all pairs via nested loops—which costs O(N^2) time—we maintain an 
     *    `unordered_map` to track the *first* index where each remainder is seen (`remainder -> first_seen_index`).
     *    As we loop through the array, we track the cumulative `runningSum` and calculate its 
     *    modulo remainder: `remainder = runningSum % k`.
     *    - Handling Negative Values: The problem parameters state numbers are non-negative, but standardizing 
     *      `((remainder % k) + k) % k` ensures absolute mathematical compatibility.
     * 
     * 3. THE SIZE INVARIANT CONSTRAINT (AT LEAST TWO ELEMENTS):
     *    If the current remainder already exists in our map at `map[remainder]`, we compute the size 
     *    of the subarray: `current_index - map[remainder]`. If this distance is greater than or equal to 2, 
     *    we have successfully verified the size constraint and return true.
     *    If the remainder is new, we store the current index. We do NOT update the index if the remainder 
     *    re-appears, because keeping the earliest index maximizes the chance of satisfying the length requirement.
     * 
     * 4. CRITICAL INITIALIZATION STATE:
     *    Before starting the loop, we initialize the map with `{0: -1}`. This handles cases where a 
     *    valid prefix sum starting exactly at index 0 is perfectly divisible by k (since `i - (-1) = i + 1`, 
     *    correctly calculating the subarray length).
     */
    bool checkSubarraySum(vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        if (n < 2) return false;

        // Map stores: {Modulo_Remainder -> First_Seen_Index_Location}
        unordered_map<int, int> remainderIndexMap;
        
        // Base case: Remainder 0 is implicitly mapped to index -1 before parsing elements
        remainderIndexMap[0] = -1;

        int runningSum = 0;

        for (int i = 0; i < n; ++i) {
            runningSum += nums[static_cast<size_t>(i)];
            
            int remainder = runningSum % k;
            // Normalize remainder to handle potential negative mathematical variations safely
            if (remainder < 0) {
                remainder += k;
            }

            // Check if this remainder has been encountered previously
            if (remainderIndexMap.find(remainder) != remainderIndexMap.end()) {
                // Invariant Check: Validate that the window spans at least two elements
                if (i - remainderIndexMap[remainder] >= 2) {
                    return true;
                }
            } else {
                // Log only the first occurrence to preserve the widest possible window frame
                remainderIndexMap[remainder] = i;
            }
        }

        return false;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The loop parses each element exactly once, performing hash map lookups 
 *                  and updates in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(min(N, k)) Auxiliary - In the worst-case scenario, the hash map grows 
 *                   proportionally to the array size N or bounded by the total unique modular 
 *                   remainders possible under divisor value k.
 */
int main() {
    int totalElementsCount, k;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> totalElementsCount) || totalElementsCount < 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(totalElementsCount));
    cout << "Enter the array elements consecutively (Non-negative integers):\n";
    for (int i = 0; i < totalElementsCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Elements must be non-negative integers." << endl;
            return 1;
        }
    }

    cout << "Enter the divisor modulo target value (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid divisor constraint configured. k must be greater than 0." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting prefix remainder matching and window size checks..." << endl;
    bool hasMatchingSubarray = solver.checkSubarraySum(nums, k);

    if (hasMatchingSubarray) {
        cout << "Outcome: A valid continuous subarray of size >= 2 was found! (Result = true)" << endl;
    } else {
        cout << "Outcome: No satisfying continuous subarray configurations identified. (Result = false)" << endl;
    }

    return 0;
}