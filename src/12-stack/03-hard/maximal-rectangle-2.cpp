#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * HELPER LOGIC: MONOTONIC STACK LARGEST RECTANGLE IN HISTOGRAM
     * Evaluates a 1D row layout to find the largest rectangular bound in linear time.
     */
    int largestRectangleArea(const vector<int>& heights) {
        stack<int> indexStack;
        int maxArea = 0;
        int n = static_cast<int>(heights.size());

        for (int i = 0; i <= n; ++i) {
            int currentHeight = (i == n) ? 0 : heights[static_cast<size_t>(i)];

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

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: 2D MATRIX HISTOGRAM ARRAY TRANSLATION + MONOTONIC STACK CORE EXECUTION
     * -------------------------------------------------------------------------------------
     * The objective is to find the largest rectangle containing only '1's in a 2D binary matrix.
     * 
     * 1. THE DYNAMIC ROW-TO-HISTOGRAM TRANSLATION:
     *    We transform the 2D matrix problem into consecutive 1D "Largest Rectangle in Histogram"
     *    sub-problems. We maintain an integer array `heights` tracking the width/height profile 
     *    of consecutive '1' cells standing above each column index.
     * 
     * 2. HORIZONTAL ACCUMULATION PIPELINE:
     *    As we iterate row by row down the matrix:
     *      - If matrix[row][col] == '1', we increment our cumulative height counter: `heights[col] += 1`.
     *      - If matrix[row][col] == '0', the structural pillar breaks; we reset the value: `heights[col] = 0`.
     *    After parsing each row, we feed the compiled histogram baseline into our monotonic stack engine,
     *    updating the global maximum area covered across all matrix levels.
     */
    int maximalRectangle(const vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;

        int numCols = static_cast<int>(matrix[0].size());
        vector<int> heights(static_cast<size_t>(numCols), 0);
        int maxGlobalArea = 0;

        for (const auto& row : matrix) {
            // Update the height matrix profile for the current operational row sequence
            for (int col = 0; col < numCols; ++col) {
                if (row[static_cast<size_t>(col)] == '1') {
                    heights[static_cast<size_t>(col)] += 1;
                } else {
                    heights[static_cast<size_t>(col)] = 0; // Ground broken; reset baseline depth
                }
            }
            // Execute the monotonic stack algorithm over the calculated row histogram height track
            maxGlobalArea = max(maxGlobalArea, largestRectangleArea(heights));
        }

        return maxGlobalArea;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents matrix rows and C represents columns. 
 *                  Updating histogram profiles and invoking the monotonic stack pass both scale
 *                  linearly per row, resulting in a tight optimal bounded execution grid.
 *
 * SPACE COMPLEXITY: O(C) Auxiliary - Memory footprint requires a tracking array of size matching
 *                   the total columns, combined with the inner stack allocation depth.
 */
int main() {
    cout << "=== 2D Histogram Matrix Maximal Rectangle Engine ===\n";
    cout << "Enter total row count (R): ";
    int rows;
    if (!(cin >> rows) || rows < 0) return 1;

    cout << "Enter total column count (C): ";
    int cols;
    if (!(cin >> cols) || cols < 0) return 1;

    vector<vector<char>> matrix(static_cast<size_t>(rows), vector<char>(static_cast<size_t>(cols)));
    if (rows > 0 && cols > 0) {
        cout << "Enter the 2D matrix characters grid row by row (composed of '0' and '1'):\n";
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
            }
        }
    }

    Solution solver;
    cout << "\nTranslating layer baselines into active histograms and optimizing boundaries..." << endl;
    int maximalArea = solver.maximalRectangle(matrix);

    cout << "Maximal Continuous Binary Area Found: [" << maximalArea << "]\n";

    return 0;
}