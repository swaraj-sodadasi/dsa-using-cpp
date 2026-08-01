#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
private:
    vector<vector<string>> solutions;
    
    /*
     * BITMASK TRACKING MECHANISMS (O(1) CONSTANT-TIME CONFLICT CHECKS)
     * ----------------------------------------------------------------
     * Traditional conflict tracking requires scanning rows, columns, and diagonals,
     * which runs in O(N) time per queen placement. 
     * By utilizing bitmasks (integers), we track threats as flipped bits (1 = attacked, 0 = safe).
     *
     * 1. colsMask: Tracks columns. Column 'col' sets the col-th bit.
     * 2. diagMask: Tracks main diagonals (\). Along these, (row - col) is constant.
     *    To prevent negative indices, we offset it: idx = row - col + n - 1.
     * 3. antiDiagMask: Tracks anti-diagonals (/). Along these, (row + col) is constant.
     */
    int colsMask = 0;
    int diagMask = 0;
    int antiDiagMask = 0;

    /*
     * RECURSIVE STATE BACKTRACKING CORE ENGINE
     * ----------------------------------------
     * Explores placement combinations by proceeding systematically row by row.
     *
     * Parameters:
     *  - row: The active row index we are trying to place a queen in.
     *  - n: Total number of queens / size dimensions of the board.
     *  - currentBoard: The character matrix representation of the board state.
     */
    void solve(int row, int n, vector<string>& currentBoard) {
        // BASE CASE: SUCCESS CONVERGENCE REACHED
        // If the row pointer equals n, it means we have successfully placed a queen 
        // in every row from 0 to n-1 without conflicts. Save this board configuration.
        if (row == n) {
            solutions.push_back(currentBoard);
            return;
        }

        // Iterate through all column choices for the active row
        for (int col = 0; col < n; ++col) {
            // Calculate diagonal translation bit indices
            int diagIdx = row - col + n - 1;
            int antiDiagIdx = row + col;

            // BITWISE CONFLICT CHECKING
            // Using bitwise AND (&) combined with left-shifts (1 << shift), we query if
            // the targeted column or diagonal bit trackers are already set to 1.
            // If any check evaluates to true, the cell is under attack -> Prune branch.
            if ((colsMask & (1 << col)) || 
                (diagMask & (1 << diagIdx)) || 
                (antiDiagMask & (1 << antiDiagIdx))) {
                continue; 
            }

            // STEP 1: CHOOSE 
            // Place the Queen marker and toggle the attack vector bits using bitwise OR (|=)
            currentBoard[static_cast<size_t>(row)][static_cast<size_t>(col)] = 'Q';
            colsMask |= (1 << col);
            diagMask |= (1 << diagIdx);
            antiDiagMask |= (1 << antiDiagIdx);

            // STEP 2: RECURSE 
            // Move down to look for valid options in the next consecutive row
            solve(row + 1, n, currentBoard);

            // STEP 3: UNCHOOSE / BACKTRACK
            // Revert the state changes by restoring the empty marker ('.') and resetting 
            // the attack bits back to 0 using a bitwise invert-and-AND operator combination (&= ~).
            currentBoard[static_cast<size_t>(row)][static_cast<size_t>(col)] = '.';
            colsMask &= ~(1 << col);
            diagMask &= ~(1 << diagIdx);
            antiDiagMask &= ~(1 << antiDiagIdx);
        }
    }

public:
    vector<vector<string>> solveNQueens(int n) {
        solutions.clear();
        colsMask = 0;
        diagMask = 0;
        antiDiagMask = 0;

        // Initialize an empty chessboard layout configuration
        vector<string> currentBoard(static_cast<size_t>(n), string(static_cast<size_t>(n), '.'));
        
        solve(0, n, currentBoard);
        return solutions;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N!) - The upper bound search complexity is limited by N placements in the 
 *                  first row, at most N-2 configurations in the second, etc. The rapid O(1) bitwise 
 *                  pruning forces invalid branches to collapse instantly.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The system allocation constraints are bound linearly 
 *                   by the max depth of the recursive execution stack, which peaks at N frames.
 */
int main() {
    int n;
    cout << "Enter the chessboard size configuration parameter (N): ";
    if (!(cin >> n) || n <= 0 || n > 16) {
        cout << "Invalid sizing parameters. Please input a number between 1 and 16." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting recursive grid placement with bit-vector conflict checking..." << endl;
    vector<vector<string>> results = solver.solveNQueens(n);

    cout << "\nTotal complete layout distinct solutions found: " << results.size() << "\n";
    
    int maxPrint = min(static_cast<int>(results.size()), 3);
    if (results.size() > 0) {
        cout << "Displaying the first " << maxPrint << " solution layouts:\n";
        for (int i = 0; i < maxPrint; ++i) {
            cout << "Solution Variant #" << i + 1 << ":\n";
            for (const string& row : results[static_cast<size_t>(i)]) {
                cout << "  " << row << "\n";
            }
            cout << "\n";
        }
    }

    return 0;
}