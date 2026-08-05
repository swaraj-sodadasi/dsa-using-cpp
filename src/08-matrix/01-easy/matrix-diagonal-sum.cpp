#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * INDEX-MATCHING PRIMARY/SECONDARY CROSS COLLECTION ENGINE
     * --------------------------------------------------------
     * The objective is to compute the sum of all elements on the primary diagonal 
     * and the secondary diagonal of a square matrix, ensuring that elements 
     * intersecting both diagonals are counted exactly once.
     *
     * Mathematical & Algorithmic Strategy:
     * - A matrix is square if its row count equals its column count ($N \times N$).
     * - Let $i$ represent the current row index running from $0$ to $N-1$.
     * - Primary Diagonal Rule: The element belongs to the primary diagonal if the row 
     *   index equals the column index. Target: `mat[i][i]`.
     * - Secondary Diagonal Rule: The element belongs to the secondary diagonal if the column 
     *   index matches the reverse linear offset. Target: `mat[i][n - 1 - i]`.
     * - Intersection Management: If the matrix dimension $N$ is odd, the primary and 
     *   secondary diagonals intersect at the exact center element: `mat[n/2][n/2]`. 
     *   By adding both diagonals inside a single loop, this element would be counted twice.
     * - To preserve absolute linear arithmetic execution constraints, we accumulate both diagonal 
     *   cells in one pass, and then check if $N$ is odd. If true, we subtract the center cell once.
     *
     * Execution Protocol:
     * - Step 1: Initialize a scalar accumulator variable `diagonalSumResult` to 0.
     * - Step 2: Loop through the rows from $0$ to $N-1$. Accumulate `mat[i][i]` and `mat[i][n - 1 - i]`.
     * - Step 3: Apply the structural boundary intersection parity check. If `n % 2 != 0`, 
     *   deduct the duplicated center node `mat[n / 2][n / 2]` from the final total.
     */
    int diagonalSum(vector<vector<int>>& mat) {
        size_t n = mat.size();
        int diagonalSumResult = 0;

        // Step 2: Collect both primary and secondary diagonal fields in a single pass
        for (size_t i = 0; i < n; ++i) {
            diagonalSumResult += mat[i][i];             // Primary Diagonal accumulation
            diagonalSumResult += mat[i][n - 1 - i];     // Secondary Diagonal accumulation
        }

        // Step 3: Deduct the intersecting center node if the matrix length is odd
        if (n % 2 != 0) {
            size_t centerIndex = n / 2;
            diagonalSumResult -= mat[centerIndex][centerIndex];
        }

        return diagonalSumResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the row/column dimension count of the square matrix. 
 *                  The single pass loop inspects exactly N steps, avoiding quadratic O(N^2) 
 *                  full-grid search overhead completely.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation structure runs strictly in place, allocating 
 *                   zero extra storage buffers, vectors, or dynamic heap components.
 */
int main() {
    int n;
    cout << "Enter the row/column size capacity of the square matrix (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid boundary parameter choice configured." << endl;
        return 1;
    }

    vector<vector<int>> mat(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n)));
    cout << "Enter matrix integer cell values row by row:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> mat[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting primary and secondary index-matching diagonal passes..." << endl;
    int totalDiagonalSum = solver.diagonalSum(mat);

    cout << "Total computed cross collection diagonal sum metric: " << totalDiagonalSum << endl;

    return 0;
}