#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * CUSTOM LOWER BOUND IMPLEMENTATION
     * ---------------------------------
     * Locates the first occurrence index of the target.
     * If the target is not present, it identifies where it would be inserted.
     */
    int findLowerBound(const vector<int>& nums, int target) {
        int left = 0;
        int right = static_cast<int>(nums.size());

        while (left < right) {
            int mid = left + (right - left) / 2;

            // Shift right pointer down if mid is greater or equal, pressing toward the left boundary
            if (nums[static_cast<size_t>(mid)] >= target) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }

    /*
     * CUSTOM UPPER BOUND IMPLEMENTATION
     * ---------------------------------
     * Locates the first position containing an element strictly greater than the target.
     */
    int findUpperBound(const vector<int>& nums, int target) {
        int left = 0;
        int right = static_cast<int>(nums.size());

        while (left < right) {
            int mid = left + (right - left) / 2;

            // Shift left pointer past mid even if it equals target, pressing toward the right boundary
            if (nums[static_cast<size_t>(mid)] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }

public:
    /*
     * TARGET RANGE EXTRACTION STRATEGY
     * --------------------------------
     * The objective is to find the starting and ending index positions of a given target 
     * value inside an array sorted in non-decreasing order. Runtime must be O(log N).
     *
     * Algorithmic Strategy:
     * - We execute two distinct binary search phases to bound the element range.
     * - Step 1: Use `findLowerBound` to locate the earliest possible starting point.
     * - Step 2: Validate the lower bound index result. If it points outside array boundaries 
     *   or the element at that index doesn't match the target, we know the target doesn't exist. 
     *   We return [-1, -1] immediately.
     * - Step 3: Use `findUpperBound` to locate the trailing edge. The upper bound function 
     *   returns the index *after* the last target match, so the true end position is `upperBoundIndex - 1`.
     */
    vector<int> searchRange(vector<int>& nums, int target) {
        int lowerBoundIndex = findLowerBound(nums, target);

        // Boundary Validation: Check if the target exists in the array
        if (lowerBoundIndex == static_cast<int>(nums.size()) || nums[static_cast<size_t>(lowerBoundIndex)] != target) {
            return {-1, -1};
        }

        int upperBoundIndex = findUpperBound(nums, target);
        
        // The upper bound returns the insertion index after target; subtract 1 for the closing index position
        return {lowerBoundIndex, upperBoundIndex - 1};
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - Where N represents the total elements inside the array.
 *                  The system invokes two decoupled binary search functions, each halving the 
 *                  workspace area to execute within at most 2 * log_2(N) index evaluations.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The interval bounding checks run completely in-place 
 *                   using simple integer markers, requiring zero extra dynamic memory allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements in non-decreasing sorted order separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    // Sanity check to ensure input sequence satisfies sorted prerequisite constraints
    if (!is_sorted(nums.begin(), nums.end())) {
        cout << "Constraint Error: Elements must be entered in sorted order." << endl;
        return 1;
    }

    int target;
    cout << "Enter the target search value: ";
    if (!(cin >> target)) return 1;

    Solution solver;
    cout << "\nExecuting custom lower and upper bound binary search ranges..." << endl;
    vector<int> rangeResult = solver.searchRange(nums, target);

    cout << "Target element boundary indices: [ " << rangeResult[0] << ", " << rangeResult[1] << " ]" << endl;

    return 0;
}