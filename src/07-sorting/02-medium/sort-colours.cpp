#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * DIJKSTRA'S THREE-WAY PARTITIONING / DUTCH NATIONAL FLAG ENGINE
     * --------------------------------------------------------------
     * The objective is to sort an array containing objects colored red, white, or blue 
     * (represented as integers 0, 1, and 2, respectively) in-place, using a single linear 
     * pass and constant auxiliary space.
     *
     * Algorithmic Strategy (Dijkstra's Three-Way Partitioning):
     * - We maintain three pointers to establish four distinct tracking zones:
     *   1. `[0 ... low-1]`: Handled elements strictly matching value 0 (Red).
     *   2. `[low ... mid-1]`: Handled elements strictly matching value 1 (White).
     *   3. `[mid ... high]`: Unprocessed element segment zone window.
     *   4. `[high+1 ... N-1]`: Handled elements strictly matching value 2 (Blue).
     *
     * State Transition Rules:
     * - The `mid` pointer evaluates the active item `nums[mid]`:
     *   - Case 0: `nums[mid] == 0`. The element belongs in the low zone. We swap `nums[low]` 
     *     and `nums[mid]`, then advance both `low++` and `mid++`. (Since the swapped element 
     *     from index `low` was already verified as a 1, it is safe to advance `mid`).
     *   - Case 1: `nums[mid] == 1`. The element is already in the correct relative area. 
     *     We simply advance `mid++`.
     *   - Case 2: `nums[mid] == 2`. The element belongs in the high zone. We swap `nums[mid]` 
     *     and `nums[high]`, then decrement `high--`. We do NOT advance `mid` here, because 
     *     the new element swapped from index `high` is unprocessed and must be evaluated on the 
     *     next loop step.
     */
    void sortColors(vector<int>& nums) {
        int low = 0;
        int mid = 0;
        int high = static_cast<int>(nums.size()) - 1;

        // Process elements until the mid evaluation pointer crosses the upper boundary tracker
        while (mid <= high) {
            if (nums[static_cast<size_t>(mid)] == 0) {
                swap(nums[static_cast<size_t>(low)], nums[static_cast<size_t>(mid)]);
                low++;
                mid++;
            } 
            else if (nums[static_cast<size_t>(mid)] == 1) {
                mid++;
            } 
            else { // nums[mid] == 2
                swap(nums[static_cast<size_t>(mid)], nums[static_cast<size_t>(high)]);
                high--;
            }
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements in the nums array. 
 *                  The array is sorted in a single linear pass; the `mid` pointer advances 
 *                  or the `high` pointer contracts during every single step.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - All partitioning swaps run completely in-place 
 *                   using simple integer boundary index pointers, consuming no heap blocks.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the color matrix array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive integer value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array items (only values 0 for Red, 1 for White, or 2 for Blue) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0 || nums[static_cast<size_t>(i)] > 2) {
            cout << "Constraint Error: Input values must belong strictly to set {0, 1, 2}." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting Dijkstra's three-way Dutch National Flag pointer partitioning..." << endl;
    solver.sortColors(nums);

    cout << "Partition sorted output sequence layout:\n[ ";
    for (size_t i = 0; i < nums.size(); ++i) {
        cout << nums[i] << (i + 1 < nums.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}