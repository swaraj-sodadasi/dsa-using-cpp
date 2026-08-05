#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * FIRST ROW/COLUMN IN-PLACE STATE FLAGS OPTIMIZATION ENGINE
     * ---------------------------------------------------------
     * The objective is to design an in-place algorithm that scans an M x N matrix, 
     * and if an element is 0, sets its entire row and column to 0.
     *
     * Space Optimization Strategy:
     * - A naive approach uses an O(M * N) copy matrix. 
     * - A standard optimized approach tracks row/column state histories using O(M + N) 
     *   auxiliary bit arrays.
     * - To achieve true O(1) auxiliary space complexity, we use the matrix's **first row** 
     *   and **first column** as our live state flag markers.
     * - Because the intersection cell matrix[0][0] handles flags for both row 0 and column 0, 
     *   we use a separate scalar variable `firstColHasZero` to track column 0's independent state.
     *
     * Execution Protocol:
     * - Step 1: Scan the first column to determine if it natively contains any zero.
     * - Step 2: Scan the remaining cell grid space (from row 0, col 1). If matrix[r][c] == 0, 
     *   set its corresponding header flags: matrix[r][0] = 0 and matrix[0][c] = 0.
     * - Step 3: Iterate backwards from the bottom-right corner up to row 0. Update cell values 
     *   to 0 if their row or column headers are flagged.
     * - Step 4: Finally, if `firstColHasZero` is true, update the entire first column to 0.
     */
    void setZeroes(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return;

        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());
        bool firstColHasZero = false;

        // Step 1 & 2: Set flag markers in the first row and column
        for (int r = 0; r < rows; ++r) {
            // Check if the first column needs to be zeroed out later
            if (matrix[static_cast<size_t>(r)][0] == 0) {
                firstColHasZero = true;
            }
            
            // Scan the remaining columns to drop header markers
            for (int c = 1; c < cols; ++c) {
                if (matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] == 0) {
                    matrix[static_cast<size_t>(r)][0] = 0;
                    matrix[0][static_cast<size_t>(c)] = 0;
                }
            }
        }

        // Step 3: Iterate backwards to prevent overwriting active header tracking states prematurely
        for (int r = rows - 1; r >= 0; --r) {
            for (int c = cols - 1; c >= 1; --c) {
                if (matrix[static_cast<size_t>(r)][0] == 0 || matrix[0][static_cast<size_t>(c)] == 0) {
                    matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] = 0;
                }
            }
            // Update the first column cell for the current row if the flag is set
            if (firstColHasZero) {
                matrix[static_cast<size_t>(r)][0] = 0;
            }
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * N) - Where M represents row count and N represents column count 
 *                  of the matrix layout grid. The algorithm runs exactly two full grid pass scans.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The header cells of the input grid are recycled as 
 *                   state flag trackers, requiring no extra auxiliary memory structures.
 */
int main() {
    int rows, cols;
    cout << "Enter matrix grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural grid options configured." << endl;
        return 1;
    }

    vector<vector<int>> matrix(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix cell element values row by row:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nExecuting in-place row/column flag marking steps..." << endl;
    solver.setZeroes(matrix);

    cout << "\nProcessed matrix layout state visualization:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cout << matrix[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}