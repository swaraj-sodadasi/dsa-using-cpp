#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CIRCULAR MONOTONIC STACK VIA MODULO ARRAY DOUBLING
     * -------------------------------------------------------------------------------------
     * The objective is to find the next greater number for every element in a circular array.
     * 
     * 1. THE VIRTUAL ARRAY DOUBLING HYPOTHESIS:
     *    To handle the circular property without copying the data into a new double-sized array,
     *    we simulate traversal over a virtual array of size 2 * N. Indices are mapped back
     *    to the original array using the modulo operator: `i % n`.
     * 
     * 2. MONOTONIC STACK COORDINATION PIPELINE:
     *    We traverse this virtual array in reverse order (from 2 * N - 1 down to 0). We maintain
     *    a monotonic stack that tracks indices of elements that are potential candidates for
     *    the next greater element.
     * 
     * 3. THE LIQUIDATION & RESOLUTION LAYER:
     *    For each index `i`:
     *      - We convert it to the circular index: `circularIdx = i % n`.
     *      - We pop elements from the stack whose values are less than or equal to `nums[circularIdx]`.
     *      - During the first half of the virtual loop (`i < n`), the stack's top element contains
     *        the index of the next greater number. If empty, the value is -1.
     *      - We push `circularIdx` onto the stack and continue.
     */
    vector<int> nextGreaterElements(const vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        vector<int> result(static_cast<size_t>(n), -1);
        stack<int> indicesStack;

        // Run reverse pass over a virtual array of size 2 * n
        for (int i = 2 * n - 1; i >= 0; --i) {
            int circularIdx = i % n;

            // Clear indices from the stack whose values are <= the current number
            while (!indicesStack.empty() && nums[static_cast<size_t>(indicesStack.top())] <= nums[static_cast<size_t>(circularIdx)]) {
                indicesStack.pop();
            }

            // Record results only during the actual array index range pass phase
            if (i < n) {
                result[static_cast<size_t>(circularIdx)] = indicesStack.empty() ? -1 : nums[static_cast<size_t>(indicesStack.top())];
            }

            indicesStack.push(circularIdx);
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the input array.
 *                  The system evaluates a virtual loop boundary of 2 * N items, where each index
 *                  is pushed onto the stack at most twice and popped at most twice.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal stack stores at most N indices concurrently.
 */
int main() {
    cout << "=== Circular Monotonic Stack Next Greater Element Engine ===\n";
    cout << "Enter the total number of elements in the circular array: ";
    int elementsCount;
    if (!(cin >> elementsCount) || elementsCount < 0) return 1;

    vector<int> nums(static_cast<size_t>(elementsCount));
    if (elementsCount > 0) {
        cout << "Enter the integer elements sequentially:\n";
        for (int i = 0; i < elementsCount; ++i) {
            cin >> nums[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nSimulating circular array doubling loops and running monotonic reductions..." << endl;
    vector<int> circularOutcome = solver.nextGreaterElements(nums);

    cout << "Circular Next Greater Elements Result: [ ";
    for (int val : circularOutcome) cout << val << " ";
    cout << "]\n";

    return 0;
}