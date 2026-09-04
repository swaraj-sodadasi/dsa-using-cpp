#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SUBARRAY WITH ELEMENTS GREATER THAN VARYING THRESHOLD
     * -------------------------------------------------------------------------------------
     * We need to find a subarray of length k such that every element in it is greater 
     * than (threshold / k). This is equivalent to: min(subarray) > threshold / k,
     * which translates to: min(subarray) * k > threshold.
     * 
     * 1. MONOTONIC STACK / DEQUE FOR NEXT/PREVIOUS SMALLER ELEMENTS:
     *    For each element nums[i], if it acts as the minimum element of a subarray,
     *    we find the maximal contiguous range [L_i, R_i] where nums[i] is the minimum.
     * 
     * 2. BOUNDARY CALCULATION:
     *    - `leftBoundary[i]`: Index of the first element to the left strictly smaller than nums[i].
     *    - `rightBoundary[i]`: Index of the first element to the right strictly smaller than nums[i].
     *    - Maximum length `k` of a subarray where nums[i] is the minimum is:
     *      `k = rightBoundary[i] - leftBoundary[i] - 1`.
     * 
     * 3. THRESHOLD VALIDATION:
     *    If `static_cast<long long>(nums[i]) * k > threshold`, then `k` is a valid length,
     *    and any such subarray of length `k` satisfies the condition.
     */
    int validSubarraySize(const vector<int>& nums, int threshold) {
        int n = static_cast<int>(nums.size());
        vector<int> leftBoundary(static_cast<size_t>(n), -1);
        vector<int> rightBoundary(static_cast<size_t>(n), n);

        deque<int> dq;

        // Find Previous Smaller Element for each index
        for (int i = 0; i < n; ++i) {
            while (!dq.empty() && nums[static_cast<size_t>(dq.back())] >= nums[static_cast<size_t>(i)]) {
                dq.pop_back();
            }
            if (!dq.empty()) {
                leftBoundary[static_cast<size_t>(i)] = dq.back();
            }
            dq.push_back(i);
        }

        dq.clear();

        // Find Next Smaller Element for each index
        for (int i = n - 1; i >= 0; --i) {
            while (!dq.empty() && nums[static_cast<size_t>(dq.back())] >= nums[static_cast<size_t>(i)]) {
                dq.pop_back();
            }
            if (!dq.empty()) {
                rightBoundary[static_cast<size_t>(i)] = dq.back();
            }
            dq.push_back(i);
        }

        // Evaluate maximal length for each candidate minimum element
        for (int i = 0; i < n; ++i) {
            int k = rightBoundary[static_cast<size_t>(i)] - leftBoundary[static_cast<size_t>(i)] - 1;
            if (static_cast<long long>(nums[static_cast<size_t>(i)]) * k > threshold) {
                return k;
            }
        }

        return -1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of elements in nums.
 *                  Each element is pushed and popped from the deque/stack at most twice.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Arrays for left and right boundaries and deque buffer.
 */
int main() {
    cout << "=== Subarray With Elements Greater Than Varying Threshold Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter threshold value: ";
    int threshold;
    if (!(cin >> threshold) || threshold < 0) return 1;

    Solution solver;
    int result = solver.validSubarraySize(nums, threshold);

    cout << "Valid subarray length k: [" << result << "]\n";

    return 0;
}