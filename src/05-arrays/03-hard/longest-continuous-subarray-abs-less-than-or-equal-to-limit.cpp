#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * SLIDING WINDOW WITH MAX-MIN MONOTONIC DEQUES ENGINE
     * ---------------------------------------------------
     * The objective is to find the maximum length of a contiguous subarray such that 
     * the absolute difference between any two elements of this subarray is less than 
     * or equal to a given limit.
     *
     * Algorithmic Optimization Strategy:
     * - A naive approach checks the absolute difference of all subarrays, taking O(N^2) or O(N^3) time.
     * - We optimize this to strict O(N) linear time using a sliding window combined with two 
     *   monotonic double-ended queues (deques). 
     * - The deques act as structural trackers to maintain the maximum and minimum values 
     *   currently within our dynamic window frame:
     *     1. `maxDeque`: Monotonically decreasing tracking storage. Stores values in descending 
     *        order, keeping the current maximum element at the front.
     *     2. `minDeque`: Monotonically increasing tracking storage. Stores values in ascending 
     *        order, keeping the current minimum element at the front.
     *
     * Window Maintenance Sequence:
     * - Step 1: Expand the window by sliding the `right` pointer forward, inserting `nums[right]` 
     *   into both deques while maintaining their monotonic properties.
     * - Step 2: Check the validity condition. The maximum difference inside the current window is 
     *   always `maxDeque.front() - minDeque.front()`. 
     * - Step 3: If this difference exceeds `limit`, the window is invalid. We must shrink it from the left 
     *   by sliding the `left` pointer forward. If `nums[left]` matches the front element of either deque, 
     *   we pop that element out since it is leaving the window boundaries.
     * - Step 4: Throughout the sweep, calculate the valid window dimension (`right - left + 1`) 
     *   and capture the maximum length encountered.
     */
    int longestSubarray(vector<int>& nums, int limit) {
        // Monotonic deques tracking max and min element values inside the active sliding window
        deque<int> maxDeque;
        deque<int> minDeque;
        
        size_t left = 0;
        int maxLengthResult = 0;
        size_t n = nums.size();

        // Step 1: Slide the right pointer forward to expand the window
        for (size_t right = 0; right < n; ++right) {
            int currentVal = nums[right];

            // Maintain decreasing property for maxDeque: pop smaller elements from the back
            while (!maxDeque.empty() && maxDeque.back() < currentVal) {
                maxDeque.pop_back();
            }
            maxDeque.push_back(currentVal);

            // Maintain increasing property for minDeque: pop larger elements from the back
            while (!minDeque.empty() && minDeque.back() > currentVal) {
                minDeque.pop_back();
            }
            minDeque.push_back(currentVal);

            // Step 3: Shrink the window if the max-min difference breaks the limit constraint
            while (!maxDeque.empty() && !minDeque.empty() && (maxDeque.front() - minDeque.front() > limit)) {
                // If the element leaving the window is the current max or min, drop it from the deque
                if (nums[left] == maxDeque.front()) {
                    maxDeque.pop_front();
                }
                if (nums[left] == minDeque.front()) {
                    minDeque.pop_front();
                }
                left++; // Shrink left boundary forward
            }

            // Step 4: Calculate the size of the active valid window frame
            int currentWindowSize = static_cast<int>(right - left + 1);
            maxLengthResult = max(maxLengthResult, currentWindowSize);
        }

        return maxLengthResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items inside the array.
 *                  Although there are nested while loops, every element is pushed onto and 
 *                  popped off the deques at most once across the entire sweep runtime, 
 *                  ensuring strict amortized O(1) time per element.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - In the worst-case scenario, the monotonic deques can store 
 *                   up to N elements if the sequence is strictly sorted.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int limit;
    cout << "Enter the absolute difference limit threshold: ";
    if (!(cin >> limit) || limit < 0) {
        cout << "Invalid limit constraint parameter settings." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting sliding window with max-min monotonic deques sweep..." << endl;
    int result = solver.longestSubarray(nums, limit);

    cout << "Maximum contiguous length matching boundary criteria: " << result << endl;

    return 0;
}