#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CONTINUOUS SUBARRAYS (DUAL MONOTONIC DEQUE SLIDING WINDOW)
     * -------------------------------------------------------------------------------------
     * The objective is to count all continuous subarrays where the absolute difference 
     * between any two elements is at most 2.
     * 
     * 1. DUAL MONOTONIC DEQUES:
     *    - `maxDeque`: Monotonically decreasing deque tracking indices of max elements.
     *    - `minDeque`: Monotonically increasing deque tracking indices of min elements.
     * 
     * 2. SLIDING WINDOW & SUBARRAY COUNTING:
     *    - Expand `right` and maintain monotonic order in both deques.
     *    - While `nums[maxDeque.front()] - nums[minDeque.front()] > 2`, contract from `left`:
     *      pop front indices if they equal `left`, then `left++`.
     *    - For each valid `right`, all subarrays ending at `right` and starting between 
     *      `left` and `right` are valid. Count added is `right - left + 1`.
     */
    long long continuousSubarrays(const vector<int>& nums) {
        deque<int> maxDeque;
        deque<int> minDeque;

        int left = 0;
        long long totalSubarrays = 0;

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

            // Shrink window if maximum difference exceeds 2
            while (nums[static_cast<size_t>(maxDeque.front())] - nums[static_cast<size_t>(minDeque.front())] > 2) {
                if (maxDeque.front() == left) {
                    maxDeque.pop_front();
                }
                if (minDeque.front() == left) {
                    minDeque.pop_front();
                }
                left++;
            }

            totalSubarrays += (right - left + 1);
        }

        return totalSubarrays;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of elements in nums.
 *                  Each element is pushed and popped from both deques at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Space allocated for the two monotonic deques.
 */
int main() {
    cout << "=== Continuous Subarrays Monotonic Deque Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    long long result = solver.continuousSubarrays(nums);

    cout << "Total continuous valid subarrays: [" << result << "]\n";

    return 0;
}