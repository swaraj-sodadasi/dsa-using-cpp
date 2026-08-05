#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * LAYER-BY-LAYER COORDINATE SIMULATION AND GENERATION ENGINE
     * -----------------------------------------------------------
     * The objective is to generate a square matrix of dimensions N x N filled with 
     * elements from 1 to N^2 in spiral order.
     *
     * Algorithmic Strategy (Boundary Compression Navigation):
     * - We define four tracking boundary variables to represent the active layer limits:
     *   `topRow` (0), `bottomRow` (N-1), `leftCol` (0), and `rightCol` (N-1).
     * - We populate the matrix by looping through the current outer layer and then shifting 
     *   the boundaries inward (compressing the tracking box) until all cells are filled.
     * - Each layer loop performs four sequential tracking passes:
     *   1. Traverse from left to right across the current `topRow`, then increment `topRow`.
     *   2. Traverse from top to bottom down the current `rightCol`, then decrement `rightCol`.
     *   3. Traverse from right to left across the current `bottomRow`, then decrement `bottomRow`.
     *   4. Traverse from bottom to top up the current `leftCol`, then increment `leftCol`.
     * - The process continues while our element counter `currentVal <= n * n`.
     *
     * Execution Protocol:
     * - Step 1: Pre-allocate an N x N destination grid initialized with 0.
     * - Step 2: Establish the layer boundary index variables and set `currentVal = 1`.
     * - Step 3: Run the coordinate traversal loops, updating tracking boundaries step-by-step.
     */
    vector<vector<int>> generateMatrix(int n) {
        // Step 1: Initialize the square matrix container layout structure
        vector<vector<int>> spiralGrid(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n), 0));
        
        if (n <= 0) return spiralGrid;

        // Step 2: Establish the perimeter tracking boundary indices
        int topRow = 0;
        int bottomRow = n - 1;
        int leftCol = 0;
        int rightCol = n - 1;

        int currentVal = 1;
        int targetMax = n * n;

        // Step 3: Loop layers systematically until all scalar inputs are written
        while (currentVal <= targetMax) {
            
            // Pass 1: Traverse horizontally from left to right along the top boundary line
            for (int c = leftCol; c <= rightCol && currentVal <= targetMax; ++c) {
                spiralGrid[static_cast<size_t>(topRow)][static_cast<size_t>(c)] = currentVal++;
            }
            topRow++;

            // Pass 2: Traverse vertically from top to bottom along the right boundary line
            for (int r = topRow; r <= bottomRow && currentVal <= targetMax; ++r) {
                spiralGrid[static_cast<size_t>(r)][static_cast<size_t>(rightCol)] = currentVal++;
            }
            rightCol--;

            // Pass 3: Traverse horizontally from right to left along the bottom boundary line
            for (int c = rightCol; c >= leftCol && currentVal <= targetMax; --c) {
                spiralGrid[static_cast<size_t>(bottomRow)][static_cast<size_t>(c)] = currentVal++;
            }
            bottomRow--;

            // Pass 4: Traverse vertically from bottom to top along the left boundary line
            for (int r = bottomRow; r >= topRow && currentVal <= targetMax; --r) {
                spiralGrid[static_cast<size_t>(r)][static_cast<size_t>(leftCol)] = currentVal++;
            }
            leftCol++;
        }

        return spiralGrid;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2) - Where N represents the matrix dimension width capacity. The boundary 
 *                  compression loops visit and populate every single array coordinate index cell 
 *                  exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Bypasses secondary data structure overhead completely. 
 *                   Excluding the output grid allocation matrix container memory footprint, 
 *                   scalar tracking states execute entirely in place.
 */
int main() {
    int n;
    cout << "Enter the grid dimension parameter width size (N) for the square matrix: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid boundary dimension value configured." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting layer-by-layer spiral coordinate mapping..." << endl;
    vector<vector<int>> resultGrid = solver.generateMatrix(n);

    cout << "\nGenerated matrix spiral layout configuration visualization:\n";
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            cout << resultGrid[static_cast<size_t>(r)][static_cast<size_t>(c)] << "\t";
        }
        cout << "\n";
    }

    return 0;
}