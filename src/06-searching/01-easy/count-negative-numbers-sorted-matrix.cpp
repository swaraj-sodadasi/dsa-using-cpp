#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * STAIRCASE ELIMINATION FROM CORNER ENGINE
     * ----------------------------------------
     * The objective is to count the total number of negative values inside an M x N 
     * matrix that is sorted monotonically in descending order both row-wise and column-wise.
     *
     * Architectural Matrix Structure:
     *   [  4,  3,  2, -1 ]
     *   [  3,  2,  1, -1 ]
     *   [  1,  1, -1, -2 ]
     *   [ -1, -1, -2, -3 ]
     *
     * Algorithmic & Geometric Strategy:
     * - A naive search sweeps every cell, taking quadratic O(M * N) time.
     * - We optimize this to linear O(M + N) time by exploiting the structural sorting property.
     *   We start our search from the **bottom-left corner** (row = M - 1, col = 0).
     *
     * Elimination Walk Strategy:
     * - At any cell `matrix[row][col]`:
     *   - If the current value is negative (`< 0`), then because the matrix is sorted in descending 
     *     order down each column, **all elements below this cell in the same column are also negative**.
     *     More importantly, every single element to the right of this cell in the current row 
     *     must also be negative: `[col, col + 1, ..., N - 1]`.
     *   - Therefore, we can instantly count all negative elements for this row: `totalNegativeItems += (N - col)`.
     *     Since this row's negative elements are completely counted, we step upward: `row--`.
     *   - If the current value is non-negative (`>= 0`), it means all elements to the left of this 
     *     cell in the current row are also non-negative. This entire column cell state is safe, 
     *     so we step to the right to look for the negative boundary: `col++`.
     * - The process forms a "staircase" path that cuts diagonally across the grid matrix, exiting 
     *   the boundaries in at most M + N steps.
     */
    int countNegatives(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rowsCount = static_cast<int>(grid.size());
        int colsCount = static_cast<int>(grid[0].size());

        // Initialize coordinates to the bottom-left corner of the matrix layout
        int row = rowsCount - 1;
        int col = 0;
        int totalNegativeItems = 0;

        // Traverse the grid within valid boundaries
        while (row >= 0 && col < colsCount) {
            if (grid[static_cast<size_t>(row)][static_cast<size_t>(col)] < 0) {
                // If grid[row][col] is negative, all elements to its right in this row are negative
                totalNegativeItems += (colsCount - col);
                
                // Move upward to evaluate the preceding row segment
                row--;
            } else {
                // If grid[row][col] is non-negative, move right to find the negative transition point
                col++;
            }
        }

        return totalNegativeItems;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M + N) - Where M represents rows count and N represents columns count.
 *                  At each loop iteration, we either decrement the row pointer or increment 
 *                  the column pointer, bounding the path length to the matrix dimensions.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The matrix staircase search operates completely in-place 
 *                   using simple integer tracking variables, requiring no extra memory overhead.
 */
int main() {
    int m, n;
    cout << "Enter the matrix dimension metrics (Rows M and Columns N) separated by a space: ";
    if (!(cin >> m >> n) || m <= 0 || n <= 0) {
        cout << "Invalid parameter. Dimensions must be greater than 0." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n)));
    cout << "Enter matrix cells row by row (sorted in descending order across rows and cols):\n";
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting staircase elimination from corner boundary sweep..." << endl;
    int result = solver.countNegatives(grid);

    cout << "Total count of negative numbers discovered inside the matrix: " << result << endl;

    return 0;
}