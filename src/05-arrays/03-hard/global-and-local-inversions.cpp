#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * MATHEMATICAL BOUND VERIFICATION ENGINE
     * ---------------------------------------
     * The objective is to determine whether the number of global inversions is exactly 
     * equal to the number of local inversions in a permutation array.
     *
     * Mathematical Framework:
     * - A Local Inversion is defined as:   A[i] > A[i + 1]  where 0 <= i < N - 1
     * - A Global Inversion is defined as:  A[i] > A[j]      where 0 <= i < j < N
     * 
     * Algorithmic Strategy:
     * - Since every local inversion is naturally a global inversion, the counts can only 
     *   be equal if there are **zero** non-local global inversions.
     * - A non-local global inversion happens if an element at index `i` is greater than 
     *   an element at index `j` where `j >= i + 2`.
     * - Instead of doing a heavy Merge-Sort inversion sweep taking O(N log N) time, we can 
     *   evaluate this in O(N) linear time. We iterate through the array while maintaining 
     *   a running maximum (`currentMax`) of the prefix sequence.
     * - For each element `nums[i]`, we check it against the maximum element seen up to 
     *   index `i - 2`. If `currentMax > nums[i]`, we have found a non-local global inversion, 
     *   meaning the global and local inversion counts cannot be equal. We return false immediately.
     */
    bool isIdealPermutation(vector<int>& nums) {
        size_t n = nums.size();
        if (n <= 2) return true; // Arrays of size 0, 1, or 2 can never have non-local inversions

        int currentMax = nums[0];

        // Linear tracking pass evaluating elements against the prefix maximum offset by 2 slots
        for (size_t i = 2; i < n; ++i) {
            // Check if a non-local global inversion boundary condition is triggered
            if (currentMax > nums[i]) {
                return false; 
            }
            // Update the running prefix maximum dynamically to include the element at i - 1
            currentMax = max(currentMax, nums[i - 1]);
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the permutation array.
 *                  The mathematical bounding check completes in a single linear pass, executing 
 *                  constant time O(1) comparison mutations per element.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The status flags are managed entirely using scalar tracking 
 *                   variables, requiring zero extra space layouts on the runtime stack.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the permutation array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter a permutation of integers from 0 to " << n - 1 << " separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0 || nums[static_cast<size_t>(i)] >= n) {
            cout << "Constraint Error: Input must strictly match a valid permutation layout." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting mathematical boundary verification sweep..." << endl;
    
    cout << boolalpha;
    bool result = solver.isIdealPermutation(nums);

    cout << "\nIdeal Permutation Verification Result: " << result << endl;
    if (result) {
        cout << "Success: The global inversion count equals the local inversion count." << endl;
    } else {
        cout << "Fail: Non-local global inversions detected within the sequence." << endl;
    }

    return 0;
}