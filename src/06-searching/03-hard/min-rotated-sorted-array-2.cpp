#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * DISCONTINUITY DETECTION UNDER DEGENERATE DUPLICATES ENGINE
     * -----------------------------------------------------------
     * The objective is to find the minimum element within an array of integers 
     * that was originally sorted in ascending order, contains duplicate elements, 
     * and has been rotated at an unknown pivot index. The solution must minimize 
     * average-case operations.
     *
     * Algorithmic & Structural Framework:
     * - In a standard rotated sorted array, we compare `nums[mid]` against `nums[right]` 
     *   to determine which partition half contains the pivot discontinuity (and thus the minimum).
     * - **The Degenerate Challenge:** When `nums[left] == nums[mid] == nums[right]`, we hit an 
     *   informational dead-zone. We cannot determine whether the minimum element lies in the 
     *   left half or the right half (e.g., [2, 2, 2, 0, 2] vs [2, 0, 2, 2, 2]).
     *
     * Degenerate Resolution Strategy (Linear Reduction):
     * - When this exact ambiguity happens, we cannot safely discard half of the array. Instead, 
     *   we safely decrement the upper bound tracker (`right--`). This strips away one duplicate 
     *   element that is guaranteed not to be the *unique* minimum unless it's the only value left.
     * - This step shifts the worst-case runtime complexity to O(N), but preserves correctness 
     *   and allows the search to regain O(log N) mechanics once the degeneracy breaks.
     *
     * Decision Branch Protocol:
     * - Compute the midpoint securely: `mid = left + (right - left) / 2`.
     * - Case 1: `nums[mid] < nums[right]`
     *   - The right partition from `mid` to `right` is strictly sorted in non-decreasing order. 
     *     Therefore, the absolute minimum element cannot be strictly to the right of `mid`. 
     *     It could be `mid` itself or somewhere to the left. We shift: `right = mid`.
     * - Case 2: `nums[mid] > nums[right]`
     *   - The array rotated somewhere between `mid` and `right`, making the right partition 
     *     non-monotonic. The minimum element must reside strictly to the right of `mid`. 
     *     We shift: `left = mid + 1`.
     * - Case 3: `nums[mid] == nums[right]`
     *   - Ambiguity zone. We decrement `right` by 1 to break the degenerate shell loop.
     */
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        // Execute localized binary search with linear degenerate pruning steps
        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] < nums[static_cast<size_t>(right)]) {
                // Right side is sorted; the pivot minimum is at mid or in the left half
                right = mid;
            } else if (nums[static_cast<size_t>(mid)] > nums[static_cast<size_t>(right)]) {
                // Right side is out of order; the pivot minimum must be strictly in the right half
                left = mid + 1;
            } else {
                // Degenerate scenario: collapse upper bound boundary step-by-step
                right--;
            }
        }

        // Convergence point guarantees left == right, capturing the minimum value element
        return nums[static_cast<size_t>(left)];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: Average O(log N), Worst-Case O(N). If the array contains heavy duplicate elements, 
 *                  the continuous boundary compression forces sequential pointer shifting. 
 *                  Otherwise, the algorithm maintains optimal binary division characteristics.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization evaluation runs entirely in-place 
 *                   using local scalar registers, requiring zero dynamic heap allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the rotated array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter elements of the rotated sorted array (duplicates allowed) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting pivot discontinuity detection under degenerate duplicate sweep..." << endl;
    int minimumElementResult = solver.findMin(nums);

    cout << "Identified absolute minimum array element value: " << minimumElementResult << endl;

    return 0;
}