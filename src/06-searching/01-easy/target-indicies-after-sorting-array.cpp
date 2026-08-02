#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * LOWER/UPPER BOUND COUNTING ENGINE (OPTIMIZED STATISTICAL O(N) LINEAR SWEEP)
     * -------------------------------------------------------------------------
     * The objective is to return a list of the target indices of `nums` after sorting 
     * `nums` in non-decreasing order. If the target is not present, return an empty vector.
     *
     * Algorithmic & Mathematical Strategy:
     * - A naive approach physically sorts the array using O(N log N) sorting networks 
     *   and then collects the indexes. 
     * - We optimize this to a strict O(N) linear time operation by utilizing counting mechanics.
     *   Instead of moving elements around, we compute where the target *would* land if the array 
     *   were sorted:
     *     1. `lessThanCount`: The count of all elements strictly smaller than `target`. In a 
     *        sorted structure, this number represents the exact **lower bound index** where the 
     *        target values must begin appearing.
     *     2. `targetCount`: The frequency of the `target` value inside the array.
     * - Once these values are known, if `targetCount > 0`, the target indices form a continuous 
     *   consecutive sequence starting exactly at `lessThanCount` and extending for `targetCount` slots:
     *   `[lessThanCount, lessThanCount + 1, ..., lessThanCount + targetCount - 1]`.
     */
    vector<int> targetIndices(vector<int>& nums, int target) {
        int lessThanCount = 0;
        int targetCount = 0;

        // Perform a single pass linear sweep to collect structural statistical counts
        for (int num : nums) {
            if (num < target) {
                lessThanCount++;
            } else if (num == target) {
                targetCount++;
            }
        }

        // Construct the result index array layout using the calculated lower/upper bound metrics
        vector<int> resultIndices;
        resultIndices.reserve(static_cast<size_t>(targetCount));
        
        for (int i = 0; i < targetCount; ++i) {
            resultIndices.push_back(lessThanCount + i);
        }

        return resultIndices;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements inside the array. 
 *                  The algorithm scans the collection exactly once to gather element counts, 
 *                  avoiding full sorting system bottlenecks.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The analysis calculations execute entirely using scalar 
 *                   tracking registers, excluding the memory allocated for the return value array.
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

    int target;
    cout << "Enter the target search value: ";
    if (!(cin >> target)) return 1;

    Solution solver;
    cout << "\nExecuting optimized lower/upper bound index tracking count sweep..." << endl;
    vector<int> result = solver.targetIndices(nums, target);

    cout << "\nTarget indices after virtual sort transformation:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}