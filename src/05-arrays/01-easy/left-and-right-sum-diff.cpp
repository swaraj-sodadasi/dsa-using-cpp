#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

class Solution {
public:
    /*
     * RUNNING ABSOLUTE PREFIX DIFFERENCE ENGINE
     * -----------------------------------------
     * The objective is to compute a transformation matrix where each element at index 'i' 
     * is equal to the absolute difference between the sum of elements to its left 
     * and the sum of elements to its right: |leftSum[i] - rightSum[i]|.
     *
     * Architectural Tracking Mechanics:
     * - A naive solution recalculates the prefix and suffix slices for every single item, 
     *   yielding a slow O(N^2) time profile.
     * - We optimize this to O(N) linear time with O(1) extra space by leveraging the mathematical 
     *   relationship between total array balance metrics.
     * - The right-side sum for the first element is simply the sum of the entire array 
     *   excluding the first element: `rightSum = totalSum - nums[0]`. The left-side sum is 0.
     *
     * State Array Evaluation Sequence:
     * - Pass 1: Perform a linear reduction sweep using `accumulate` to pre-calculate the 
     *   aggregate total sum value of all array elements (`totalSum`).
     * - Pass 2: Iterate sequentially through the input array. For each index tracker `i`:
     *   - The effective `rightSum` value is computed on the fly by subtracting the active item 
     *     from our running pool: `totalSum -= nums[i]`.
     *   - We calculate the absolute difference between `leftSum` and `rightSum` and append it 
     *     to the result structure.
     *   - We then prepare for the next step by adding the current item to our running left-side 
     *     accumulator pool: `leftSum += nums[i]`.
     */
    vector<int> leftRightDifference(vector<int>& nums) {
        size_t n = nums.size();
        vector<int> differenceResult(n);
        
        long long leftSum = 0;
        long long totalSum = accumulate(nums.begin(), nums.end(), 0LL);

        // Perform a single linear sweep to update state tracking balances
        for (size_t i = 0; i < n; ++i) {
            // Remove the active element from the remaining sum to derive the right sum profile
            totalSum -= nums[i]; 

            // Calculate the absolute difference between the left and right sum bounds
            differenceResult[i] = static_cast<int>(abs(leftSum - totalSum));

            // Shift the active element into the left accumulator pool to prepare for the next iteration
            leftSum += nums[i];
        }

        return differenceResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array. 
 *                  The logic completes within exactly two linear sweeps (one accumulation 
 *                  and one state transformation pass), avoiding nested comparison loops.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation engine requires no extra dynamic tracking 
 *                   frameworks on the heap, keeping a static scalar layout footprint outside 
 *                   of the required return vector space.
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

    Solution solver;
    cout << "\nExecuting running absolute prefix difference transformation..." << endl;
    vector<int> result = solver.leftRightDifference(nums);

    cout << "\nGenerated Transform Matrix Outcome:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}