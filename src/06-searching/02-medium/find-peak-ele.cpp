#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * LOCAL EXTREMA LOCALIZED BINARY SEARCH ENGINE
     * ---------------------------------------------
     * The objective is to find a peak element in an array where nums[i] != nums[i+1] 
     * for all valid i, and return its index. A peak element is strictly greater 
     * than its neighbors. The solution must run in O(log N) time.
     *
     * Mathematical & Algorithmic Strategy:
     * - A naive solution scans the array linearly, taking O(N) time.
     * - We optimize this to O(log N) by using binary search, leveraging the property 
     *   that a local peak must exist on any ascending slope. 
     * - We assume the virtual elements at index -1 and index N are negative infinity.
     *
     * Slope Analysis Framework:
     * - For any midpoint index `mid`:
     *   - If `nums[mid] < nums[mid + 1]`, the sequence is currently ascending toward the right. 
     *     Since the right boundary eventually drops down to negative infinity, an apex/peak 
     *     is guaranteed to exist somewhere to the right of `mid`. We shift: `left = mid + 1`.
     *   - If `nums[mid] > nums[mid + 1]`, the sequence is descending toward the right. This means 
     *     `mid` itself could be a peak, or a peak exists somewhere to its left. We contract: `right = mid`.
     * - When `left == right`, the convergence point isolates a valid local peak index.
     */
    int findPeakElement(vector<int>& nums) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        // Perform specialized binary search to locate the localized local extrema
        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] < nums[static_cast<size_t>(mid + 1)]) {
                // Ascending slope: A peak element is guaranteed to reside in the right partition
                left = mid + 1;
            } else {
                // Descending slope: The mid element is a peak candidate, or a peak lies to the left
                right = mid;
            }
        }

        // Convergence point identifies a guaranteed local peak element index
        return left;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - Where N represents the total elements within the array. 
 *                  The localized binary search space halves at each iteration step, 
 *                  bounding performance within log_2(N) checks.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization evaluation operates entirely in place 
 *                   using simple scalar loop variables, requiring zero extra stack space.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces (consecutive elements must be unique):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    // Constraint Verification: Ensure no two adjacent elements are equal
    for (int i = 0; i < n - 1; ++i) {
        if (nums[static_cast<size_t>(i)] == nums[static_cast<size_t>(i + 1)]) {
            cout << "Constraint Error: Adjacent elements cannot be equal." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting local extrema localized binary search sweep..." << endl;
    int peakIndexResult = solver.findPeakElement(nums);

    cout << "Identified peak element location index: " << peakIndexResult << endl;
    cout << "Peak element value discovered: " << nums[static_cast<size_t>(peakIndexResult)] << endl;

    return 0;
}