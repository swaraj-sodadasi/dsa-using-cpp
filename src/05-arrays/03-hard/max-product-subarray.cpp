#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * DYNAMIC MIN-MAX PRODUCT BOUNDS STATE TRACKING ENGINE
     * ----------------------------------------------------
     * The objective is to find a contiguous subarray within a 1D integer array 
     * that has the largest product, and return that product.
     *
     * Mathematical & Algorithmic Mechanics:
     * - A naive approach calculates the product of every possible subarray, taking O(N^2) time.
     * - We optimize this to a single linear pass O(N) by expanding Kadane's algorithm to track 
     *   both the maximum and minimum products ending at the current position.
     * - Why track the minimum? Multiplying two negative numbers produces a positive number. 
     *   A highly negative running product can instantly flip into a highly positive product 
     *   if it encounters another negative number.
     *
     * State Modification Scenarios:
     * - When we encounter a negative number `nums[i] < 0`, the maximum possible product 
     *   and the minimum possible product swap roles. Multiplying `currentMax` by a negative 
     *   makes it tiny, while multiplying `currentMin` by a negative might make it huge.
     * - Therefore, if `nums[i]` is negative, we swap `currentMax` and `currentMin` before 
     *   updating our states.
     * - At each position `i`, our choices for the new `currentMax` and `currentMin` are:
     *   1. Extend the existing product chain: `currentMax * nums[i]` / `currentMin * nums[i]`.
     *   2. Discard the past chain and start a fresh subarray right here: `nums[i]`.
     */
    int maxProduct(vector<int>& nums) {
        if (nums.empty()) return 0;

        // Seed initial tracking states with the first element anchor
        int currentMax = nums[0];
        int currentMin = nums[0];
        int globalMax = nums[0];
        size_t n = nums.size();

        // Perform a single pass linear sweep starting from index position 1
        for (size_t i = 1; i < n; ++i) {
            int num = nums[i];

            // If the element is negative, its multiplication swaps the maximum and minimum bounds
            if (num < 0) {
                swap(currentMax, currentMin);
            }

            // Decision Step: either extend the running product chain or start a new partition from here
            currentMax = max(num, currentMax * num);
            currentMin = min(num, currentMin * num);

            // Sync running max state changes into the global historical tracker
            globalMax = max(globalMax, currentMax);
        }

        return globalMax;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  The logic checks each array element exactly once in a single forward pass, 
 *                  updating local scalar values in constant O(1) time per iteration.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization evaluation operates entirely in place, 
 *                   requiring zero extra space layout properties on the runtime stack.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (integers, can be positive, negative, or zero) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting dynamic min-max product bounds state tracking pass..." << endl;
    int result = solver.maxProduct(nums);

    cout << "Maximum contiguous subarray product value discovered: " << result << endl;

    return 0;
}