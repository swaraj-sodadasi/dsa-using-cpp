#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * ROW MINIMA AND COLUMN MAXIMA CROSS VALIDATION ENGINE
     * ---------------------------------------------------
     * The objective is to discover all "lucky numbers" inside an M x N matrix.
     * A lucky number is defined as an element that is the absolute minimum element 
     * in its row and the absolute maximum element in its column.
     *
     * Algorithmic Strategy (Pre-computed Extrema Cross-Reference):
     * - An element at coordinate (r, c) is lucky if and only if:
     *   matrix[r][c] == min_in_row[r] AND matrix[r][c] == max_in_col[c]
     * - Instead of performing repeated linear scans for each coordinate cell, we pre-calculate 
     *   the properties in two fast cache loops:
     *   1. A vector `rowMinima` of size M to store the minimum value of each row.
     *   2. A vector `colMaxima` of size N to store the maximum value of each column.
     * - Once populated, we make a second pass over the matrix, checking each element 
     *   against these arrays in O(1) time.
     *
     * Execution Protocol:
     * - Step 1: Initialize row tracking caches to INT_MAX and column tracking caches to INT_MIN.
     * - Step 2: Loop through all grid coordinates to populate `rowMinima` and `colMaxima`.
     * - Step 3: Loop through the matrix a second time to cross-validate candidates and gather 
     *   matching elements into a result container.
     */
    vector<int> luckyNumbers(vector<vector<int>>& matrix) {
        size_t rows = matrix.size();
        size_t cols = matrix[0].size();

        // Step 1: Pre-allocate optimization cache vectors matching grid boundaries
        vector<int> rowMinima(rows, INT_MAX);
        vector<int> colMaxima(cols, INT_MIN);

        // Step 2: Compute extreme boundary characteristics across a single pass
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                int value = matrix[r][c];
                rowMinima[r] = min(rowMinima[r], value);
                colMaxima[c] = max(colMaxima[c], value);
            }
        }

        vector<int> luckyCandidates;

        // Step 3: Cross-validate element properties to filter structural matches
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                int value = matrix[r][c];
                if (value == rowMinima[r] && value == colMaxima[c]) {
                    luckyCandidates.push_back(value);
                }
            }
        }

        return luckyCandidates;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * N) - Where M represents row count and N represents column count 
 *                  of the matrix layout. The initialization and cross-validation passes 
 *                  each scan the full matrix grid in linear time relative to total cell counts.
 *
 * SPACE COMPLEXITY: O(M + N) Auxiliary - Avoids full grid replication matrix buffers by 
 *                   storing only the isolated row minima and column maxima lookup vectors.
 */
int main() {
    int rows, cols;
    cout << "Enter matrix grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> matrix(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix cell element integer values row by row:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nExecuting row/column cross-validation parsing loops..." << endl;
    vector<int> result = solver.luckyNumbers(matrix);

    cout << "\nDiscovered lucky numbers sequence vector matching definitions: [ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]" << endl;

    return 0;
}