#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * SINGLE-PASS MULTI-STATE FLAG EVALUATION ENGINE
     * ----------------------------------------------
     * An array is monotonic if it is either entirely non-increasing or entirely non-decreasing.
     *
     * Architectural Tracking Mechanics:
     * - A naive approach might perform two independent linear checks (one for increasing, one for decreasing),
     *   which increases runtime code path overhead.
     * - We optimize this by evaluating both conditional states simultaneously using two simple boolean tracking flags:
     *   `isIncreasing` and `isDecreasing`, both initialized to true.
     *
     * State Window Evaluation Sequence:
     * - We perform a single linear sweep across the array, evaluating adjacent pairs starting from index 1.
     * - For each adjacent pair transition:
     *   - If `nums[i] < nums[i - 1]`, the array cannot be non-decreasing, so we drop the `isIncreasing` flag to false.
     *   - If `nums[i] > nums[i - 1]`, the array cannot be non-increasing, so we drop the `isDecreasing` flag to false.
     * - Pruning: If both tracking flags are flipped to false at any point during the sweep, the array breaks the 
     *   monotonic constraints early, and we can terminate execution immediately -> Return false.
     * - If the loop completes and at least one tracking flag remains true, the array is confirmed to be monotonic.
     */
    bool isMonotonic(vector<int>& nums) {
        bool isIncreasing = true;
        bool isDecreasing = true;
        size_t n = nums.size();

        // Perform a single pass checking adjacent element relationships
        for (size_t i = 1; i < n; ++i) {
            if (nums[i] < nums[i - 1]) {
                isIncreasing = false;
            }
            if (nums[i] > nums[i - 1]) {
                isDecreasing = false;
            }

            // EARLY PRUNING: Terminate early if the sequence violates both directional properties
            if (!isIncreasing && !isDecreasing) {
                return false;
            }
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  The logic completes within a single forward sweep, verifying each 
 *                  adjacent pair relationship exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation requires only a few constant scalar tracking variables,
 *                   consuming zero extra space overhead on the runtime heap stack.
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
    cout << "\nExecuting single-pass multi-state flag evaluation..." << endl;
    
    if (solver.isMonotonic(nums)) {
        cout << "Result: True! The array is a valid monotonic sequence." << endl;
    } else {
        cout << "Result: False! The array breaks monotonic trend direction constraints." << endl;
    }

    return 0;
}