#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * MONOTONIC STACK SEGMENT ANALYSIS ENGINE
     * ----------------------------------------
     * The objective is to find a contiguous subarray of length 'k' such that every element 
     * in the subarray is strictly greater than (threshold / k). If such a subarray exists, 
     * return its length 'k'; otherwise, return -1.
     *
     * Algorithmic Strategy:
     * - The condition `nums[i] > threshold / k` can be rewritten as `k > threshold / nums[i]`.
     * - To maximize our chances of finding a valid 'k' for any element `nums[i]`, we should find 
     *   the **longest possible contiguous subarray** where `nums[i]` acts as the minimum element.
     * - If the largest valid subarray where `nums[i]` is the absolute minimum has length `maxK`, 
     *   and `nums[i] > threshold / maxK`, then this subarray satisfies the constraint.
     *
     * Boundary Determination via Monotonic Stack:
     * - For each element `i`, we need to find:
     *   1. `left[i]`: The index of the nearest element to the left that is strictly smaller than `nums[i]`.
     *   2. `right[i]`: The index of the nearest element to the right that is smaller than or equal to `nums[i]`.
     * - The maximum length of a subarray where `nums[i]` is the minimum is: `k = right[i] - left[i] - 1`.
     * - We track these boundaries efficiently in a single linear pass using a monotonic increasing stack 
     *   storing element indices.
     */
    int validSubarraySize(vector<int>& nums, int threshold) {
        size_t n = nums.size();
        
        // Structures to map out boundary constraints for each array segment position anchor
        vector<int> leftBoundaries(n, -1);
        vector<int> rightBoundaries(n, static_cast<int>(n));
        
        // Monotonic stack tracking active index indices positions
        stack<int> indexStack;

        // Step 1: Forward sweep to determine the left boundary mapping limits
        for (size_t i = 0; i < n; ++i) {
            // Maintain increasing stack property: pop elements greater or equal to current value
            while (!indexStack.empty() && nums[static_cast<size_t>(indexStack.top())] >= nums[i]) {
                indexStack.pop();
            }
            // If the stack is not empty, the top element is the closest smaller item to the left
            if (!indexStack.empty()) {
                leftBoundaries[i] = indexStack.top();
            }
            indexStack.push(static_cast<int>(i));
        }

        // Clear stack to reuse for right boundary evaluation tracking
        while (!indexStack.empty()) {
            indexStack.pop();
        }

        // Step 2: Backward sweep to determine the right boundary mapping limits
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            // Maintain increasing stack property: pop elements greater or equal to current value
            while (!indexStack.empty() && nums[static_cast<size_t>(indexStack.top())] >= nums[static_cast<size_t>(i)]) {
                indexStack.pop();
            }
            // If the stack is not empty, the top element is the closest smaller item to the right
            if (!indexStack.empty()) {
                rightBoundaries[static_cast<size_t>(i)] = indexStack.top();
            }
            indexStack.push(i);
        }

        // Step 3: Evaluate each maximum segment size against the threshold condition
        for (size_t i = 0; i < n; ++i) {
            int k = rightBoundaries[i] - leftBoundaries[i] - 1;
            
            // Re-map formula to direct multiplication to avoid floating point division overhead issues
            if (nums[i] > threshold / k) {
                return k; // Valid subarray length found
            }
        }

        return -1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the input array.
 *                  Every array element index is pushed onto and popped from the monotonic stack 
 *                  at most once across the independent tracking sweeps, securing strict linear runtime.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The framework allocates boundary maps and stack objects 
 *                   proportional to the size of the array dataset context.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (strictly positive integers) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Elements must be strictly positive integers." << endl;
            return 1;
        }
    }

    int threshold;
    cout << "Enter the threshold division boundary limit value: ";
    if (!(cin >> threshold) || threshold <= 0) {
        cout << "Invalid threshold constraint parameter settings." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting monotonic stack segment analysis sweep..." << endl;
    int result = solver.validSubarraySize(nums, threshold);

    if (result != -1) {
        cout << "Matching valid contiguous subarray size discovered: " << result << endl;
    } else {
        cout << "Result: -1 (No segment sub-sequence matches the parameter thresholds)" << endl;
    }

    return 0;
}