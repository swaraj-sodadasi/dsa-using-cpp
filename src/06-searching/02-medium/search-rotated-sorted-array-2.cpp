#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * DISCONTINUITY SEARCH HANDLING DUPLICATE ELEMENTS ENGINE
     * ---------------------------------------------------------
     * The objective is to determine if a target value exists within an array of integers 
     * that was originally sorted in ascending order, potentially contains duplicate elements, 
     * and has been rotated at an unknown pivot index.
     *
     * Algorithmic & Structural Framework:
     * - Like the unique element variant, a single rotation splits the array into two halves, 
     *   where at least one half is always guaranteed to be monotonically sorted.
     * - **The Duplicate Challenge:** When `nums[left] == nums[mid] == nums[right]`, we encounter an 
     *   informational dead-zone. We can no longer deduce which side of the pivot is perfectly sorted 
     *   (e.g., [1, 0, 1, 1, 1] vs [1, 1, 1, 0, 1]).
     *
     * Worst-Case Mitigation Strategy (Linear Reduction):
     * - When this ambiguity occurs, we apply a safe boundary compression strategy: we decrement `right` 
     *   and increment `left`. This trims the identical outer boundary shells without losing data, 
     *   dropping the time complexity to O(N) in the absolute worst-case scenario.
     *
     * Branch Pruning Logic Protocol:
     * - Compute the midpoint securely: `mid = left + (right - left) / 2`.
     * - If `nums[mid] == target`, return `true` immediately.
     * - **Ambiguity Check:** If `nums[left] == nums[mid] && nums[mid] == nums[right]`, trigger boundary 
     *   compression: `left++`, `right--`, and continue.
     * - Case 1: The left half `[left, mid]` is monotonically sorted (`nums[left] <= nums[mid]`).
     *   - Prune the search space by checking if target fits within this sorted range: 
     *     `nums[left] <= target && target < nums[mid]`.
     *   - If true, shift `right = mid - 1`. Otherwise, branch right: `left = mid + 1`.
     * - Case 2: The right half `[mid, right]` is monotonically sorted.
     *   - Check if target fits within this sorted range: `nums[mid] < target && target <= nums[right]`.
     *   - If true, shift `left = mid + 1`. Otherwise, branch left: `right = mid - 1`.
     */
    bool search(vector<int>& nums, int target) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] == target) {
                return true; // Target located successfully
            }

            // Handle informational dead-zone caused by duplicate boundaries
            if (nums[static_cast<size_t>(left)] == nums[static_cast<size_t>(mid)] && 
                nums[static_cast<size_t>(mid)] == nums[static_cast<size_t>(right)]) {
                left++;
                right--;
                continue;
            }

            // Case 1: Evaluate if the left partition half is sorted
            if (nums[static_cast<size_t>(left)] <= nums[static_cast<size_t>(mid)]) {
                // Pruning Condition: Target sits inside the monotonic left range
                if (nums[static_cast<size_t>(left)] <= target && target < nums[static_cast<size_t>(mid)]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } 
            // Case 2: The right partition half must be sorted
            else {
                // Pruning Condition: Target sits inside the monotonic right range
                if (nums[static_cast<size_t>(mid)] < target && target <= nums[static_cast<size_t>(right)]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }

        return false; // Target value is absent from the collection range
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: Average O(log N), Worst-Case O(N). When duplicate values fill the bounds, 
 *                  the continuous boundary compression forces a linear scan. Otherwise, the algorithm 
 *                  maintains logarithmic division characteristics.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The pivot discontinuity traversal executes strictly in-place, 
 *                   using simple integer markers without allocation stack overflows.
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

    int target;
    cout << "Enter the target search value: ";
    if (!(cin >> target)) return 1;

    Solution solver;
    cout << "\nExecuting pivot discontinuity search with duplicate reduction handling..." << endl;
    
    cout << boolalpha;
    bool result = solver.search(nums, target);

    cout << "Target existence determination result: " << result << endl;

    return 0;
}