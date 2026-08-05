#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * DYNAMIC STATE SORTING OPTIMIZATION PASS ENGINE
     * ----------------------------------------------
     * The objective is to find the length of the longest wiggle subsequence. 
     * A wiggle sequence is one where the differences between successive numbers 
     * strictly alternate between positive and negative numbers.
     *
     * Algorithmic Strategy (Linear State Space Space Reduction):
     * - We track two running metrics through a single element evaluation pass:
     *   1. `up`: The length of the longest wiggle sequence ending with a rising element (nums[i] > nums[i-1]).
     *   2. `down`: The length of the longest wiggle sequence ending with a falling element (nums[i] < nums[i-1]).
     *
     * State Optimization Transitions:
     * - Case A: If `nums[i] > nums[i-1]`, it can extend a sequence that was previously falling. 
     *           Therefore, `up = down + 1`. The `down` state stays the same.
     * - Case B: If `nums[i] < nums[i-1]`, it can extend a sequence that was previously rising. 
     *           Therefore, `down = up + 1`. The `up` state stays the same.
     * - Case C: If `nums[i] == nums[i-1]`, there is no slope variation. Both `up` and `down` state 
     *           counters remain unmodified.
     *
     * - At the end of the pass, the final answer is simply the maximum of `up` and `down`.
     */
    int wiggleMaxLength(vector<int>& nums) {
        size_t n = nums.size();
        if (n < 2) return static_cast<int>(n);

        // Initialize state vectors to track matching subproblem limits
        int up = 1;
        int down = 1;

        // Perform the single optimization pass over the vector sequence elements
        for (size_t i = 1; i < n; ++i) {
            if (nums[i] > nums[i - 1]) {
                up = down + 1;
            } else if (nums[i] < nums[i - 1]) {
                down = up + 1;
            }
        }

        return max(up, down);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements inside the sequence collection. 
 *                  The state evaluation updates variables in a single linear step loop.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The code optimizes allocations using two scalar state 
 *                   registers, executing in place without additional heap vectors.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive integer value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting dynamic state optimization pass..." << endl;
    int maxWiggleLen = solver.wiggleMaxLength(nums);

    cout << "Length of the longest matching wiggle subsequence: " << maxWiggleLen << endl;

    return 0;
}