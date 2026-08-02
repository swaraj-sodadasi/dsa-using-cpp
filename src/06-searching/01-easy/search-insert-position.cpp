#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * LOWER BOUND LOCATION MECHANICS ENGINE
     * -------------------------------------
     * The objective is to return the index of a target value if found within a sorted 
     * array of distinct integers. If not found, return the index where it would be 
     * inserted to maintain sorted order.
     *
     * Algorithmic & Mathematical Strategy:
     * - This problem maps exactly to finding the "Lower Bound" (the first element in the 
     *   range that does not compare less than the target: `element >= target`).
     * - We maintain a search window using two scalar indexes: `left = 0` and `right = nums.size()`.
     *   Using `nums.size()` as the initial right boundary accounts for the case where the 
     *   target is larger than all elements and must be appended to the end.
     *
     * Boundary Convergence Protocol:
     * - Compute the midpoint securely to avoid integer overflow: `mid = left + (right - left) / 2`.
     * - Evaluation Steps:
     *   - If `nums[mid] >= target`, the current index is a viable insertion slot, or the target 
     *     lies further to the left. We constrict the search space by shifting the right boundary 
     *     directly to the midpoint: `right = mid`.
     *   - If `nums[mid] < target`, the target must reside strictly past the current index position. 
     *     We advance the left boundary beyond the midpoint: `left = mid + 1`.
     * - Loop Termination: The loop exits when `left == right`. At this exact convergence index, 
     *   `left` points to the correct position (the lower bound slot location).
     */
    int searchInsert(vector<int>& nums, int target) {
        int left = 0;
        int right = static_cast<int>(nums.size());

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] >= target) {
                // The current value meets or exceeds the target; search left to find the exact lower bound
                right = mid;
            } else {
                // The current value is too small; the required insertion slot must lie strictly to the right
                left = mid + 1;
            }
        }

        // At convergence, left and right match to specify the lower bound location index
        return left;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - Where N represents the total elements inside the array. 
 *                  The binary search range halves at each iteration level, bounding 
 *                  the execution within log_2(N) comparison transformations.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The lower bound tracking operates entirely in-place 
 *                   using scalar index variables, requiring zero extra dynamic memory.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter distinct array elements in ascending sorted order separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    // Verification check step ensuring input array sequence satisfies sorting rules
    if (!is_sorted(nums.begin(), nums.end())) {
        cout << "Constraint Error: Elements must be entered in sorted order." << endl;
        return 1;
    }

    int target;
    cout << "Enter the target value to evaluate: ";
    if (!(cin >> target)) return 1;

    Solution solver;
    cout << "\nExecuting lower bound location binary search sweep..." << endl;
    int targetInsertIndex = solver.searchInsert(nums, target);

    cout << "Calculated search insert position target index slot: " << targetInsertIndex << endl;

    return 0;
}