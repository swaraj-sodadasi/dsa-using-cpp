#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * IN-PLACE SORTING OF INDEPENDENT DIAGONAL PATHS ENGINE
     * -----------------------------------------------------
     * The objective is to sort a 2D matrix diagonally in ascending order, where 
     * each diagonal starting from the top-left to bottom-right is independently sorted.
     *
     * Mathematical Alignment Rule:
     * - For any matrix cell at coordinate position (row, col), all elements sharing the exact 
     *   same index difference value (row - col) belong strictly to the same diagonal path.
     *
     * Algorithmic Strategy:
     * - We can isolate and sort each diagonal path independently. To cover every diagonal in the 
     *   matrix, we need to pick a starting cell for each path. Every diagonal starts either at 
     *   the first column of a row (row = i, col = 0) or the first row of a column (row = 0, col = j).
     *
     * Sorting Protocol:
     * - Step 1: Define a helper function `sortDiagonal(matrix, startRow, startCol)` to manage a path.
     * - Step 2: Traverse along the diagonal path from the starting cell down to the matrix boundary, 
     *   extracting all element values into a temporary scratchpad vector (`diagonalBuffer`).
     * - Step 3: Sort the extracted values in ascending order using `std::sort`.
     * - Step 4: Write the sorted values back into the matrix along the same diagonal path, updating 
     *   the cells in-place.
     * - Step 5: Run this sorting process for all diagonals starting along the left boundary (row 0 to M-1, col 0), 
     *   and all diagonals starting along the top boundary (row 0, col 1 to N-1).
     */
    void sortDiagonal(vector<vector<int>>& mat, int startRow, int startCol, int m, int n) {
        vector<int> diagonalBuffer;
        int r = startRow;
        int c = startCol;

        // Step 2: Extract elements along the current diagonal path
        while (r < m && c < n) {
            diagonalBuffer.push_back(mat[static_cast<size_t>(r)][static_cast<size_t>(c)]);
            r++;
            c++;
        }

        // Step 3: Sort the buffered elements in ascending order
        sort(diagonalBuffer.begin(), diagonalBuffer.end());

        // Step 4: Write the sorted elements back into the matrix in-place
        r = startRow;
        c = startCol;
        size_t bufferIndex = 0;
        while (r < m && c < n) {
            mat[static_cast<size_t>(r)][static_cast<size_t>(c)] = diagonalBuffer[bufferIndex++];
            r++;
            c++;
        }
    }

    vector<vector<int>>& diagonalSort(vector<vector<int>>& mat) {
        if (mat.empty() || mat[0].empty()) return mat;

        int m = static_cast<int>(mat.size());
        int n = static_cast<int>(mat[0].size());

        // Step 5: Process all diagonals starting from the first column of each row
        for (int i = 0; i < m; ++i) {
            sortDiagonal(mat, i, 0, m, n);
        }

        // Process all diagonals starting from the first row of each column (skipping 0,0)
        for (int j = 1; j < n; ++j) {
            sortDiagonal(mat, 0, j, m, n);
        }

        return mat;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * N * log(min(M, N))) - Where M represents the row count and N represents 
 *                  the column count of the matrix. The total number of cells processed is M * N. 
 *                  Each cell belongs to exactly one diagonal, and the maximum length of any diagonal 
 *                  is bounded by min(M, N). Sorting a diagonal takes O(L log L) time where L <= min(M, N).
 *
 * SPACE COMPLEXITY: O(min(M, N)) Auxiliary - The allocation buffer stores elements for one diagonal 
 *                   at a time during the intermediate sorting steps, bounded by the maximum diagonal length.
 */
int main() {
    int m, n;
    cout << "Enter the total number of rows in the matrix (M): ";
    if (!(cin >> m) || m <= 0) {
        cout << "Invalid parameter. Row count must be a positive integer value." << endl;
        return 1;
    }

    cout << "Enter the total number of columns in the matrix (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Column count must be a positive integer value." << endl;
        return 1;
    }

    vector<vector<int>> mat(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n)));
    cout << "\nEnter the elements of the grid matrix row by row:\n";
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> mat[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting in-place sorting of independent diagonal paths..." << endl;
    solver.diagonalSort(mat);

    cout << "\nDiagonally sorted matrix output structure:\n";
    for (int i = 0; i < m; ++i) {
        cout << "[ ";
        for (int j = 0; j < n; ++j) {
            cout << mat[static_cast<size_t>(i)][static_cast<size_t>(j)] << (j + 1 < n ? ", " : "");
        }
        cout << " ]\n";
    }

    return 0;
}