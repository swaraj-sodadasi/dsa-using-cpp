#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CONSTRAINED SUBARRAY SUM (MONOTONIC DEQUE OPTIMIZED DP)
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum sum of a non-empty subarray such that for every 
     * two consecutive elements in the subarray at indices i and j (with i < j), j - i <= k.
     * 
     * 1. DP TRANSITION:
     *    `dp[i] = nums[i] + max(0, max(dp[j]))` for `j` in range `[max(0, i - k), i - 1]`.
     * 
     * 2. MONOTONIC DEQUE OPTIMIZATION:
     *    - We maintain a monotonically decreasing deque storing indices `j`.
     *    - `dq.front()` always holds the index with the maximum DP score in window `[i - k, i - 1]`.
     *    - Expunge indices outside the sliding window boundary (`dq.front() < i - k`).
     *    - Maintain non-increasing monotonicity by popping indices from back whose DP value `<= dp[i]`.
     *    - Only push index `i` to deque if `dp[i] > 0` (since negative DP values never contribute 
     *      positively to future subsums).
     */
    int constrainedSubsetSum(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        vector<int> dp(static_cast<size_t>(n));
        deque<int> dq; // Monotonically decreasing deque storing indices based on DP values

        int maxSum = nums[0];

        for (int i = 0; i < n; ++i) {
            // Remove indices outside the window [i - k, i - 1]
            if (!dq.empty() && dq.front() < i - k) {
                dq.pop_front();
            }

            // Calculate DP transition using max value from deque
            int maxPrev = !dq.empty() ? dp[static_cast<size_t>(dq.front())] : 0;
            dp[static_cast<size_t>(i)] = nums[static_cast<size_t>(i)] + max(0, maxPrev);

            maxSum = max(maxSum, dp[static_cast<size_t>(i)]);

            // Maintain decreasing order of DP values in deque
            while (!dq.empty() && dp[static_cast<size_t>(dq.back())] <= dp[static_cast<size_t>(i)]) {
                dq.pop_back();
            }

            // Only push if dp[i] > 0 as non-positive sums won't help future subarrays
            if (dp[static_cast<size_t>(i)] > 0) {
                dq.push_back(i);
            }
        }

        return maxSum;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Each index is pushed and popped from the deque at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Space allocated for DP table array and monotonic deque.
 */
int main() {
    cout << "=== Constrained Subarray Sum Monotonic Deque Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter maximum distance constraint k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    int maxSubSum = solver.constrainedSubsetSum(nums, k);

    cout << "Maximum constrained subarray sum: [" << maxSubSum << "]\n";

    return 0;
}