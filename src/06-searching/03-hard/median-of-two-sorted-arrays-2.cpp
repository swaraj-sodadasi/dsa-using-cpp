#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

class Solution {
public:
    /*
     * SIMULTANEOUS ARRAY DIVISION OPTIMIZATION ENGINE
     * -----------------------------------------------
     * The objective is to find the median of two sorted arrays, `nums1` and `nums2`, 
     * in O(log(min(M, N))) time, which is strictly faster than merging them.
     *
     * Mathematical & Algorithmic Strategy:
     * - If we partition both arrays such that the total number of elements on the left 
     *   side equals the total number of elements on the right side (or left has one extra 
     *   if the total length is odd), the median can be derived directly from the boundary 
     *   values of these partitions.
     * - We only need to binary search on the *smaller* array to find the correct partition 
     *   index `partition1`. The partition index for the second array is deterministically 
     *   calculated as: `partition2 = (totalLength + 1) / 2 - partition1`.
     *
     * Cross-Boundary Evaluation Protocol:
     * - Let `maxLeft1` and `minRight1` be the boundary elements of `nums1` at `partition1`.
     * - Let `maxLeft2` and `minRight2` be the boundary elements of `nums2` at `partition2`.
     * - A valid median partition is found when the cross-boundaries are valid:
     *   1. `maxLeft1 <= minRight2`
     *   2. `maxLeft2 <= minRight1`
     * - If `maxLeft1 > minRight2`, our partition in `nums1` is too far right. We contract 
     *   the upper bound: `right = partition1 - 1`.
     * - If `maxLeft2 > minRight1`, our partition in `nums1` is too far left. We contract 
     *   the lower bound: `left = partition1 + 1`.
     * - We use virtual +/- infinity (INT_MIN/INT_MAX) for boundary edge cases where a 
     *   partition falls at the extreme start or end of an array.
     */
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // Optimization: Ensure nums1 is always the smaller array to minimize the binary search space
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }

        int m = static_cast<int>(nums1.size());
        int n = static_cast<int>(nums2.size());
        
        int left = 0;
        int right = m;

        // Perform simultaneous array division binary search
        while (left <= right) {
            int partition1 = left + (right - left) / 2;
            int partition2 = (m + n + 1) / 2 - partition1;

            // Resolve boundary edge cases using scalar infinity bounds
            int maxLeft1 = (partition1 == 0) ? INT_MIN : nums1[static_cast<size_t>(partition1 - 1)];
            int minRight1 = (partition1 == m) ? INT_MAX : nums1[static_cast<size_t>(partition1)];

            int maxLeft2 = (partition2 == 0) ? INT_MIN : nums2[static_cast<size_t>(partition2 - 1)];
            int minRight2 = (partition2 == n) ? INT_MAX : nums2[static_cast<size_t>(partition2)];

            // Check if we have discovered the perfect cross-boundary split
            if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
                // If total combined length is even, median is the average of the inner boundaries
                if ((m + n) % 2 == 0) {
                    return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0;
                } 
                // If total combined length is odd, median is the largest element on the left side
                else {
                    return max(maxLeft1, maxLeft2);
                }
            } 
            else if (maxLeft1 > minRight2) {
                // The left partition of nums1 is too large; compress the search window to the left
                right = partition1 - 1;
            } 
            else {
                // The left partition of nums1 is too small; shift the search window to the right
                left = partition1 + 1;
            }
        }

        return 0.0; // Fallback return (should not be reached given valid sorted inputs)
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log(min(M, N))) - Where M and N represent the lengths of the two arrays. 
 *                  The binary search strictly operates on the smaller of the two arrays, halving 
 *                  the search space at each interval without merging data.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The cross-boundary partition tracking runs entirely in-place 
 *                   using simple integer boundary trackers, requiring zero dynamic memory allocation.
 */
int main() {
    int m, n;
    
    cout << "Enter the total number of items in the first array (nums1): ";
    if (!(cin >> m) || m < 0) {
        cout << "Invalid parameter. Array size cannot be negative." << endl;
        return 1;
    }
    vector<int> nums1(static_cast<size_t>(m));
    if (m > 0) {
        cout << "Enter elements for nums1 in sorted order separated by spaces:\n";
        for (int i = 0; i < m; ++i) {
            cin >> nums1[static_cast<size_t>(i)];
        }
        if (!is_sorted(nums1.begin(), nums1.end())) {
            cout << "Constraint Error: Elements must be entered in sorted order." << endl;
            return 1;
        }
    }

    cout << "Enter the total number of items in the second array (nums2): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid parameter. Array size cannot be negative." << endl;
        return 1;
    }
    vector<int> nums2(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter elements for nums2 in sorted order separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> nums2[static_cast<size_t>(i)];
        }
        if (!is_sorted(nums2.begin(), nums2.end())) {
            cout << "Constraint Error: Elements must be entered in sorted order." << endl;
            return 1;
        }
    }

    if (m == 0 && n == 0) {
        cout << "Constraint Error: Both arrays cannot be empty simultaneously." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting simultaneous matrix/array division optimization sweep..." << endl;
    double medianResult = solver.findMedianSortedArrays(nums1, nums2);

    cout << fixed << setprecision(5);
    cout << "Calculated precise median of combined sorted sequences: " << medianResult << endl;

    return 0;
}