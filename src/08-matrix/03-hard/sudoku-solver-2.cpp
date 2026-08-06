#include <iostream>
#include <vector>

using namespace std;

class Solution {
private:
    // Bitmasks to track used digits (1-9) across structural dimensions
    // rowConstraints[r] tracks digits in row r, colConstraints[c] tracks digits in col c, etc.
    int rowConstraints[9] = {0};
    int colConstraints[9] = {0};
    int boxConstraints[9] = {0};

    /*
     * DYNAMIC LOOKUP HELPERS
     * ----------------------
     * Computes the 3x3 box index from standard (r, c) matrix coordinates.
     */
    inline int getBoxIndex(int r, int c) {
        return (r / 3) * 3 + (c / 3);
    }

    /*
     * DEPTH-FIRST BACKTRACKING SEARCH ENGINE
     * ---------------------------------------
     * Iterates cell-by-cell to find the lexicographically first valid board completion.
     */
    bool solveSudokuDFS(vector<vector<char>>& board, int cellIndex) {
        // Base Case: All 81 cells successfully processed without constraint violations
        if (cellIndex == 81) {
            return true;
        }

        int r = cellIndex / 9;
        int c = cellIndex % 9;

        // Skip cells that are pre-populated with permanent structural constraints
        if (board[static_cast<size_t>(r)][static_cast<size_t>(c)] != '.') {
            return solveSudokuDFS(board, cellIndex + 1);
        }

        int boxIdx = getBoxIndex(r, c);

        // Try placing digits 1 through 9
        for (int digit = 1; digit <= 9; ++digit) {
            int digitBitmask = 1 << digit;

            // Step 2: In-place Multi-Constraint verification using fast bitwise AND operations
            if (!(rowConstraints[r] & digitBitmask) && 
                !(colConstraints[c] & digitBitmask) && 
                !(boxConstraints[boxIdx] & digitBitmask)) {
                
                // Step 3: Mutate board state and set corresponding validation flags
                board[static_cast<size_t>(r)][static_cast<size_t>(c)] = static_cast<char>('0' + digit);
                rowConstraints[r] |= digitBitmask;
                colConstraints[c] |= digitBitmask;
                boxConstraints[boxIdx] |= digitBitmask;

                // Recurse to process the subsequent downstream matrix cells
                if (solveSudokuDFS(board, cellIndex + 1)) {
                    return true; 
                }

                // Step 4: Backtracking step — restore initial state parameters
                board[static_cast<size_t>(r)][static_cast<size_t>(c)] = '.';
                rowConstraints[r] &= ~digitBitmask;
                colConstraints[c] &= ~digitBitmask;
                boxConstraints[boxIdx] &= ~digitBitmask;
            }
        }

        return false; // Trigger backtracking alert up the recursive call chain
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MATRIX BACKTRACKING WITH MULTI-CONSTRAINT VERIFICATION MATRICES
     * -------------------------------------------------------------------------------------
     * The objective is to solve a standard 9x9 Sudoku puzzle by filling empty cells ('.').
     * The grid must satisfy three overlapping safety conditions simultaneously:
     * 1. Each row must contain digits 1-9 without repetition.
     * 2. Each column must contain digits 1-9 without repetition.
     * 3. Each sub-box grid partition must contain digits 1-9 without repetition.
     *
     * 1. PRE-COMPUTED STATE FLAG INITIALIZATION:
     *    Instead of executing costly linear validation scans of length 9 every time we test a candidate 
     *    digit, we map constraint states to low-level integers operating as bitmasks. An 8-bit integer field 
     *    caches the presence of numbers 1-9 for each row, column, and box cluster.
     *
     * 2. COMBINATORIAL BITWISE VALIDATION:
     *    Testing if a digit can occupy a cell reduces to simple bitwise checking: 
     *    `!(rowConstraints[r] & (1 << digit))`. This converts O(N) validation steps into true O(1) time.
     *
     * 3. DEPTH-FIRST SEARCH WITH BACKTRACKING:
     *    We trace through cells linearly from 0 to 80. If a path fails downstream, the algorithm backtracks:
     *    it clears the cell, clears the matching bitmask flags, and tries the next available digit.
     */
    void solveSudoku(vector<vector<char>>& board) {
        // Step 1: Baseline inspection pass to compile existing constraint bitmasks
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[static_cast<size_t>(r)][static_cast<size_t>(c)] != '.') {
                    int digit = board[static_cast<size_t>(r)][static_cast<size_t>(c)] - '0';
                    int digitBitmask = 1 << digit;
                    
                    rowConstraints[r] |= digitBitmask;
                    colConstraints[c] |= digitBitmask;
                    boxConstraints[getBoxIndex(r, c)] |= digitBitmask;
                }
            }
        }

        // Launch the recursive backtracking algorithm starting at cell index 0
        solveSudokuDFS(board, 0);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(9^M) - Where M represents the total count of empty cells ('.') on the board.
 *                  The extreme theoretical upper bound is limited by the search space options, though 
 *                  overlapping multi-constraint bitmasks prune dead-end branches immediately.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The call stack is bounded by the static grid footprint size (81 levels). 
 *                   The bitmask lookup registers (`rowConstraints`, `colConstraints`, `boxConstraints`) 
 *                   use negligible, fixed global memory layouts.
 */
int main() {
    vector<vector<char>> board(9, vector<char>(9));
    cout << "Enter the 9x9 Sudoku puzzle row by row (use '.' for blank cells):\n";
    
    for (int r = 0; r < 9; ++r) {
        cout << "Row [" << r + 1 << "]: ";
        for (int c = 0; c < 9; ++c) {
            cin >> board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nExecuting bitmask-accelerated backtracking matrix search..." << endl;
    solver.solveSudoku(board);

    cout << "\nSuccessfully Solved Sudoku Grid Layout Configuration:\n";
    for (int r = 0; r < 9; ++r) {
        if (r % 3 == 0 && r != 0) cout << "------+-------+------\n";
        for (int c = 0; c < 9; ++c) {
            if (c % 3 == 0 && c != 0) cout << "| ";
            cout << board[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}