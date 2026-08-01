#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * SINGLE-PASS INFLECTION POINT COUNTING ENGINE
     * --------------------------------------------
     * The problem requires us to determine if an array was originally sorted in non-decreasing
     * order and then rotated some number of positions (including zero rotations).
     *
     * Mathematical Insufficiency of Simple Sorting:
     * - An array that is sorted and rotated shifts its sequence boundaries, meaning it will 
     *   have at most ONE "inflection point" (a position where an element is strictly greater 
     *   than the next element: nums[i] > nums[i + 1]).
     *
     * Implementation & Structural Mechanics:
     * - We perform a single linear sweep across the array, evaluating every adjacent pair.
     * - Crucially, the array is treated as a circular structure to handle the rotation wrapper step. 
     *   We check the transition from the final element back to the initial anchor element:
     *   `nums[n - 1] > nums[0]`.
     * - If the array is a valid sorted and rotated sequence, the count of these drop-offs (inflection points) 
     *   must be less than or equal to 1. 
     * - If we discover 2 or more inflection points, the array breaks the constraint framework -> Return false.
     */
    bool check(vector<int>& nums) {
        int inflectionPointsCount = 0;
        size_t n = nums.size();

        // Loop through the array treating it as a closed loop
        for (size_t i = 0; i < n; ++i) {
            // Use the modulo operator to cleanly wrap the index from (n-1) back to 0
            if (nums[i] > nums[(i + 1) % n]) {
                inflectionPointsCount++;
            }

            // Optimization Pruning: Terminate early if the count surpasses the legal constraint boundary
            if (inflectionPointsCount > 1) {
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
 *                  The logic completes within a single forward loop, processing each 
 *                  adjacent pair transition exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation requires only a few constant tracking variables,
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
    cout << "\nExecuting single-pass inflection point counting..." << endl;
    
    if (solver.check(nums)) {
        cout << "Result: True! The array is a valid sorted and rotated sequence." << endl;
    } else {
        cout << "Result: False! The array breaks sorted/rotated boundary constraints." << endl;
    }

    return 0;
}