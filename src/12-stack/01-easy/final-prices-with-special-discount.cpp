#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BASIC MONOTONIC NEXT-LESS ELEMENT ARRAY SCAN
     * -------------------------------------------------------------------------------------
     * The objective is to compute the final price of each item after applying a special 
     * discount. The discount for the i-th item is prices[j], where j is the smallest 
     * index greater than i such that prices[j] <= prices[i].
     * 
     * 1. THE MONOTONIC STACK INSIGHT:
     *    A naive nested-loop solution checks all downstream elements for each item, 
     *    resulting in a quadratic O(N^2) time complexity. Instead, we use a monotonic 
     *    increasing stack that stores indices of elements waiting for their discounts.
     * 
     * 2. THE LIQUIDATION MECHANISM:
     *    As we traverse the array from left to right, the current element prices[i] acts 
     *    as a potential discount candidate. We check the top of our stack: as long as 
     *    the stack is not empty and the price at the top index is greater than or equal 
     *    to prices[i], we have found the *first* next-less-or-equal element for that item.
     *    We pop the index from the stack, subtract prices[i] from its value in-place, 
     *    and repeat until the condition fails.
     * 
     * 3. IN-PLACE MUTATION SAFETY:
     *    After verifying and popping all items eligible for a discount from prices[i], 
     *    we push the current index `i` onto the stack. Elements left in the stack at the 
     *    end of the sweep have no next-less element downstream, meaning they receive 
     *    zero discount, leaving their original prices perfectly intact.
     */
    vector<int> finalPrices(vector<int>& prices) {
        stack<int> discountStack;
        int n = static_cast<int>(prices.size());

        for (int i = 0; i < n; ++i) {
            // Liquidate indices in the stack whose prices are >= the current element
            while (!discountStack.empty() && prices[static_cast<size_t>(discountStack.top())] >= prices[static_cast<size_t>(i)]) {
                int targetIdx = discountStack.top();
                discountStack.pop();
                prices[static_cast<size_t>(targetIdx)] -= prices[static_cast<size_t>(i)];
            }
            // Push the active index to wait for its corresponding downstream discount
            discountStack.push(i);
        }

        return prices;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the prices array. 
 *                  Even with the inner while loop, each index is pushed onto the stack 
 *                  exactly once and popped at most once, bounding execution linearly.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The stack contains at most N elements 
 *                   in worst-case scenarios where prices are strictly increasing (e.g., [1, 2, 3, 4]).
 */
int main() {
    cout << "=== Monotonic Stack Next-Less Element Discount Engine ===\n";
    cout << "Enter the total number of shop items: ";
    int itemCount;
    if (!(cin >> itemCount) || itemCount < 0) return 1;

    vector<int> itemPrices(static_cast<size_t>(itemCount));
    if (itemCount > 0) {
        cout << "Enter the raw integer item prices sequentially:\n";
        for (int i = 0; i < itemCount; ++i) {
            cin >> itemPrices[static_cast<size_t>(i)];
        }
    }

    // Display initial state layout
    cout << "\nOriginal Price Matrix: [ ";
    for (int p : itemPrices) cout << p << " ";
    cout << "]\n";

    Solution solver;
    cout << "Scanning monotonic indices and matching discount values..." << endl;
    vector<int> finalOutcome = solver.finalPrices(itemPrices);

    // Display processed output matrix
    cout << "Final Discounted Prices: [ ";
    for (int p : finalOutcome) cout << p << " ";
    cout << "]\n";

    return 0;
}