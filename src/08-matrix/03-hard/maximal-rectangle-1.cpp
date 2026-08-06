#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: HISTOGRAM COMPRESSION + MONOTONIC STACK DECOMPOSITION
     * -------------------------------------------------------------------------------------
     * The goal is to find the area of the largest rectangle containing only '1's in a binary matrix.
     * We break down the 2D matrix problem into independent 1D "Largest Rectangle in Histogram" tasks.
     * 
     * 1. HISTOGRAM REDUCTION MATRIX PLANE LAYER:
     *    We maintain a running 1D `heights` vector of size C. As we step row by row, if `matrix[r][c] == '1'`,
     *    we increment the continuous vertical bar height: `heights[c]++`. If it is '0', the continuous sequence 
     *    is broken, so we reset the column height: `heights[c] = 0`.
     * 
     * 2. MONOTONIC STACK OPTIMIZATION:
     *    For each row, we treat `heights` as a histogram and calculate its maximal bounding area in linear time.
     *    We use a strict monotonic ascending stack to track column indices. When the current column bar height 
     *    is shorter than the bar at the top of the stack, it acts as a right boundary constraint. We pop elements
     *    and compute areas:
     *      - Height = height of the popped index bar.
     *      - Width  = Current Index - New Stack Top Index - 1 (or current index if stack is empty).
     * 
     * 3. GLOBAL MAX AREA REDUCTION:
     *    We maintain a running max of all computed areas across all rows to determine the absolute absolute result.
     */
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;

        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());
        
        vector<int> heights(static_cast<size_t>(cols), 0);
        int maxRectangleArea = 0;

        // Process each matrix row plane layer systematically
        for (int r = 0; r < rows; ++r) {
            
            // Step 1: Accumulate and flatten vertical heights in linear time
            for (int c = 0; c < cols; ++c) {
                if (matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] == '1') {
                    heights[static_cast<size_t>(c)] += 1;
                } else {
                    heights[static_cast<size_t>(c)] = 0;
                }
            }

            // Step 2: Linear computation scan using a monotonic stack index tracker
            vector<int> monoStack; // Vector operating as an indexing stack trace
            
            for (int c = 0; c <= cols; ++c) {
                // Use a dummy height of 0 at c == cols to flush out any remaining stack frames at the end
                int currentBarHeight = (c == cols) ? 0 : heights[static_cast<size_t>(c)];

                while (!monoStack.empty() && heights[static_cast<size_t>(monoStack.back())] > currentBarHeight) {
                    int targetPopIndex = monoStack.back();
                    monoStack.pop_back();

                    int height = heights[static_cast<size_t>(targetPopIndex)];
                    
                    // Width is determined by the boundaries matching current index and left smaller element index
                    int width = monoStack.empty() ? c : (c - monoStack.back() - 1);
                    
                    maxRectangleArea = max(maxRectangleArea, height * width);
                }
                monoStack.push_back(c);
            }
        }

        return maxRectangleArea;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity.
 *                  Every individual grid coordinate is parsed once during the histogram accumulation pass,
 *                  and then every column index is pushed/popped from the stack at most once per row sweep.
 *
 * SPACE COMPLEXITY: O(C) Auxiliary - Bypasses large matrix re-allocations by maintaining only 
 *                   linear tracking arrays (`heights` and the active monotonic indexing stack vector) 
 *                   scaled to match row capacity.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary configurations configured." << endl;
        return 1;
    }

    vector<vector<char>> matrix(static_cast<size_t>(rows), vector<char>(static_cast<size_t>(cols)));
    cout << "Enter board character values row by row ('1' or '0' only):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] != '0' && 
                matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] != '1') {
                cout << "Constraint Error: Input grid element must be strictly character '0' or '1'." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting global monotonic histogram expansion checks..." << endl;
    int maximalArea = solver.maximalRectangle(matrix);

    cout << "Total maximal area metric computed for the largest '1' rectangle: " << maximalArea << endl;

    return 0;
}