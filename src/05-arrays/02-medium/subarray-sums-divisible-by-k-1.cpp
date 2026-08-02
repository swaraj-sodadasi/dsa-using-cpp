#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * MODULAR PREFIX SUM FREQUENCY HASHING ENGINE
     * -------------------------------------------
     * The objective is to find the total number of contiguous subarrays whose sums 
     * are divisible by a given integer 'k'.
     *
     * Mathematical & Algorithmic Mechanics:
     * - A naive solution checks all subarray combinations, taking O(N^2) time.
     * - We optimize this to O(N) linear time by leveraging prefix sums and modular arithmetic.
     * - Let `prefixSum[i]` be the sum of elements from index 0 to i. The sum of a subarray 
     *   from index `j + 1` to `i` is: `subarraySum = prefixSum[i] - prefixSum[j]`.
     * - For `subarraySum` to be divisible by `k`, its remainder modulo `k` must be 0:
     *   `(prefixSum[i] - prefixSum[j]) % k == 0`  =>  `prefixSum[i] % k == prefixSum[j] % k`.
     * - Therefore, whenever we encounter a prefix sum remainder that has occurred before, 
     *   every past occurrence marks the end of a valid subarray ending at our current index.
     *
     * Negative Remainder Standardization Rule:
     * - In C++, the `%` operator can return a negative value if the dividend is negative 
     *   (e.g., `-5 % 3 = -2`). Mathematically, a remainder of `-2` under modulo 3 is equivalent 
     *   to a positive remainder of `1` (`-2 + 3 = 1`).
     * - To handle this consistently, we standardize all remainders to be strictly positive:
     *   `normalizedRemainder = (currentRemainder % k + k) % k`.
     *
     * State Window Evaluation Sequence:
     * - Initialize a hash map or fixed-size array `remainderCounts` to track remainder frequencies.
     * - **Critical Seed Step:** Increment the frequency of remainder `0` by 1 (`remainderCounts[0] = 1`). 
     *   This correctly captures any prefix sum that is directly divisible by `k` on its own.
     * - Iterate through the array, keeping a running `runningSum`.
     * - At each step, compute the `normalizedRemainder`.
     * - If this remainder already exists in our tracking history, add its frequency count to our 
     *   global `divisibleSubarraysCount`.
     * - Update the registry by incrementing the count for this remainder.
     */
    int subarraysDivByK(vector<int>& nums, int k) {
        int divisibleSubarraysCount = 0;
        int runningSum = 0;

        // Since remainders will always fall within the range [0, k - 1], we can use a fixed-size 
        // frequency vector instead of a hash map to reduce execution overhead.
        vector<int> remainderCounts(static_cast<size_t>(k), 0);
        
        // Base case configuration: a clean remainder of 0 has been observed once initially
        remainderCounts[0] = 1;

        // Perform a single linear pass across the array
        for (int num : nums) {
            runningSum += num;

            // Compute remainder and normalize to guarantee a positive value in the range [0, k - 1]
            int normalizedRemainder = ((runningSum % k) + k) % k;

            // Accumulate the number of times this specific remainder has been seen before
            divisibleSubarraysCount += remainderCounts[static_cast<size_t>(normalizedRemainder)];

            // Register the current remainder state into the history pool
            remainderCounts[static_cast<size_t>(normalizedRemainder)]++;
        }

        return divisibleSubarraysCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array. 
 *                  The logic performs a single linear pass over the dataset. Remainder lookup 
 *                  and array entry mutations execute in constant O(1) time.
 *
 * SPACE COMPLEXITY: O(k) Auxiliary - The optimization framework allocates a fixed allocation 
 *                   vector of size k to track remainder frequencies, matching the divisor constraint.
 */
int main() {
    int n, k;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (integers, can be positive, negative, or zero) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the target divisor parameter (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid divisor constraint. The value of k must be greater than 0." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting modular prefix sum frequency hashing sweep..." << endl;
    int result = solver.subarraysDivByK(nums, k);

    cout << "Total matching divisible contiguous subarrays identified: " << result << endl;

    return 0;
}