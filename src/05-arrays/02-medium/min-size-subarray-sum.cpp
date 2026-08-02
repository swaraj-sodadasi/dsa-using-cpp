#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * VARIABLE-LENGTH SLIDING WINDOW EXPANSION/CONTRACTION ENGINE
     * -----------------------------------------------------------
     * The objective is to find the minimal length of a contiguous subarray of which the 
     * sum is greater than or equal to a target value. If no such subarray exists, return 0.
     *
     * Architectural Tracking Mechanics:
     * - A naive solution computes the sum of all possible subarrays, taking O(N^2) time.
     * - We optimize this to O(N) linear time using a two-pointer sliding window technique. 
     *   The window expands by moving a `right` pointer to include elements, and contracts 
     *   by moving a `left` pointer to discard elements from the start of the window.
     *
     * Window Modification Sequence:
     * - Step 1: Initialize `left = 0`, `currentWindowSum = 0`, and set `minSubarrayLength` 
     *   to an upper bound index boundary indicator (`INT_MAX`).
     * - Step 2: Iterate through the array using a `right` pointer to expand the window, 
     *   adding `nums[right]` to `currentWindowSum`.
     * - Step 3: While `currentWindowSum >= target`, a valid window is found. 
     *   - Calculate the active window length: `right - left + 1`.
     *   - Update `minSubarrayLength` with the minimum length found so far.
     *   - Greedily attempt to contract the window from the left by subtracting `nums[left]` 
     *     from `currentWindowSum` and incrementing `left`.
     * - Step 4: After sweeping the array, check if `minSubarrayLength` was updated. If it 
     *   remains `INT_MAX`, no valid subarray was found, so we return 0.
     */
    int minSubArrayLen(int target, vector<int>& nums) {
        int minSubarrayLength = INT_MAX;
        int currentWindowSum = 0;
        size_t left = 0;
        size_t n = nums.size();

        // Expand the right boundary window frame step-by-step
        for (size_t right = 0; right < n; ++right) {
            currentWindowSum += nums[right];

            // Contract the window from the left as long as the target sum is met
            while (currentWindowSum >= target) {
                int currentWindowLength = static_cast<int>(right - left + 1);
                minSubarrayLength = min(minSubarrayLength, currentWindowLength);

                // Evict the element at the left index pointer and slide the boundary forward
                currentWindowSum -= nums[left];
                left++;
            }
        }

        // Return 0 if the tracking constraint value was never updated
        return (minSubarrayLength == INT_MAX) ? 0 : minSubarrayLength;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  Although there is a nested while loop, each element is visited at most 
 *                  twice (once when expanding the window using the `right` pointer, and at 
 *                  most once when contracting using the `left` pointer), ensuring strict linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sliding window boundaries are tracked using simple 
 *                   scalar index pointers, requiring no extra memory layout footprints on the heap.
 */
int main() {
    int n, target;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter positive array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Array elements must be strictly positive integers." << endl;
            return 1;
        }
    }

    cout << "Enter the target subarray sum boundary value: ";
    if (!(cin >> target) || target <= 0) {
        cout << "Invalid target parameter constraint." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting variable-length sliding window expansion/contraction sweep..." << endl;
    int result = solver.minSubArrayLen(target, nums);

    if (result > 0) {
        cout << "Minimal length of a matching contiguous subarray sequence: " << result << endl;
    } else {
        cout << "Result: 0 (No valid contiguous subarray matches the target sum constraint)" << endl;
    }

    return 0;
}