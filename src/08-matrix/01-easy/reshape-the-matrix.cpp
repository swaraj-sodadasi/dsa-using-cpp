#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * ROW-MAJOR TO COLUMN-MAJOR MAPPING ARITHMETIC ENGINE
     * ----------------------------------------------------
     * The objective is to reshape an M x N matrix into a new matrix with dimensions R x C.
     * If the transformation is impossible due to a mismatch in total element capacity, 
     * the original matrix must be returned unmodified.
     *
     * Algorithmic & Mathematical Strategy:
     * - Let the original matrix have dimensions `originalRows` and `originalCols`. The total 
     *   number of elements inside is `totalElements = originalRows * originalCols`.
     * - A reshape operation is mathematically valid if and only if: `r * c == totalElements`. 
     *   If this conditional invariant fails, we halt and return the source matrix layout.
     * - To execute the conversion in O(1) auxiliary space, we use row-major flattening logic. 
     *   Any 2D grid index (i, j) can be mapped to a flat 1D array index `linearIndex` using:
     *   linearIndex = i * originalCols + j
     * - Conversely, a flat `linearIndex` can be mapped back into any target 2D grid of width 
     *   `targetCols` using modular arithmetic:
     *   targetRow = linearIndex / targetCols
     *   targetCol = linearIndex % targetCols
     *
     * Execution Protocol:
     * - Step 1: Compute dimensions and verify capacity equality constraints.
     * - Step 2: Initialize a new destination grid `reshapedMatrix` with dimensions R x C.
     * - Step 3: Iterate through every element of the original matrix using a flat index mapper, 
     *   projecting values directly into their correct target locations.
     */
    vector<vector<int>> matrixReshape(vector<vector<int>>& mat, int r, int c) {
        int originalRows = static_cast<int>(mat.size());
        int originalCols = static_cast<int>(mat[0].size());
        
        // Step 1: Validate transformation mapping constraints
        if (originalRows * originalCols != r * c) {
            return mat;
        }

        // Step 2: Allocate the new target matrix layout space
        vector<vector<int>> reshapedMatrix(static_cast<size_t>(r), vector<int>(static_cast<size_t>(c)));

        // Step 3: Flatten coordinates sequentially to populate target matrix cells
        for (int i = 0; i < originalRows; ++i) {
            for (int j = 0; j < originalCols; ++j) {
                // Calculate the global 1D index footprint mapping location
                int linearIndex = i * originalCols + j;

                // Deconstruct 1D sequence index to isolate new target 2D indexes
                size_t targetRow = static_cast<size_t>(linearIndex / c);
                size_t targetCol = static_cast<size_t>(linearIndex % c);

                reshapedMatrix[targetRow][targetCol] = mat[static_cast<size_t>(i)][static_cast<size_t>(j)];
            }
        }

        return reshapedMatrix;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents the target row dimension count and C represents 
 *                  the target column dimension count. The code steps through every single element 
 *                  in the input grid exactly once to direct its flat re-indexing target allocation.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Excluding the memory structure initialized to hold the required 
 *                   output dataset, the conversion logic processes indexing dynamically in place.
 */
int main() {
    int originalRows, originalCols;
    cout << "Enter source matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> originalRows >> originalCols) || originalRows <= 0 || originalCols <= 0) {
        cout << "Invalid boundary parameter choices configured." << endl;
        return 1;
    }

    vector<vector<int>> mat(static_cast<size_t>(originalRows), vector<int>(static_cast<size_t>(originalCols)));
    cout << "Enter source matrix integer values row by row:\n";
    for (int i = 0; i < originalRows; ++i) {
        for (int j = 0; j < originalCols; ++j) {
            cin >> mat[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    int targetRows, targetCols;
    cout << "\nEnter new target matrix layout dimensions (Rows followed by Columns): ";
    cin >> targetRows >> targetCols;

    Solution solver;
    cout << "\nExecuting row-major arithmetic element remapping sweeps..." << endl;
    vector<vector<int>> resultMatrix = solver.matrixReshape(mat, targetRows, targetCols);

    int actualResultRows = static_cast<int>(resultMatrix.size());
    int actualResultCols = static_cast<int>(resultMatrix[0].size());

    cout << "\nProcessed matrix structural output representation:\n";
    for (int i = 0; i < actualResultRows; ++i) {
        for (int j = 0; j < actualResultCols; ++j) {
            cout << resultMatrix[static_cast<size_t>(i)][static_cast<size_t>(j)] << " ";
        }
        cout << "\n";
    }

    // Dynamic verification message notice check
    if (actualResultRows == originalRows && actualResultCols == originalCols && targetRows != originalRows) {
        cout << "[Notice: Invariant failure encountered. Returned original grid unchanged.]" << endl;
    }

    return 0;
}