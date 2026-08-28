#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MONOTONIC STACK FOR NEXT SMALLER ELEMENT (FORWARD INSPECTION)
     * -------------------------------------------------------------------------------------
     * The objective is to apply a special discount to items in a shop: the discount on item
     * `i` is the price of the first item `j` (where `j > i`) that has a price less than or 
     * equal to `prices[i]` (`prices[j] <= prices[i]`).
     * 
     * 1. THE MONOTONIC SEQUENCE:
     *    We maintain a monotonic increasing stack of *indices*. This stack holds indices of
     *    prices whose discount has not yet been resolved.
     * 
     * 2. DISPATCHING DISCOUNTS:
     *    We iterate through the array from left to right. For each price `prices[i]`:
     *      - While the stack is not empty and the current price is less than or equal to
     *        the price at the index on top of the stack (`prices[i] <= prices[stack.top()]`):
     *          - We have found the discount for the item at `stack.top()`.
     *          - We subtract `prices[i]` from the price of the item at that popped index.
     *          - Pop the index from the stack.
     *      - Push the current index `i` onto the stack.
     * 
     * 3. INVARIANT:
     *    Indices remaining in the stack at the end of the loop have no valid discount, 
     *    meaning they pay the full price (already set in our output array).
     */
    vector<int> finalPrices(const vector<int>& prices) {
        vector<int> result = prices; // Initialize result with original prices
        stack<int> discountStack;
        int n = static_cast<int>(prices.size());

        for (int i = 0; i < n; ++i) {
            // Apply the discount to all elements in the stack that are >= the current price
            while (!discountStack.empty() && prices[static_cast<size_t>(i)] <= prices[static_cast<size_t>(discountStack.top())]) {
                int targetIdx = discountStack.top();
                discountStack.pop();
                result[static_cast<size_t>(targetIdx)] -= prices[static_cast<size_t>(i)];
            }
            discountStack.push(i);
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items.
 *                  Each index is pushed onto the stack exactly once and popped at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The stack holds at most N elements in a strictly 
 *                   increasing layout (e.g., [1, 2, 3, 4]).
 */
int main() {
    cout << "=== Final Prices with Special Discount Optimizer ===\n";
    cout << "Enter the total number of items: ";
    int count;
    if (!(cin >> count) || count < 0) return 1;

    vector<int> prices(static_cast<size_t>(count));
    if (count > 0) {
        cout << "Enter the item prices sequentially:\n";
        for (int i = 0; i < count; ++i) {
            cin >> prices[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nScanning downstream sequences and matching active discount bounds..." << endl;
    vector<int> finalPrices = solver.finalPrices(prices);

    cout << "Adjusted Invoice Ledger Matrix:\n[";
    for (size_t i = 0; i < finalPrices.size(); ++i) {
        cout << finalPrices[i] << (i + 1 < finalPrices.size() ? ", " : "");
    }
    cout << "]\n";

    return 0;
}