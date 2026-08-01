#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Solution {
public:
    /*
     * FIXED-SIZE SLIDING WINDOW ACCUMULATION ENGINE
     * ---------------------------------------------
     * The problem asks us to find a contiguous subarray of a fixed length 'k' 
     * that has the maximum average value, and return this maximum average.
     *
     * Mathematical Optimization Technique:
     * - A naive solution recalculates the sum of all elements in the window every time 
     *   the window moves forward, leading to a slow O(N * k) time footprint.
     * - We optimize this by using a fixed-size sliding window. Since the denominator 'k' 
     *   is constant, maximizing the average value is mathematically equivalent to 
     *   maximizing the sum of the window elements.
     *
     * State Window Evaluation Sequence:
     * - Phase 1 (Seeding): Calculate the sum of the first 'k' elements (`nums[0]` through `nums[k-1]`) 
     *   to establish our baseline tracking window sum.
     * - Phase 2 (Sliding Sweep): Slide the window forward one element at a time from index `i = k` 
     *   to the end of the array.
     * - For each step, we update the window sum in constant O(1) time by adding the new element 
     *   entering the window (`nums[i]`) and subtracting the old element falling out of the 
     *   window (`nums[i - k]`).
     * - We track the maximum window sum encountered during the sweep and divide it by 'k' at the 
     *   very end to compute the precise maximum average.
     */
    double findMaxAverage(vector<int>& nums, int k) {
        size_t windowSize = static_cast<size_t>(k);
        long long currentWindowSum = 0;

        // PHASE 1: Seed the initial window sum with the first k elements
        for (size_t i = 0; i < windowSize; ++i) {
            currentWindowSum += nums[i];
        }

        long long maxWindowSum = currentWindowSum;

        // PHASE 2: Slide the window across the remaining elements of the array
        for (size_t i = windowSize; i < nums.size(); ++i) {
            // Update the window sum in O(1) time by adding the incoming and removing the outgoing element
            currentWindowSum += nums[i] - nums[i - windowSize];
            
            // Maintain the maximum window sum observed so far
            maxWindowSum = max(maxWindowSum, currentWindowSum);
        }

        // Return the final maximum average using static casting to prevent integer division truncation
        return static_cast<double>(maxWindowSum) / k;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array. 
 *                  The logic performs exactly one linear pass over the input sequence, 
 *                  updating the running sub-total tracking variables in constant O(1) time per step.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation requires no extra memory allocation tracking 
 *                   frameworks on the heap, maintaining a fixed scalar profile throughout.
 */
int main() {
    int n, k;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    cout << "Enter the fixed subarray target sliding window size (k): ";
    if (!(cin >> k) || k <= 0 || k > n) {
        cout << "Invalid sizing parameters. Window size must satisfy 0 < k <= N." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting fixed-size sliding window accumulation sweep..." << endl;
    double maxAverageResult = solver.findMaxAverage(nums, k);

    // Set output formatting configuration for precision display
    cout << fixed << setprecision(5);
    cout << "Maximum derived contiguous subarray average value outcome: " << maxAverageResult << endl;

    return 0;
}