#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Solution {
public:
    /*
     * IN-PLACE INDEX INVERSION MARKER ASSIGNMENT ENGINE
     * -------------------------------------------------
     * The objective is to find all elements that appear twice in an array of size N,
     * where all integers are in the range [1, N]. Crucially, the solution must run 
     * in O(N) time and use O(1) auxiliary space.
     *
     * Mathematical & Algorithmic Mechanics:
     * - Since the values are bounded between 1 and N, they correspond directly to array indices from 0 to N-1.
     * - We can treat the input array itself as a hash table. As we traverse the array, we look at the absolute 
     *   value of the current element, `val = abs(nums[i])`. This value tells us which index to inspect: `targetIndex = val - 1`.
     * - We check the sign of the element at `targetIndex`:
     *   - If it's positive, this is the first time we've encountered the value `val`. We flag it by multiplying the value 
     *     at `targetIndex` by -1 (inverting its sign to negative).
     *   - If it's already negative, it means we have visited this index before. Therefore, `val` is a duplicate element.
     * - This approach lets us track state changes directly within the original array, avoiding any extra space overhead.
     */
    vector<int> findDuplicates(vector<int>& nums) {
        vector<int> duplicatesResult;
        size_t n = nums.size();

        // Perform a single linear pass across the input array array
        for (size_t i = 0; i < n; ++i) {
            // Obtain the original absolute value to determine the corresponding index map placement
            int val = abs(nums[i]);
            size_t targetIndex = static_cast<size_t>(val - 1);

            // If the element at the target index is negative, it's a duplicate match
            if (nums[targetIndex] < 0) {
                duplicatesResult.push_back(val);
            } else {
                // Otherwise, invert the sign to leave a marker for subsequent passes
                nums[targetIndex] = -nums[targetIndex];
            }
        }

        // Optional Restoration Step: Restore the array back to positive values if needed
        for (size_t i = 0; i < n; ++i) {
            nums[i] = abs(nums[i]);
        }

        return duplicatesResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  The logic completes within a single forward sweep pass (plus an optional 
 *                  restoration pass), performing constant time O(1) mutations per element.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The status flags are managed entirely inside the input array 
 *                   by toggling signs, requiring no extra memory layout footprint outside the 
 *                   required return vector space.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (integers must fall within the range [1, " << n << "]) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 1 || nums[static_cast<size_t>(i)] > n) {
            cout << "Constraint Error: Elements must strictly conform to the range [1, " << n << "]." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting in-place index inversion marker sweep..." << endl;
    vector<int> result = solver.findDuplicates(nums);

    cout << "\nDuplicate elements located within the sequence:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";
    cout << "Total duplicate elements captured: " << result.size() << endl;

    return 0;
}