#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * ROW/COLUMN OPERATION ACCUMULATION TRACKING ENGINE
     * --------------------------------------------------
     * The objective is to determine the total number of cells containing odd values 
     * in an m x n matrix after applying a series of increment operations defined 
     * in the indices matrix `indices`. Each operation increment updates all elements 
     * in a specified row and all elements in a specified column.
     *
     * Space & Time Optimization Strategy:
     * - An naive matrix simulation requires O(m * n) time per operation, leading to a slow 
     *   O(L * m * n) approach where L is the number of indices updates.
     * - Optimization: Instead of performing updates directly on a full 2D array, we observe 
     *   that the final value of a cell at coordinate (r, c) is simply the total number of 
     *   times row `r` was incremented plus the total number of times column `c` was incremented.
     * - We track the mutation frequencies using two lightweight independent boolean status 
     *   arrays: `rowOddStatus` and `colOddStatus`. Flipping the bit state (odd vs even parity) 
     *   takes only O(1) time per operation.
     *
     * Parity Accumulation Reduction Logic:
     * - A final cell value is odd if and only if one of its components (row count or column count) 
     *   is odd and the other is even.
     * - Let `oddRowsCount` be the total number of rows flagged with odd parity, and `oddColsCount` 
     *   be the total number of columns flagged with odd parity.
     * - The number of even rows is `(m - oddRowsCount)`, and the number of even columns is 
     *   `(n - oddColsCount)`.
     * - Using pure combinatorics, the number of odd value matrix cells is derived as:
     *   Total Odd Cells = oddRowsCount * (n - oddColsCount) + oddColsCount * (m - oddRowsCount)
     *
     * Execution Protocol:
     * - Step 1: Initialize boolean state tracking flags for rows and columns.
     * - Step 2: Loop through each indices command pair, flipping the odd parity bit for the target indices.
     * - Step 3: Scan the tracking vectors to count the absolute frequencies of true flags.
     * - Step 4: Apply the combinatorics parity equation to compute the output result in O(m + n) time.
     */
    int oddCells(int m, int n, vector<vector<int>>& indices) {
        // Step 1: Initialize bit counters to manage tracking parity statuses
        vector<bool> rowOddStatus(static_cast<size_t>(m), false);
        vector<bool> colOddStatus(static_cast<size_t>(n), false);

        // Step 2: Toggle parity frequencies for targeted rows and columns
        for (const auto& op : indices) {
            int targetRow = op[0];
            int targetCol = op[1];

            rowOddStatus[static_cast<size_t>(targetRow)] = !rowOddStatus[static_cast<size_t>(targetRow)];
            colOddStatus[static_cast<size_t>(targetCol)] = !colOddStatus[static_cast<size_t>(targetCol)];
        }

        // Step 3: Count total tracking vectors holding odd states
        int oddRowsCount = 0;
        for (int r = 0; r < m; ++r) {
            if (rowOddStatus[static_cast<size_t>(r)]) {
                oddRowsCount++;
            }
        }

        int oddColsCount = 0;
        for (int c = 0; c < n; ++c) {
            if (colOddStatus[static_cast<size_t>(c)]) {
                oddColsCount++;
            }
        }

        // Step 4: Execute mathematical reduction to output the result without allocating a full matrix
        int oddCellsTotal = oddRowsCount * (n - oddColsCount) + oddColsCount * (m - oddRowsCount);
        return oddCellsTotal;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(L + M + N) - Where L represents the length capacity footprint of the 
 *                  indices array, M represents rows count, and N represents columns count. 
 *                  The state accumulation tracks indices in O(L) time, and the tracking scan 
 *                  takes O(M + N) steps, bypassing full grid traversal overhead completely.
 *
 * SPACE COMPLEXITY: O(M + N) Auxiliary - Avoids full O(M * N) grid layout matrices by storing 
 *                   only two isolated boolean state arrays matching matrix dimensions.
 */
int main() {
    int m, n;
    cout << "Enter matrix structural layout bounds (Rows followed by Columns): ";
    if (!(cin >> m >> n) || m <= 0 || n <= 0) {
        cout << "Invalid matrix boundary parameter options configured." << endl;
        return 1;
    }

    int operationsCount;
    cout << "Enter total count threshold of increment operations to process: ";
    if (!(cin >> operationsCount) || operationsCount < 0) {
        cout << "Invalid operations parameter count value choice." << endl;
        return 1;
    }

    vector<vector<int>> indices(static_cast<size_t>(operationsCount), vector<int>(2));
    if (operationsCount > 0) {
        cout << "Enter operation index coordinates row by row (Format: row_index column_index):\n";
        for (int i = 0; i < operationsCount; ++i) {
            cin >> indices[static_cast<size_t>(i)][0] >> indices[static_cast<size_t>(i)][1];

            // Validation boundary index check logic
            if (indices[static_cast<size_t>(i)][0] < 0 || indices[static_cast<size_t>(i)][0] >= m ||
                indices[static_cast<size_t>(i)][1] < 0 || indices[static_cast<size_t>(i)][1] >= n) {
                cout << "Constraint Error: Operation parameter indices fall outside matrix dimensional bounds." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting lightweight tracking state evaluations..." << endl;
    int resultingOddCells = solver.oddCells(m, n, indices);

    cout << "Total computed matrix grid cells containing odd integer values: " << resultingOddCells << endl;

    return 0;
}