#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * HISTOGRAM MONOTONIC STACK INTEGRATION ENGINE
     * ---------------------------------------------
     * Given an M x N binary matrix, count the total number of submatrices filled with 1s.
     *
     * Mathematical & Algorithmic Strategy:
     * - Step 1: Compress the 2D matrix layout by maintaining a linear `heights` vector of size N. 
     *   As we loop row by row, if `mat[r][c] == 1`, we increment `heights[c]`. If it's 0, we reset `heights[c] = 0`.
     * - Step 2: For each row, calculate the number of valid submatrices ending on that row by treating 
     *   the row as a histogram.
     * - Step 3: Use a monotonic stack to find the nearest column to the left with a strictly smaller height 
     *   (`leftSmaller`). 
     * - Step 4: Maintain a dynamic programming tracking buffer `dp` where `dp[c]` represents the total valid 
     *   submatrices ending at column `c` for the current row baseline.
     *   The state transition formula:
     *   `dp[c] = heights[c] * (c - leftSmaller) + (leftSmaller == -1 ? 0 : dp[leftSmaller])`
     * - Step 5: Accumulate the sum of all elements in the `dp` buffer across all row sweeps into `totalSubmatrices`.
     */
    int numSubmat(vector<vector<int>>& mat) {
        if (mat.empty() || mat[0].empty()) return 0;

        int rows = static_cast<int>(mat.size());
        int cols = static_cast<int>(mat[0].size());

        vector<int> heights(static_cast<size_t>(cols), 0);
        int totalSubmatrices = 0;

        // Traverse row by row across 2D grid planes
        for (int r = 0; r < rows; ++r) {
            // Step 1: Dynamic structural update of the continuous histogram heights
            for (int c = 0; c < cols; ++c) {
                if (mat[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    heights[static_cast<size_t>(c)] += 1;
                } else {
                    heights[static_cast<size_t>(c)] = 0;
                }
            }

            // Step 2 & 3: Reset stack and DP components for the current histogram line sweep
            stack<int> monoStack; // Stores column indices maintaining an increasing height order
            vector<int> dp(static_cast<size_t>(cols), 0);

            for (int c = 0; c < cols; ++c) {
                // Maintain the strict monotonic ascending layout invariant condition
                while (!monoStack.empty() && heights[static_cast<size_t>(monoStack.top())] >= heights[static_cast<size_t>(c)]) {
                    monoStack.pop();
                }

                int leftSmallerIndex = monoStack.empty() ? -1 : monoStack.top();

                // Step 4: Dynamic Programming accumulation of valid rectangle boundaries
                int currentWidth = c - leftSmallerIndex;
                dp[static_cast<size_t>(c)] = heights[static_cast<size_t>(c)] * currentWidth;
                
                if (leftSmallerIndex != -1) {
                    dp[static_cast<size_t>(c)] += dp[static_cast<size_t>(leftSmallerIndex)];
                }

                // Step 5: Aggregate the submatrices count into the global total accumulator register
                totalSubmatrices += dp[static_cast<size_t>(c)];

                monoStack.push(c);
            }
        }

        return totalSubmatrices;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity. 
 *                  For each row loop plane, every single column coordinate element index is pushed 
 *                  and popped from the monotonic stack at most once, matching absolute linear execution.
 *
 * SPACE COMPLEXITY: O(C) Auxiliary - Bypasses large matrix workspace re-allocations by preserving 
 *                   only linear tracking memory arrays (`heights`, `dp`, and the tracking index stack) 
 *                   scaled to the width of a single row.
 */
int main() {
    int rows, cols;
    cout << "Enter binary matrix grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary parameter choices configured." << endl;
        return 1;
    }

    vector<vector<int>> mat(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix binary cell values row by row (Only 0s and 1s are allowed):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> mat[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (mat[static_cast<size_t>(r)][static_cast<size_t>(c)] != 0 && 
                mat[static_cast<size_t>(r)][static_cast<size_t>(c)] != 1) {
                cout << "Constraint Error: Grid elements must be strictly 0 or 1." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting histogram compression and monotonic stack scans..." << endl;
    int resultingCount = solver.numSubmat(mat);

    cout << "Total computed submatrices containing exclusively all ones: " << resultingCount << endl;

    return 0;
}