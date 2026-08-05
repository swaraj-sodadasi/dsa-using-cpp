#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * 2D MATRIX DYNAMIC PROGRAMMING AREA EXPANSION ENGINE
     * ----------------------------------------------------
     * The objective is to find the maximum square sub-matrix containing only '1's 
     * in a binary matrix and return its total area.
     *
     * Mathematical & Dynamic Programming Strategy:
     * - Let `dp[r][c]` represent the side length of the largest square whose bottom-right 
     *   corner is situated at coordinate index position `(r, c)`.
     * - A square grid can only extend if all three adjacent contributing corners can support it. 
     *   Therefore, the state transition relation formula expands as follows:
     *   If grid[r][c] == '1':
     *       dp[r][c] = min({ dp[r-1][c], dp[r][c-1], dp[r-1][c-1] }) + 1
     *   If grid[r][c] == '0':
     *       dp[r][c] = 0
     *
     * Space Optimization Protocol (1D Vector Ring Cache):
     * - Computing the current row relies exclusively on the values from the *current row* (left cell) 
     *   and the *previous row* (top and top-left cells).
     * - Instead of maintaining a full $O(R \times C)$ grid structure, we reduce memory allocations 
     *   to a single 1D vector row buffer cache of size $C + 1$. 
     * - We maintain a scalar tracking variable `prevDiagonal` to cache the old `dp[r-1][c-1]` diagonal 
     *   state parameter before it gets overwritten by the column advancement steps.
     *
     * Execution Protocol:
     * - Step 1: Initialize a 1D optimization vector container `dp` of size `cols + 1` filled with 0s.
     * - Step 2: Traverse each grid row sequentially. Reset `prevDiagonal = 0` at the start of each row.
     * - Step 3: Parse elements column by column. Evaluate the cell, update the ring buffer, 
     *           and capture the max side length discovered.
     * - Step 4: Square the maximum side length to compute the total area and return it.
     */
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;

        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());
        
        // Step 1: Pre-allocate space-optimized 1D tracking container array
        vector<int> dp(static_cast<size_t>(cols + 1), 0);
        int maxSideLength = 0;
        int prevDiagonal = 0; // Caches the top-left structural diagonal state node

        // Step 2 & 3: Run row-major area expansion logic sweeps
        for (int r = 0; r < rows; ++r) {
            prevDiagonal = 0; // Reset row baseline threshold pointer before column iterations
            
            for (int c = 1; c <= cols; ++c) {
                int tempCache = dp[static_cast<size_t>(c)];
                
                if (matrix[static_cast<size_t>(r)][static_cast<size_t>(c - 1)] == '1') {
                    // Update current cell state using top, left, and diagonal historical parameters
                    dp[static_cast<size_t>(c)] = min({dp[static_cast<size_t>(c)], dp[static_cast<size_t>(c - 1)], prevDiagonal}) + 1;
                    maxSideLength = max(maxSideLength, dp[static_cast<size_t>(c)]);
                } else {
                    dp[static_cast<size_t>(c)] = 0;
                }
                
                // Shift cached item to serve as the diagonal cell for the next column loop step
                prevDiagonal = tempCache;
            }
        }

        // Step 4: Multiply dimensions to derive the total area metric space
        return maxSideLength * maxSideLength;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity 
 *                  of the input grid matrix. The nested tracking execution pass evaluates each 
 *                  cell coordinate exactly once.
 *
 * SPACE COMPLEXITY: O(C) Auxiliary - Bypasses full matrix replication allocation models by maintaining 
 *                   only a single space-optimized linear tracking row cache vector of size C + 1.
 */
int main() {
    int rows, cols;
    cout << "Enter binary matrix grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary parameter layouts configured." << endl;
        return 1;
    }

    vector<vector<char>> matrix(static_cast<size_t>(rows), vector<char>(static_cast<size_t>(cols)));
    cout << "Enter matrix single-character element values row by row ('1' or '0' only):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] != '0' && 
                matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] != '1') {
                cout << "Constraint Error: Grid elements must be strictly character '0' or '1'." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting 2D space-optimized dynamic area expansion checks..." << endl;
    int maximalSquareArea = solver.maximalSquare(matrix);

    cout << "Total computed area metric of the largest identified '1' square: " << maximalSquareArea << endl;

    return 0;
}