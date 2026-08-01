#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Solution {
public:
    /*
     * IN-PLACE ELEMENT NEGATION INDEX MAPPING ENGINE
     * ----------------------------------------------
     * The problem requires us to find all integers in the range [1, n] that do not 
     * appear in an array of size n. Crucially, we must do this in O(N) time and without 
     * allocating extra O(N) auxiliary space.
     *
     * State Transformation Mechanic:
     * - Since all numbers in the array fall within the range [1, n], each value can be 
     *   directly mapped to a unique index position between 0 and n - 1: `targetIndex = abs(nums[i]) - 1`.
     * - We can use the array itself as a hash table by treating the sign of the value at 
     *   `targetIndex` as a boolean flag. 
     * - As we sweep through the array, for each value we encounter, we look up its mapped 
     *   `targetIndex` and multiply the element stored there by -1 to flip its state to negative. 
     *   This explicitly marks that the number `abs(nums[i])` has been visited.
     * - If a number already has a negative sign, it means we have seen that value before. 
     *   We leave it negative to handle duplicate elements smoothly.
     *
     * Result Compilation Sweep:
     * - We run a second linear sweep through the mutated array. Any index position `i` that 
     *   still holds a strictly positive value indicates that the number `i + 1` never appeared 
     *   in the original dataset. We save these missing numbers to our results list.
     */
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        vector<int> missingNumbersResult;
        size_t n = nums.size();

        // PASS 1: Mark present numbers by negating the values at their corresponding index positions
        for (size_t i = 0; i < n; ++i) {
            // Use absolute value because the element at the current index might have already been negated
            int targetIndex = abs(nums[i]) - 1;
            
            if (nums[static_cast<size_t>(targetIndex)] > 0) {
                nums[static_cast<size_t>(targetIndex)] *= -1;
            }
        }

        // PASS 2: Collect all indices that remain positive, mapping them back to missing values
        for (size_t i = 0; i < n; ++i) {
            if (nums[i] > 0) {
                missingNumbersResult.push_back(static_cast<int>(i + 1));
            }
        }

        return missingNumbersResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  The logic performs exactly two sequential linear sweeps over the input data,
 *                  avoiding costly nested comparison loops.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The input array is mutated in place to track state flags, 
 *                   ensuring no additional heap memory is consumed outside of the final results vector.
 */
int main() {
    int n;
    cout << "Enter the size of the array (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements where each element is between 1 and " << n << " (separated by spaces):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 1 || nums[static_cast<size_t>(i)] > n) {
            cout << "Constraint Error: Elements must fall within the range [1, " << n << "]." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting in-place element negation index mapping..." << endl;
    vector<int> disappearedNumbers = solver.findDisappearedNumbers(nums);

    cout << "\nNumbers that disappeared from the sequence:\n[ ";
    for (size_t i = 0; i < disappearedNumbers.size(); ++i) {
        cout << disappearedNumbers[i] << (i + 1 < disappearedNumbers.size() ? ", " : "");
    }
    cout << " ]\n";
    cout << "Total disappeared items captured: " << disappearedNumbers.size() << endl;

    return 0;
}