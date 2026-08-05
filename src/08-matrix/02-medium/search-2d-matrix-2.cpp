#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * STAIRCASE ELIMINATION STRATEGY ENGINE
     * -------------------------------------
     * The objective is to design an highly efficient algorithm to search for a target value 
     * in an M x N matrix. This matrix possesses the following structural sorting invariants:
     * 1. Integers in each row are sorted in ascending from left to right.
     * 2. Integers in each column are sorted in ascending from top to bottom.
     *
     * Algorithmic Strategy (Staircase Dimensional Reduction):
     * - An intuitive approach might involve searching row-by-row with binary search, taking O(M log N) time.
     *   However, we can optimize this to linear O(M + N) time by utilizing a pointer starting 
     *   at the **Top-Right corner** coordinate (row = 0, col = N - 1).
     * - At any current node matrix[row][col], we can make a decisive elimination choice:
     *   - Case A: If `matrix[row][col] == target`, the value is found, and we return true.
     *   - Case B: If `matrix[row][col] > target`, then because the current column is sorted 
     *             in ascending order downwards, every element below the current node is guaranteed 
     *             to be strictly greater than the target. Thus, we can completely eliminate this column 
     *             by decrementing `col`.
     *   - Case C: If `matrix[row][col] < target`, then because the current row is sorted 
     *             in ascending order from left to right, every element to the left of the current node 
     *             is guaranteed to be strictly less than the target. Thus, we can completely eliminate 
     *             this row by incrementing `row`.
     * - This cell evaluation moves like a staircase down and left, guaranteeing termination as soon as 
     *   the coordinate pointers step outside the boundary constraints.
     *
     * Execution Protocol:
     * - Step 1: Initialize pointer coordinates to the top-right cell: `row = 0`, `col = cols - 1`.
     * - Step 2: Loop while `row < rows` and `col >= 0`.
     * - Step 3: Branch code paths dynamically based on comparison math, shifting row or column vectors.
     */
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;

        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());

        // Step 1: Position structural pointer tracking bounds at the top-right index corner
        int currentRow = 0;
        int currentCol = cols - 1;

        // Step 2 & 3: Run dynamic row/column elimination steps
        while (currentRow < rows && currentCol >= 0) {
            int value = matrix[static_cast<size_t>(currentRow)][static_cast<size_t>(currentCol)];

            if (value == target) {
                return true; // Target matched successfully
            } 
            else if (value > target) {
                currentCol--; // Eliminate the entire column space
            } 
            else {
                currentRow++; // Eliminate the entire row space
            }
        }

        return false; // Target is not present within matrix boundary parameters
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M + N) - Where M represents the row count and N represents the column count 
 *                  of the matrix grid layout. Every comparison operation reliably eliminates at least 
 *                  one entire row or column from the remaining search space, bounding total structural 
 *                  steps linearly.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization strategy evaluates search spaces entirely in place, 
 *                   requiring only scalar coordinate pointers.
 */
int main() {
    int rows, cols;
    cout << "Enter matrix grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid matrix dimension parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> matrix(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter sorted matrix values row by row (must be sorted along both axes):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    int target;
    cout << "\nEnter the target integer value to search for: ";
    cin >> target;

    Solution solver;
    cout << "\nExecuting staircase elimination routine from the top-right corner..." << endl;
    bool discovered = solver.searchMatrix(matrix, target);

    cout << "Search Validation Outcome: " 
         << (discovered ? "TRUE (Target found in matrix)" : "FALSE (Target not found in matrix)") << endl;

    return 0;
}