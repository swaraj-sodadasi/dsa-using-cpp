#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: JUMP GAME VI (MONOTONIC DEQUE DP OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * The objective is to reach the last index with the maximum score, where from index i
     * you can jump to any index in the range [i + 1, min(n - 1, i + k)].
     * 
     * 1. DP RELATION:
     *    dp[i] = nums[i] + max(dp[j]) for j in [max(0, i - k), i - 1]
     * 
     * 2. MONOTONIC DEQUE OPTIMIZATION:
     *    A naive DP approach checks all k previous states for each index, resulting in O(N * K).
     *    By maintaining a monotonically decreasing deque of indices based on their DP values:
     *      - The front of the deque (`dq.front()`) always stores the index with the maximum DP score
     *        within the valid jump window [i - k, i - 1].
     *      - Before adding `i`, we pop elements from the back of the deque whose DP scores are 
     *        less than or equal to `dp[i]` to preserve decreasing monotonicity.
     *      - Before querying the maximum, we pop indices from the front that fall outside the 
     *        sliding window `[i - k, i - 1]`.
     * 
     * This reduces the runtime from O(N * K) to O(N).
     */
    int maxResult(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        vector<int> dp(static_cast<size_t>(n));
        dp[0] = nums[0];

        // Monotonic deque storing indices, ordered such that dp values are non-increasing
        deque<int> dq;
        dq.push_back(0);

        for (int i = 1; i < n; ++i) {
            // Remove indices that are out of the sliding window boundary [i - k]
            while (!dq.empty() && dq.front() < i - k) {
                dq.pop_front();
            }

            // Optimal transition: current value + max DP value in window
            dp[static_cast<size_t>(i)] = nums[static_cast<size_t>(i)] + dp[static_cast<size_t>(dq.front())];

            // Maintain decreasing monotonic order in the deque
            while (!dq.empty() && dp[static_cast<size_t>(dq.back())] <= dp[static_cast<size_t>(i)]) {
                dq.pop_back();
            }

            dq.push_back(i);
        }

        return dp[static_cast<size_t>(n - 1)];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Each index is pushed and popped from the deque at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - DP table array of size N and sliding window deque of size at most K.
 */
int main() {
    cout << "=== Jump Game VI Monotonic Deque Engine ===\n";
    cout << "Enter the number of elements in array: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter maximum jump limit k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    int maxScore = solver.maxResult(nums, k);

    cout << "Maximum achievable score: [" << maxScore << "]\n";

    return 0;
}