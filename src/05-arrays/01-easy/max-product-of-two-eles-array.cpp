#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * TWO-PASS MAX EXTRACTION ENGINE
     * ------------------------------
     * The problem asks us to pick two distinct indices 'i' and 'j' in an array 
     * such that the product (nums[i] - 1) * (nums[j] - 1) is maximized.
     * Since all integers in the array are positive, maximizing this product is 
     * mathematically equivalent to finding the two largest numbers in the array.
     *
     * Implementation Framework:
     * - Instead of an expensive O(N log N) sorting step, we can isolate the two 
     *   largest values using two independent constant-space tracking scans.
     * - Pass 1: Scan the array to locate the absolute maximum element (`max1`) 
     *   and cache its specific index position (`max1Index`).
     * - Pass 2: Scan the array a second time to isolate the second largest element 
     *   (`max2`), explicitly skipping the item at `max1Index` to ensure distinct 
     *   array elements are selected.
     */
    int maxProduct(vector<int>& nums) {
        int max1 = 0;
        size_t max1Index = 0;

        // PASS 1: Sweep the array to discover the largest integer value
        for (size_t i = 0; i < nums.size(); ++i) {
            if (nums[i] > max1) {
                max1 = nums[i];
                max1Index = i;
            }
        }

        int max2 = 0;

        // PASS 2: Sweep the array again to find the second largest integer, skipping max1Index
        for (size_t i = 0; i < nums.size(); ++i) {
            if (i == max1Index) {
                continue; // Guarantee distinct index evaluation
            }
            if (nums[i] > max2) {
                max2 = nums[i];
            }
        }

        // Return the maximized product after applying standard index offset constraints
        return (max1 - 1) * (max2 - 1);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array. 
 *                  The logic performs exactly two sequential linear sweeps over the input data, 
 *                  running much faster than sorting alternatives.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation engine requires no extra dynamic heap allocations, 
 *                   maintaining a constant execution footprint throughout the calculation.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n < 2) {
        cout << "Invalid parameter. The array must contain at least 2 elements." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter positive array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Elements must be positive integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting linear two-pass max extraction..." << endl;
    int result = solver.maxProduct(nums);

    cout << "Maximum derived component product value outcome: " << result << endl;

    return 0;
}