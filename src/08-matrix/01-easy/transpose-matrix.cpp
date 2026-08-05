#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * COORDINATE INDEX INVERSION MATRIX MUTATION ENGINE
     * --------------------------------------------------
     * The objective is to calculate the transpose of a given 2D matrix.
     * The transpose of a matrix is obtained by flipping it over its main diagonal,
     * which converts the matrix's row indices into column indices and vice versa.
     *
     * Mathematical & Algorithmic Strategy:
     * - Let the input matrix `matrix` have dimensions $R \times C$ (where $R$ is the number of rows
     *   and $C$ is the number of columns).
     * - The resulting transposed matrix `transposedMatrix` will structurally have dimensions $C \times R$.
     * - For every valid coordinate pair $(r, c)$ in the original matrix, the value is mapped
     *   to its inverted index destination coordinate $(c, r)$ in the new matrix space:
     *   transposedMatrix[c][r] = matrix[r][c]
     * - This approach naturally handles non-square (rectangular) matrices where $R \neq C$ safely
     *   by isolating the allocations beforehand.
     *
     * Execution Protocol:
     * - Step 1: Capture the baseline matrix sizes $R$ and $C$.
     * - Step 2: Initialize the destination matrix layout with inverted dimensions $C \times R$.
     * - Step 3: Use nested loops to iterate through the source coordinates, shifting each 
     *   element to its new inverted row-column target index location.
     */
    vector<vector<int>> transpose(vector<vector<int>>& matrix) {
        size_t rows = matrix.size();
        size_t cols = matrix[0].size();

        // Step 2: Allocate target matrix grid space with inverted row and column sizes
        vector<vector<int>> transposedMatrix(cols, vector<int>(rows));

        // Step 3: Traverse coordinates sequentially to copy inverted index metrics
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                transposedMatrix[c][r] = matrix[r][c];
            }
        }

        return transposedMatrix;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity 
 *                  of the input grid layout. The nested traversal engine copies every single 
 *                  matrix cell value exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Excluding the memory footprint dynamically allocated to return 
 *                   the required transposed output structure, the re-indexing execution loop 
 *                   runs strictly in place.
 */
int main() {
    int rows, cols;
    cout << "Enter source matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary parameter dimensions configured." << endl;
        return 1;
    }

    vector<vector<int>> matrix(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter source matrix integer cell values row by row:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nExecuting coordinate index inversion routine..." << endl;
    vector<vector<int>> resultMatrix = solver.transpose(matrix);

    int resultRows = static_cast<int>(resultMatrix.size());
    int resultCols = static_cast<int>(resultMatrix[0].size());

    cout << "\nProcessed transposed matrix configuration visualization:\n";
    for (int r = 0; r < resultRows; ++r) {
        for (int c = 0; c < resultCols; ++c) {
            cout << resultMatrix[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}