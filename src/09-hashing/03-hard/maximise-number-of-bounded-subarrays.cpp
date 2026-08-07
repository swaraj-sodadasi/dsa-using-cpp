#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DE-POLISHED MONOTONIC WINDOWS + BOUNDARY SLIDER INDEX PAIRS
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the total number of contiguous subarrays where the 
     * maximum element falls within the inclusive range bounds [leftBound, rightBound].
     * 
     * 1. THE CONSTRAINT PARTITION LOGIC:
     *    An element at index `i` can be categorized into three distinct tracking states:
     *      - State A (Valid Anchor): `leftBound <= nums[i] <= rightBound`. This element qualifies
     *        the maximum condition for any subarray containing it.
     *      - State B (Below Range): `nums[i] < leftBound`. This element can extend an existing 
     *        valid subarray but cannot qualify a new one on its own.
     *      - State C (Violating Boundary): `nums[i] > rightBound`. This element instantly invalidates
     *        any subarray containing it, resetting our sliding window boundaries.
     * 
     * 2. THE THREE-POINTER INDEX INVARIANT SWEEP:
     *    Instead of executing a costly $O(N^2)$ brute-force check, we traverse the array in a 
     *    single pass using three index anchors:
     *      - `invalidIndex`: Tracks the most recent index position containing a violating element (State C).
     *      - `validIndex`: Tracks the most recent index position containing a valid anchor element (State A).
     * 
     *    As we loop through the array with pointer `i`:
     *      - If `nums[i] > rightBound`, we update `invalidIndex = i`.
     *      - If `nums[i] >= leftBound`, we update `validIndex = i`.
     *      
     *    If `validIndex > invalidIndex`, it means there is a valid anchor available since the last 
     *    violation. The number of valid subarrays ending at the current index `i` is exactly:
     *    `validIndex - invalidIndex`. We add this value to our global running total.
     */
    long long numSubarrayBoundedMax(vector<int>& nums, int leftBound, int rightBound) {
        long long totalBoundedSubarraysCount = 0;
        int n = static_cast<int>(nums.size());
        
        // Invariant anchors tracking state boundaries
        int invalidIndex = -1; 
        int validIndex = -1;   

        // Single-pass linear execution sweep
        for (int i = 0; i < n; ++i) {
            if (nums[static_cast<size_t>(i)] > rightBound) {
                invalidIndex = i;
            }
            if (nums[static_cast<size_t>(i)] >= leftBound) {
                validIndex = i;
            }

            // Accumulate subarrays ending at 'i' if a valid anchor exists in the current window block
            if (validIndex > invalidIndex) {
                totalBoundedSubarraysCount += (validIndex - invalidIndex);
            }
        }

        return totalBoundedSubarraysCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The single pass loop parses each element exactly once, performing index adjustments 
 *                  and conditional updates in true O(1) constant runtime.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The tracking state registers are modified completely in place 
 *                   without allocating auxiliary memory blocks.
 */
int main() {
    int totalElementsCount, leftBound, rightBound;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> totalElementsCount) || totalElementsCount <= 0) {
        cout << "Invalid sizing parameter configured." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(totalElementsCount));
    cout << "Enter the array elements consecutively:\n";
    for (int i = 0; i < totalElementsCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the lower range limit cap (Left Bound): ";
    cin >> leftBound;
    cout << "Enter the upper range limit cap (Right Bound): ";
    cin >> rightBound;

    if (leftBound > rightBound) {
        cout << "Constraint Error: Left bound cannot be greater than Right bound." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting monotonic window boundary adjustments..." << endl;
    long long resultingSubarraysCount = solver.numSubarrayBoundedMax(nums, leftBound, rightBound);

    cout << "\nTotal number of valid bounded subarrays discovered: " << resultingSubarraysCount << endl;

    return 0;
}