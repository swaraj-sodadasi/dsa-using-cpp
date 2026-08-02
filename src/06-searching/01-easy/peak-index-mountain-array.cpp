#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * MONOTONIC DERIVATIVE ZERO-CROSSING APPROXIMATION ENGINE
     * -------------------------------------------------------
     * The objective is to find the peak index in a "mountain array" (an array that is guaranteed 
     * to strictly increase up to an apex index, and then strictly decrease).
     *
     * Mathematical & Algorithmic Mechanics:
     * - We can model the structure by looking at the "derivative" (slope) between adjacent elements: 
     *   `slope[i] = nums[i + 1] - nums[i]`.
     * - The slope values form a monotonic property: it is strictly positive (`> 0`) on the ascending side, 
     *   and strictly negative (`< 0`) on the descending side. The peak is the point where this slope 
     *   "crosses zero."
     * - This allows us to map the problem into a binary decision framework:
     *   - If `nums[mid] < nums[mid + 1]`, the slope is rising. The peak must lie strictly to the 
     *     right of `mid`. We shift our boundary: `left = mid + 1`.
     *   - If `nums[mid] > nums[mid + 1]`, the slope is falling. The peak could be `mid` itself or 
     *     somewhere to its left. We contract our boundary: `right = mid`.
     *
     * Boundary Convergence Protocol:
     * - We initialize `left = 0` and `right = nums.size() - 1`.
     * - The loop terminates when `left == right`. At this convergence index, the zero-crossing 
     *   derivative property is fulfilled, pinpointing the peak element location.
     */
    int peakIndexInMountainArray(vector<int>& nums) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        // Execute binary search to isolate the slope inversion boundary
        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[static_cast<size_t>(mid)] < nums[static_cast<size_t>(mid + 1)]) {
                // Ascending slope zone: the peak lies strictly past the current mid element
                left = mid + 1;
            } else {
                // Descending slope zone: mid is a candidate peak, or the peak lies to the left
                right = mid;
            }
        }

        // Convergence point guarantees left == right, capturing the apex index
        return left;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - Where N represents the total elements in the mountain array. 
 *                  The binary search range halves at each evaluation level, bounding 
 *                  the execution within log_2(N) index checks.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization evaluation operates entirely in place 
 *                   using scalar index variables, requiring zero extra space on the heap.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the mountain array (must be >= 3): ";
    if (!(cin >> n) || n < 3) {
        cout << "Invalid parameter. Mountain arrays require at least 3 elements." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the mountain array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting monotonic derivative zero-crossing binary search sweep..." << endl;
    int peakIndexResult = solver.peakIndexInMountainArray(nums);

    cout << "Identified peak apex location index: " << peakIndexResult << endl;
    cout << "Apex maximum element value: " << nums[static_cast<size_t>(peakIndexResult)] << endl;

    return 0;
}