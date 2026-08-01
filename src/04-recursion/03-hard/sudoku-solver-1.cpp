#include <iostream>
#include <vector>

using namespace std;

class Solution {
private:
    /*
     * BITMASK TRACKING MECHANISMS (O(1) CONSTANT-TIME SAFETY CHECKS)
     * ----------------------------------------------------------------
     * Traditional Sudoku validation loops through rows, columns, and 3x3 grids,
     * which costs extra lookups per validation attempt.
     * By utilizing bitmasks (integers), we track used numbers as flipped bits.
     * For a value 'd' (1 to 9), its presence is represented by the bit (1 << d).
     *
     * 1. rowsMask: 9 integers tracking values present in each row.
     * 2. colsMask: 9 integers tracking values present in each column.
     * 3. boxesMask: 9 integers tracking values present in each 3x3 sub-box.
     *    The sub-box index mapping formula is: (row / 3) * 3 + (col / 3).
     */
    int rowsMask[9] = {0};
    int colsMask[9] = {0};
    int boxesMask[9] = {0};

    /*
     * RECURSIVE CONSTRAINT SATISFACTION ENGINE
     * -----------------------------------------
     * This function attempts to solve the board using a depth-first backtracking search.
     * It scans cells sequentially. If an empty cell is found, it attempts to place
     * valid digits by checking bitwise collision flags.
     *
     * State Parameters:
     *  - board: The 9x9 character matrix grid representing the active puzzle state.
     *  - r: Current row coordinate tracker.
     *  - c: Current column coordinate tracker.
     */
    bool solve(vector<vector<char>>& board, int r, int c) {
        // BASE CASE: COLUMN EDGE OVERFLOW
        // If we reach column 9, we have completed processing the current row.
        // Advance to the beginning of the next consecutive row.
        if (c == 9) {
            return solve(board, r + 1, 0);
        }

        // BASE CASE: SUCCESS CONVERGENCE REACHED
        // If the row pointer reaches index 9, the entire board has been traversed
        // and safely populated with numbers that fulfill all constraint dimensions.
        if (r == 9) {
            return true;
        }

        // If the current cell is already filled, skip it and advance to the next column
        if (board[static_cast<size_t>(r)][static_cast<size_t>(c)] != '.') {
            return solve(board, r, c + 1);
        }

        // Compute the structural sub-box grid index location
        int boxIdx = (r / 3) * 3 + (c / 3);

        // TRANSITION LOOP: TRY DIGIT SELECTION (1 TO 9)
        for (int d = 1; d <= 9; ++d) {
            int mask = 1 << d;

            // BITWISE CONFLICT DETECTION
            // Query if digit 'd' is already locked inside this row, column, or sub-box.
            // If the bitwise AND returns non-zero, the digit breaks constraints -> Prune branch.
            if ((rowsMask[r] & mask) || (colsMask[c] & mask) || (boxesMask[boxIdx] & mask)) {
                continue;
            }

            // STEP 1: CHOOSE 
            // Commit the character digit representation and toggle bit masks to 1
            board[static_cast<size_t>(r)][static_cast<size_t>(c)] = static_cast<char>('0' + d);
            rowsMask[r] |= mask;
            colsMask[c] |= mask;
            boxesMask[boxIdx] |= mask;

            // STEP 2: RECURSE 
            // Move forward to process the next step cell space in the active row path
            if (solve(board, r, c + 1)) {
                return true; // Bubble true up the execution layers immediately if puzzle resolves
            }

            // STEP 3: UNCHOOSE / BACKTRACK
            // Revert state, restore the dot marker, and clear masks using inverse mask updates (~mask &)
            board[static_cast<size_t>(r)][static_cast<size_t>(c)] = '.';
            rowsMask[r] &= ~mask;
            colsMask[c] &= ~mask;
            boxesMask[boxIdx] &= ~mask;
        }

        // Return false if no digits from 1 to 9 can satisfy the remaining empty slot constraints
        return false;
    }

public:
    /*
     * INITIAL SYNCHRONIZATION AND ENGINE INITIATION
     * ----------------------------------------------
     * Scans the pre-populated start state board matrix configurations to initialize 
     * bit arrays before starting the recursive search tracker sequence loop.
     */
    void solveSudoku(vector<vector<char>>& board) {
        // Reset local scope bit structures to zero values
        for (int i = 0; i < 9; ++i) {
            rowsMask[i] = 0;
            colsMask[i] = 0;
            boxesMask[i] = 0;
        }

        // Initialize board configuration states
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[static_cast<size_t>(r)][static_cast<size_t>(c)] != '.') {
                    int d = board[static_cast<size_t>(r)][static_cast<size_t>(c)] - '0';
                    int mask = 1 << d;
                    int boxIdx = (r / 3) * 3 + (c / 3);
                    
                    rowsMask[r] |= mask;
                    colsMask[c] |= mask;
                    boxesMask[boxIdx] |= mask;
                }
            }
        }

        // Trigger the depth-first backtracking engine starting from top-left cell (0, 0)
        solve(board, 0, 0);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(9^M) Constant Upper Bound - Where M is the total number of empty cells.
 *                  Since the grid size is fixed at a 9x9 layout matrix dimension, the operational execution 
 *                  has a strict constant worst-case limit. Bitmask conflict detection drops matching overhead to O(1).
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The depth tracking allocations are capped by the fixed maximum 
 *                   matrix dimension footprint depth limit of 81 call stack execution path layers.
 */
int main() {
    vector<vector<char>> board(9, vector<char>(9));
    cout << "Enter the 9x9 Sudoku grid row by row. Use '.' for empty cells and space between entries:\n";
    
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> board[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting recursive constraint satisfaction search..." << endl;
    solver.solveSudoku(board);

    cout << "\nSolved Sudoku Board Configuration Layout Result:\n";
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0 && i != 0) cout << "------+-------+------\n";
        for (int j = 0; j < 9; ++j) {
            if (j % 3 == 0 && j != 0) cout << "| ";
            cout << board[static_cast<size_t>(i)][static_cast<size_t>(j)] << " ";
        }
        cout << "\n";
    }

    return 0;
}