#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: LONGEST CONTINUOUS SUBARRAY WITH ABS DIFF <= LIMIT
     * (MIN-MAX DUAL MONOTONIC DEQUE SYNCHRONIZATION)
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum length of a continuous subarray such that the 
     * absolute difference between any two elements is at most `limit`.
     * 
     * 1. DUAL MONOTONIC DEQUES:
     *    - `maxDeque`: Monotonically decreasing deque tracking indices of max elements in window.
     *    - `minDeque`: Monotonically increasing deque tracking indices of min elements in window.
     * 
     * 2. SLIDING WINDOW & SYNCHRONIZATION:
     *    - Expand `right` and maintain monotonic order in both deques.
     *    - Check window validity: `nums[maxDeque.front()] - nums[minDeque.front()] <= limit`.
     *    - If violated, contract from `left`: pop indices from deques if they match `left`, then `left++`.
     * 
     * 3. MAXIMUM LENGTH TRACKING:
     *    - Update `maxLen = max(maxLen, right - left + 1)`.
     */
    int longestSubarray(const vector<int>& nums, int limit) {
        deque<int> maxDeque;
        deque<int> minDeque;

        int left = 0;
        int maxLen = 0;

        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            int val = nums[static_cast<size_t>(right)];

            // Maintain decreasing order in maxDeque
            while (!maxDeque.empty() && nums[static_cast<size_t>(maxDeque.back())] <= val) {
                maxDeque.pop_back();
            }
            maxDeque.push_back(right);

            // Maintain increasing order in minDeque
            while (!minDeque.empty() && nums[static_cast<size_t>(minDeque.back())] >= val) {
                minDeque.pop_back();
            }
            minDeque.push_back(right);

            // Shrink window if maximum difference exceeds limit
            while (nums[static_cast<size_t>(maxDeque.front())] - nums[static_cast<size_t>(minDeque.front())] > limit) {
                if (maxDeque.front() == left) {
                    maxDeque.pop_front();
                }
                if (minDeque.front() == left) {
                    minDeque.pop_front();
                }
                left++;
            }

            maxLen = max(maxLen, right - left + 1);
        }

        return maxLen;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of elements in nums.
 *                  Each element is pushed and popped from both deques at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Space allocated for the two deques storing indices.
 */
int main() {
    cout << "=== Min-Max Dual Monotonic Deque Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter limit: ";
    int limit;
    if (!(cin >> limit) || limit < 0) return 1;

    Solution solver;
    int ans = solver.longestSubarray(nums, limit);

    cout << "Longest valid subarray length: [" << ans << "]\n";

    return 0;
}