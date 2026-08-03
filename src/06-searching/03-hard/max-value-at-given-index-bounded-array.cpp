#include <iostream>

using namespace std;

class Solution {
private:
    /*
     * ARITHMETIC PROGRESSION SUMMATION HELPER
     * ---------------------------------------
     * Calculates the minimum sum required for a subarray decreasing by 1 at each step 
     * down to a minimum element floor value of 1.
     * 
     * Mathematical Formula (Arithmetic Series Summation):
     * - The total number of elements in this boundary segment is `length`.
     * - The apex peak value at the start of this segment is `peakVal`.
     * - Case 1: The segment is shorter than or equal to `peakVal` (`length <= peakVal`).
     *   The values decrease down to `peakVal - length + 1`. 
     *   Sum = (Count * (First_Element + Last_Element)) / 2
     *       = (length * (peakVal + peakVal - length + 1)) / 2
     * - Case 2: The segment extends past the decreasing slope (`length > peakVal`).
     *   The values drop down to 1, and the remaining cells are filled entirely with 1s.
     *   Sum = (Sum from 1 to peakVal) + (Count of trailing 1s)
     *       = (peakVal * (peakVal + 1)) / 2 + (length - peakVal)
     */
    long long calculateSegmentSum(long long length, long long peakVal) {
        if (length <= 0) return 0;

        if (peakVal >= length) {
            return (length * (2 * peakVal - length + 1)) / 2;
        } else {
            return (peakVal * (peakVal + 1)) / 2 + (length - peakVal);
        }
    }

    /*
     * MONOTONIC TOTAL SUM VALIDATOR
     * -----------------------------
     * Validates whether a proposed peak value `midTarget` at `index` can fit within the 
     * overall array element sum constraint `maxSum`, assuming all other elements drop 
     * by 1 per index down to a floor value of 1.
     */
    bool isValidPeak(int n, int index, int maxSum, long long midTarget) {
        // Compute lengths of the left and right descending array segments
        long long leftLength = index;
        long long rightLength = n - index - 1;

        // The values immediately adjacent to the peak start at midTarget - 1
        long long leftSum = calculateSegmentSum(leftLength, midTarget - 1);
        long long rightSum = calculateSegmentSum(rightLength, midTarget - 1);

        // Calculate total structural space required
        long long totalRequiredSum = midTarget + leftSum + rightSum;

        return totalRequiredSum <= maxSum;
    }

public:
    /*
     * ARITHMETIC PROGRESSION MATH CALCULATION + BINARY SEARCH ENGINE
     * --------------------------------------------------------------
     * The objective is to construct an array of `n` positive integers where the absolute 
     * difference between adjacent elements is at most 1, the total sum does not exceed `maxSum`, 
     * and the value at the target `index` is maximized.
     *
     * Mathematical & Search Framework:
     * - The value at `index` exhibits monotonic property constraints: if it is possible to 
     *   construct a valid array configuration with a peak value `V` at the target index, 
     *   then any value smaller than `V` is also structurally guaranteed to be valid. If it 
     *   fails at `V`, any value greater than `V` will also fail due to insufficient sum budget.
     * - This observation allows us to binary search the optimal answer range `[1, maxSum]` 
     *   to isolate the exact maximum peak value.
     */
    int maxValue(int n, int index, int maxSum) {
        int left = 1;
        int right = maxSum;
        int optimizedMaximalResult = 1;

        // Execute binary search across the monotonic target peak range continuum
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (isValidPeak(n, index, maxSum, mid)) {
                optimizedMaximalResult = mid; // Register the valid peak candidate
                left = mid + 1;               // Try to expand the search boundary to higher elements
            } else {
                right = mid - 1;              // Sum constraint breached; compress search frame down
            }
        }

        return optimizedMaximalResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log(MaxSum)) - The binary search space spans from 1 to MaxSum. 
 *                  Each step invokes an O(1) mathematical calculation using closed-form 
 *                  arithmetic series evaluations, bypassing any need for loops.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization evaluation runs entirely in-place 
 *                   using local scalar variables, requiring zero extra space partitions.
 */
int main() {
    int n, index, maxSum;
    
    cout << "Enter the total size capacity of the array (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive integer." << endl;
        return 1;
    }

    cout << "Enter the targeted index location configuration: ";
    if (!(cin >> index) || index < 0 || index >= n) {
        cout << "Constraint Error: Target index must fall within range [0, N-1]." << endl;
        return 1;
    }

    cout << "Enter the absolute ceiling total sum constraint threshold (maxSum): ";
    if (!(cin >> maxSum) || maxSum < n) {
        cout << "Constraint Error: maxSum must be at least N (since each element must be >= 1)." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting arithmetic series calculations with binary search boundary updates..." << endl;
    int optimalPeakResult = solver.maxValue(n, index, maxSum);

    cout << "Calculated maximum possible value isolated at index " << index << ": " << optimalPeakResult << endl;

    return 0;
}