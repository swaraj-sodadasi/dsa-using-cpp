#include <iostream>
#include <stack>
#include <utility>
#include <string>

using namespace std;

class StockSpanner {
private:
    // Monotonic stack storing pairs of {price, calculated_span}
    stack<pair<int, int>> monotonicStack;

public:
    StockSpanner() {}

    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ONLINE MONOTONIC VALUE-INDEX PAIR TRACKING STREAM
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the span of the stock's price for the current day,
     * defined as the maximum number of consecutive days (starting from today and going 
     * backward) for which the price of the stock was less than or equal to today's price.
     * 
     * 1. THE ONLINE STREAM CONSTRAINT:
     *    Unlike offline array problems, data points arrive sequentially one by one. 
     *    A naive approach records all elements in an array and walks backward on each 
     *    invocation, yielding quadratic O(N^2) time overall. Instead, we compute the span 
     *    immediately upon streaming entry using a compressed monotonic stack.
     * 
     * 2. ELEMENT VALUE-SPAN COMPRESSION MATRIX:
     *    Instead of tracking indices, each stack frame stores a pair: `{price, span}`.
     *    When a new price arrives:
     *      - We initialize its baseline `currentSpan = 1` (the day itself).
     *      - We look at the top of our stack: as long as the stack is not empty and the 
     *        top element's price is less than or equal to the incoming price, we absorb it.
     *      - We add the top element's precalculated span directly to our `currentSpan` 
     *        and pop it. This collapses multiple historical days into a single operation.
     *      - Finally, we push the consolidated `{price, currentSpan}` back onto the stack 
     *        and return `currentSpan`.
     */
    int next(int price) {
        int currentSpan = 1;

        // Absorb all previous structural frames that have a lower or equal price threshold
        while (!monotonicStack.empty() && monotonicStack.top().first <= price) {
            currentSpan += monotonicStack.top().second;
            monotonicStack.pop();
        }

        // Push the consolidated value-span block candidate onto the streaming stack track
        monotonicStack.push({price, currentSpan});
        return currentSpan;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Amortized - Each call to next() pushes one element onto the stack. 
 *                  While the inner while loop can pop multiple elements in a single invocation, 
 *                  any given element can be popped at most once across the entire stream runtime lifecycle.
 *
 * SPACE COMPLEXITY: O(N) Worst-case - In the worst-case scenario where stock prices are strictly 
 *                   decreasing (e.g., [100, 90, 80, 70]), no elements are popped, and the stack 
 *                   depth scales linearly with the total number of processed stream updates.
 */
int main() {
    cout << "=== Online Monotonic Value-Index Pair Tracking Stream Console ===\n";
    StockSpanner spanner;

    cout << "Commands: 'next [price]', 'exit'\n\n";
    string operationalCommand;

    while (cout << "> ", cin >> operationalCommand) {
        if (operationalCommand == "next") {
            int currentPrice;
            if (cin >> currentPrice) {
                int computedSpan = spanner.next(currentPrice);
                cout << "Price Input: [" << currentPrice << "] -> Calculated Span Matrix Outcome: [" << computedSpan << "]\n";
            }
        } else if (operationalCommand == "exit") {
            break;
        } else {
            cout << "Unknown command sequence configuration. Retry.\n";
            cin.clear();
            string clearBuffer;
            getline(cin, clearBuffer);
        }
    }

    return 0;
}