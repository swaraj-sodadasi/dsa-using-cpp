#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SHORTEST SUBARRAY WITH SUM AT LEAST K (MONOTONIC DEQUE PREFIX SUM)
     * -------------------------------------------------------------------------------------
     * The objective is to find the length of the shortest non-empty subarray with sum >= k.
     * Since array elements can be negative, standard two-pointer sliding window fails.
     * 
     * 1. PREFIX SUM ARRAY CONSTRUCTION:
     *    `prefix[i]` stores sum of first `i` elements. The sum of subarray `nums[j...i-1]` 
     *    is `prefix[i] - prefix[j]`. We seek `min(i - j)` such that `prefix[i] - prefix[j] >= k`.
     * 
     * 2. MONOTONIC INCREASING DEQUE OPTIMIZATION:
     *    We maintain indices `j` in a deque such that `prefix[j]` is strictly increasing.
     *    
     *    - OPTIMIZATION 1 (Shrink from front):
     *      If `prefix[i] - prefix[dq.front()] >= k`, we found a valid subarray of length `i - dq.front()`.
     *      We can pop `dq.front()` permanently because any future index `i'` (> i) would yield a 
     *      longer subarray `i' - dq.front()`, so `dq.front()` will never yield a shorter valid answer.
     * 
     *    - OPTIMIZATION 2 (Maintain increasing order from back):
     *      Before pushing `i`, while `prefix[dq.back()] >= prefix[i]`, we pop `dq.back()`.
     *      Why? Because `prefix[i]` is smaller and has a larger index `i`, making it a strictly better 
     *      starting boundary for future subarrays than `dq.back()`.
     */
    int shortestSubarray(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        vector<long long> prefix(static_cast<size_t>(n + 1), 0);

        for (int i = 0; i < n; ++i) {
            prefix[static_cast<size_t>(i + 1)] = prefix[static_cast<size_t>(i)] + nums[static_cast<size_t>(i)];
        }

        int minLen = n + 1;
        deque<int> dq; // Monotonically increasing deque of prefix sum indices

        for (int i = 0; i <= n; ++i) {
            // Check if current prefix sum forms a valid sum >= k with front of deque
            while (!dq.empty() && prefix[static_cast<size_t>(i)] - prefix[static_cast<size_t>(dq.front())] >= k) {
                minLen = min(minLen, i - dq.front());
                dq.pop_front();
            }

            // Maintain increasing order of prefix sums in deque
            while (!dq.empty() && prefix[static_cast<size_t>(i)] <= prefix[static_cast<size_t>(dq.back())]) {
                dq.pop_back();
            }

            dq.push_back(i);
        }

        return minLen <= n ? minLen : -1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the size of array `nums`.
 *                  Each index is pushed and popped from the deque at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Prefix sum array and deque buffer of size N + 1.
 */
int main() {
    cout << "=== Shortest Subarray with Sum at Least K Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (can include negative values):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter target sum k: ";
    int k;
    if (!(cin >> k)) return 1;

    Solution solver;
    int result = solver.shortestSubarray(nums, k);

    cout << "Shortest subarray length with sum >= " << k << ": [" << result << "]\n";

    return 0;
}