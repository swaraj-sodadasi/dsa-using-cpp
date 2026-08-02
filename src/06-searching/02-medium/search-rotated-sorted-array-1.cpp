#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * PIVOT DISCONTINUITY DETECTION AND BRANCH PRUNING ENGINE
     * --------------------------------------------------------
     * The objective is to search for a target value within an array of unique integers 
     * that has been sorted in ascending order and subsequently rotated at an unknown pivot index. 
     * The solution must achieve O(log N) run-time complexity metrics.
     *
     * Algorithmic & Structural Framework:
     * - A standard sorted array allows uniform binary search. Rotation introduces a single pivot 
     *   discontinuity, dividing the array into two sorted continuous intervals (e.g., [4,5,6,7,0,1,2]).
     * - Key Mathematical Property: For any midpoint index `mid`, dividing the array into two halves 
     *   guarantees that **at least one of the halves is perfectly sorted**. 
     *
     * Branch Pruning Logic Protocol:
     * - Compute the midpoint securely: `mid = left + (right - left) / 2`.
     * - If `nums[mid] == target`, the element is found; return the index immediately.
     * - Case 1: The left boundary segment `[left, mid]` is uniformly sorted (`nums[left] <= nums[mid]`).
     *   - Check if the `target` falls cleanly within this sorted range: `nums[left] <= target && target < nums[mid]`.
     *   - If yes, prune the right half: `right = mid - 1`.
     *   - If no, the target must reside in the opposite half: `left = mid + 1`.
     * - Case 2: The right boundary segment `[mid, right]` is uniformly sorted.
     *   - Check if the `target` falls cleanly within this sorted range: `nums[mid] < target && target <= nums[right]`.
     *   - If yes, prune the left half: `left = mid + 1`.
     *   - If no, the target must reside in the opposite half: `right = mid - 1`.
     * - The search window systematically eliminates half of the non-viable intervals until convergence or resolution.
     */
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] == target) {
                return mid; // Target correctly located
            }

            // Case 1: Evaluate if the left half is monotonically sorted
            if (nums[static_cast<size_t>(left)] <= nums[static_cast<size_t>(mid)]) {
                // Pruning Condition: Check if target sits within the monotonic left interval bounds
                if (nums[static_cast<size_t>(left)] <= target && target < nums[static_cast<size_t>(mid)]) {
                    right = mid - 1; // Focus search window entirely on the left partition
                } else {
                    left = mid + 1;  // Prune the left half, branch to the right partition
                }
            } 
            // Case 2: The right half must be monotonically sorted
            else {
                // Pruning Condition: Check if target sits within the monotonic right interval bounds
                if (nums[static_cast<size_t>(mid)] < target && target <= nums[static_cast<size_t>(right)]) {
                    left = mid + 1;  // Focus search window entirely on the right partition
                } else {
                    right = mid - 1; // Prune the right half, branch to the left partition
                }
            }
        }

        return -1; // Target value is absent from the rotated array collection
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - Where N represents the total elements inside the array. 
 *                  Every iteration executes branch pruning that cuts the active search window size 
 *                  in half, bounding search operational loops to log_2(N) checks.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The pivot discontinuity tracking calculations execute completely 
 *                   in-place using scalar variables, consuming no additional frame buffer space allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the rotated array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter unique elements of the rotated sorted array separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int target;
    cout << "Enter the target search value: ";
    if (!(cin >> target)) return 1;

    Solution solver;
    cout << "\nExecuting pivot discontinuity detection and branch pruning search sweep..." << endl;
    int indexOutcome = solver.search(nums, target);

    if (indexOutcome != -1) {
        cout << "Target element discovered at array location index: " << indexOutcome << endl;
    } else {
        cout << "Result: -1 (Target element does not exist within the given array structure)" << endl;
    }

    return 0;
}