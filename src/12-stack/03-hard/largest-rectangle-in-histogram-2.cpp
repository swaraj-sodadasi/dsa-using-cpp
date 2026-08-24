#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MONOTONIC STACK BOUNDARY WIDTH ALLOCATIONS
     * -------------------------------------------------------------------------------------
     * The objective is to find the area of the largest rectangle in a histogram.
     * 
     * 1. THE BOUNDARY LIMIT INSIGHT:
     *    For each bar, the maximum rectangle using its full height extends left and right 
     *    until it hits a bar shorter than itself. The monotonic stack finds these index bounds.
     * 
     * 2. THE LIQUIDATION MECHANISM:
     *    We maintain a strictly increasing stack of indices. We iterate from 0 to N. 
     *    To flush out any remaining items at the end without duplicate cleanup blocks, 
     *    we evaluate an extra virtual loop index `i == n` with a height candidate of 0.
     *    When `heights[i]` is less than the height at the top of the stack, the bar at 
     *    `stack.top()` has found its right boundary (`i`). We pop this index to calculate 
     *    its rectangle area:
     *      - Height = heights[poppedIndex]
     *      - Left Boundary = new stack.top() index (or -1 if the stack is now empty)
     *      - Width = right boundary - left boundary - 1 -> (i - leftBoundary - 1)
     */
    int largestRectangleArea(const vector<int>& heights) {
        stack<int> indexStack;
        int maxArea = 0;
        int n = static_cast<int>(heights.size());

        // Run loop up to n to simulate a trailing 0 height bar that flushes the stack
        for (int i = 0; i <= n; ++i) {
            int currentHeight = (i == n) ? 0 : heights[static_cast<size_t>(i)];

            // Clear indices from the stack whose heights are larger than the current element
            while (!indexStack.empty() && heights[static_cast<size_t>(indexStack.top())] > currentHeight) {
                int heightIdx = indexStack.top();
                indexStack.pop();

                int barHeight = heights[static_cast<size_t>(heightIdx)];
                int leftBoundaryIdx = indexStack.empty() ? -1 : indexStack.top();
                int currentWidth = i - leftBoundaryIdx - 1;

                maxArea = max(maxArea, barHeight * currentWidth);
            }
            
            indexStack.push(i);
        }

        return maxArea;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of bars in the histogram layout.
 *                  Every index element is pushed and popped from the stack at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal stack tracks at most N index markers inside
 *                   strictly increasing histogram arrangements (e.g., [1, 2, 3, 4, 5]).
 */
int main() {
    cout << "=== Monotonic Stack Largest Rectangle Histogram Engine ===\n";
    cout << "Enter the total number of histogram bars: ";
    int barsCount;
    if (!(cin >> barsCount) || barsCount < 0) return 1;

    vector<int> heights(static_cast<size_t>(barsCount));
    if (barsCount > 0) {
        cout << "Enter the raw integer bar heights sequentially:\n";
        for (int i = 0; i < barsCount; ++i) {
            cin >> heights[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nScanning boundary allocations and maximizing geometric area blocks..." << endl;
    int maxRectangleArea = solver.largestRectangleArea(heights);

    cout << "Largest Measurable Rectangular Area Outcome: [" << maxRectangleArea << "]\n";

    return 0;
}