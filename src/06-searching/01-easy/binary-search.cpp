#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * ITERATIVE INTERVAL HALVING ENGINE
     * ---------------------------------
     * Performs a binary search across a sorted array by continuously updating 
     * search space boundaries using a structured loop construct.
     *
     * Core Design Mechanics:
     * - We initialize two index tracking scalars: `left` at 0 and `right` at N-1.
     * - In each iteration, we compute the midpoint using `left + (right - left) / 2` 
     *   to safely prevent potential integer overflow bugs (`(left + right) / 2`).
     * - We evaluate `nums[mid]` against the `target`:
     *   - If matched, the index position is immediately returned.
     *   - If `nums[mid] < target`, the search window narrows to the right half (`left = mid + 1`).
     *   - If `nums[mid] > target`, the search window narrows to the left half (`right = mid - 1`).
     */
    int searchIterative(const vector<int>& nums, int target) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] == target) {
                return mid; 
            }
            else if (nums[static_cast<size_t>(mid)] < target) {
                left = mid + 1; // Discard the left half of the search interval
            }
            else {
                right = mid - 1; // Discard the right half of the search interval
            }
        }

        return -1; // Target value was not discovered within the array boundaries
    }

    /*
     * RECURSIVE INTERVAL HALVING ENGINE
     * ---------------------------------
     * Mimics the logic of the iterative method by passing state parameters 
     * down through functional call stack layers.
     */
    int searchRecursiveHelper(const vector<int>& nums, int left, int right, int target) {
        // Base case: the interval is exhausted, indicating the target is missing
        if (left > right) {
            return -1;
        }

        int mid = left + (right - left) / 2;

        if (nums[static_cast<size_t>(mid)] == target) {
            return mid;
        }
        else if (nums[static_cast<size_t>(mid)] < target) {
            // Recurse into the right interval space
            return searchRecursiveHelper(nums, mid + 1, right, target);
        }
        else {
            // Recurse into the left interval space
            return searchRecursiveHelper(nums, left, mid - 1, target);
        }
    }

    int searchRecursive(const vector<int>& nums, int target) {
        if (nums.empty()) return -1;
        return searchRecursiveHelper(nums, 0, static_cast<int>(nums.size()) - 1, target);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - For both Iterative and Recursive variants. 
 *                  The total active search window is divided in half at each step, 
 *                  requiring at most log_2(N) index checks.
 *
 * SPACE COMPLEXITY: 
 *   - Iterative: O(1) Auxiliary space. Operates completely in-place using scalar variables.
 *   - Recursive: O(log N) Stack space. Consumes execution stack layers proportional 
 *     to the maximum recursion depth path before arriving at a termination base case.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements in strictly ascending sorted order separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    // Verification check step ensuring array sequence satisfies sorted prerequisites
    if (!is_sorted(nums.begin(), nums.end())) {
        cout << "Constraint Error: Elements must be entered in sorted order." << endl;
        return 1;
    }

    int target;
    cout << "Enter the target search value: ";
    if (!(cin >> target)) return 1;

    Solution solver;
    cout << "\nExecuting interval halving search frameworks..." << endl;
    
    int iterativeResult = solver.searchIterative(nums, target);
    int recursiveResult = solver.searchRecursive(nums, target);

    cout << "Iterative Method Index Search Result: " << iterativeResult << endl;
    cout << "Recursive Method Index Search Result: " << recursiveResult << endl;

    return 0;
}