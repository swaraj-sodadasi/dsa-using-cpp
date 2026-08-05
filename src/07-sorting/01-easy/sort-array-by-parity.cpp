#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * IN-PLACE INVERSION POINTER SWAP ENGINE
     * --------------------------------------
     * The objective is to rearrange an integer array such that all even integers 
     * appear at the beginning of the array, followed by all odd integers. 
     * The separation must operate entirely in-place to fulfill O(1) auxiliary space constraints.
     *
     * Two-Pointer Dual Inversion Mechanics:
     * - We initialize two pointers: `left = 0` at the start, and `right = N - 1` at the end.
     * - The array is processed via a single-pass partitioning strategy similar to quicksort:
     *   - Move the `left` pointer forward while it points to a valid even number (correctly positioned).
     *   - Move the `right` pointer backward while it points to a valid odd number (correctly positioned).
     *   - If `left < right`, it implies both pointers have stalled on misaligned items: 
     *     `nums[left]` is odd and `nums[right]` is even.
     *   - We swap these two misaligned items. This instantly corrects the state of both positions.
     * - We continue this loop until the pointers cross (`left >= right`), completing the layout inversion.
     */
    vector<int>& sortArrayByParity(vector<int>& nums) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        // Converge pointers inward from both boundary edges
        while (left < right) {
            // Advance left pointer if it points to an even number
            if (nums[static_cast<size_t>(left)] % 2 == 0) {
                left++;
            } 
            // Regression right pointer if it points to an odd number
            else if (nums[static_cast<size_t>(right)] % 2 != 0) {
                right--;
            } 
            // Both pointers have stalled on misaligned parity states; execute swap
            else {
                swap(nums[static_cast<size_t>(left)], nums[static_cast<size_t>(right)]);
                left++;
                right--;
            }
        }

        return nums;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements in the collection array. 
 *                  Each element is inspected at most once by either the left or right pointer, 
 *                  ensuring linear computation execution boundaries.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The elements are shifted completely in-place 
 *                   by swapping array cells, requiring zero additional heap allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter non-negative integer array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Array elements must be non-negative integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting in-place inversion pointer swap separation pass..." << endl;
    solver.sortArrayByParity(nums);

    cout << "Rearranged array layout output:\n[ ";
    for (size_t i = 0; i < nums.size(); ++i) {
        cout << nums[i] << (i + 1 < nums.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}