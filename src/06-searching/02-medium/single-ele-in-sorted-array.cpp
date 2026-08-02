#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * PARITY INDEX HALVING STRATEGY ENGINE
     * ------------------------------------
     * The objective is to find the single unique element in a sorted array where 
     * every other element appears exactly twice. The solution must run in O(log N) 
     * time and consume O(1) auxiliary space constraints.
     *
     * Mathematical & Structural Framework:
     * - Let the unique element reside at index `P`.
     * - Phase A (Before the Unique Element): Elements appear in pairs starting at an even 
     *   index and ending at an odd index. Thus, for any valid even index `i < P`, 
     *   `nums[i] == nums[i + 1]`.
     * - Phase B (After the Unique Element): The unique element shifts the alignment of 
     *   all subsequent pairs. Elements now appear in pairs starting at an odd index and 
     *   ending at an even index. Thus, for any valid even index `i > P`, `nums[i] != nums[i + 1]`.
     *
     * Algorithmic Strategy (Parity Partitioning Check):
     * - We can leverage binary search to identify the exact point where this structural 
     *   alignment flips.
     * - We compute the midpoint `mid`. To simplify the parity check, if `mid` is odd, we 
     *   decrement it by 1 (`mid--` or `mid ^= 1`). This ensures `mid` always points to an **even index**.
     * - We then evaluate the pair condition `nums[mid] == nums[mid + 1]`:
     *   - If true, we are still in Phase A (before the unique element). The single element must 
     *     exist strictly to the right of this pair block. We shift: `left = mid + 2`.
     *   - If false, we have entered Phase B (after or at the unique element). The single element 
     *     resides either at `mid` or somewhere to its left. We contract: `right = mid`.
     * - The pointers converge precisely on the single unique element index slot.
     */
    int singleNonDuplicate(vector<int>& nums) {
        int left = 0;
        int right = static_cast<int>(nums.size()) - 1;

        // Perform parity index halving binary search loops
        while (left < right) {
            int mid = left + (right - left) / 2;

            // Force mid to be an even index to uniformly evaluate the entry pair sequence
            if (mid % 2 == 1) {
                mid--;
            }

            // Inspect the parity alignment relationship against the neighboring element
            if (nums[static_cast<size_t>(mid)] == nums[static_cast<size_t>(mid + 1)]) {
                // Monotonic pair matching rules hold; unique element exists in the upper half partition
                left = mid + 2;
            } else {
                // Misalignment detected; unique element is at mid or inside the lower half partition
                right = mid;
            }
        }

        // Convergence point guarantees left == right, isolating the unique singleton element value
        return nums[static_cast<size_t>(left)];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - Where N represents the total items inside the sorted array.
 *                  The parity evaluation window splits the search area in half during 
 *                  each validation phase, running within at most log_2(N) index updates.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The analysis calculations execute entirely in-place 
 *                   using local scalar registers, requiring zero dynamic memory allocation.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array (must be an odd integer): ";
    if (!(cin >> n) || n <= 0 || n % 2 == 0) {
        cout << "Invalid parameter. Array size must be a positive odd integer." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements in sorted order (every element must repeat twice except one):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    // Constraint Verification: Ensure array sequence satisfies sorted requirements
    if (!is_sorted(nums.begin(), nums.end())) {
        cout << "Constraint Error: Elements must be entered in sorted order." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting parity index halving search sweep..." << endl;
    int singleElementResult = solver.singleNonDuplicate(nums);

    cout << "Identified unique single element value: " << singleElementResult << endl;

    return 0;
}