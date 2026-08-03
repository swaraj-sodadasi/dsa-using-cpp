#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
private:
    /*
     * MONOTONIC SUBARRAY PARTITIONING FEASIBILITY CHECKER
     * ----------------------------------------------------
     * Determines whether the array can be partitioned into at most `maxSplits` 
     * continuous subarrays such that the maximum sum of any single subarray 
     * does not exceed the target `maxSumLimit`.
     */
    bool isFeasible(const vector<int>& nums, int maxSplits, int maxSumLimit) {
        int continuousSubarrayCount = 1;
        int currentRunningSubarraySum = 0;

        for (int num : nums) {
            // Early failure check: if a single item exceeds the limit, this allocation is impossible
            if (num > maxSumLimit) return false;

            if (currentRunningSubarraySum + num > maxSumLimit) {
                // Seal the current partition container window and begin a new contiguous subarray split
                continuousSubarrayCount++;
                currentRunningSubarraySum = num;

                // Optimization pruning: if required splits exceed target limits, abort exploration branch
                if (continuousSubarrayCount > maxSplits) return false;
            } else {
                currentRunningSubarraySum += num;
            }
        }
        return true;
    }

public:
    /*
     * MINIMAX BINARY SEARCH ON ANSWER / MONOTONIC SUBARRAY PARTITIONING ENGINE
     * ------------------------------------------------------------------------
     * The objective is to split a non-negative integer array into `k` non-empty 
     * continuous subarrays such that the minimized maximum sum of any single 
     * subarray is discovered.
     *
     * Mathematical & Search Framework:
     * - The target minimax sum value exhibits monotonic characteristics: 
     *   If a max sum limit `S` allows the array to be successfully partitioned into 
     *   `k` or fewer continuous chunks, any threshold greater than `S` will also be 
     *   feasible. If it fails, any smaller threshold is guaranteed to fail.
     * - This structural layout allows us to execute a Binary Search on the Answer Range 
     *   to isolate the exact optimal minimax sum bound.
     *
     * Boundary Initialization Protocol:
     * - Lower Bound (`left`): The single maximum element value inside `nums`. A continuous 
     *   subarray cannot contain less than one element, so the result must be at least this value.
     * - Upper Bound (`right`): The absolute total sum of all elements inside `nums`. This 
     *   corresponds to a single partition (`k = 1`), taking the whole array context.
     */
    int splitArray(vector<int>& nums, int k) {
        if (nums.empty()) return 0;

        // Establish the monotonic value domain search window
        int left = *max_element(nums.begin(), nums.end());
        int right = accumulate(nums.begin(), nums.end(), 0);
        int optimizedMinimaxResult = right;

        // Execute binary search across the viable bounded solution continuum
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (isFeasible(nums, k, mid)) {
                optimizedMinimaxResult = mid; // Register the valid sub-partition configuration threshold
                right = mid - 1;              // Attempt to compress the search bound toward lower values
            } else {
                left = mid + 1;               // Maximum limit is too small; force the search space right
            }
        }

        return optimizedMinimaxResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * log(SumNums - MaxNum)) - Where N represents the total elements inside the array. 
 *                  The binary search range bounds span between the peak element value and the array total sum. 
 *                  Each step invokes an O(N) linear partitioning verification loop.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The feasibility split evaluation engine performs completely in-place 
 *                   using simple scalar loop values, requiring zero extra dynamic memory allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter non-negative array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Array elements must be non-negative integers." << endl;
            return 1;
        }
    }

    int k;
    cout << "Enter the targeted subarray split allocation factor (K must be <= elements count): ";
    if (!(cin >> k) || k <= 0 || k > n) {
        cout << "Constraint Error: Split limit must be a positive integer less than or equal to N." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting minimax binary search on answer range optimization scan..." << endl;
    int optimalLargestSumResult = solver.splitArray(nums, k);

    cout << "Calculated minimized maximum continuous subarray sum value: " << optimalLargestSumResult << endl;

    return 0;
}