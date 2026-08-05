#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * DIRECTIONAL CROSS-RAY MATRIX SCANNING ENGINE
     * --------------------------------------------
     * The objective is to calculate the total number of available captures for a 
     * white Rook ('R') on a standard 8x8 chessboard matrix setup. 
     * 
     * Movement Protocol & Structural Constraints:
     * - The Rook moves horizontally or vertically across the board coordinate lines.
     * - A ray path terminates or captures a target whenever it strikes a white Bishop ('B'), 
     *   a black pawn ('p'), or hits the board structural grid boundary perimeter edges.
     * - If it runs into a black pawn ('p'), it counts as a valid capture event, and the 
     *   ray processing for that direction stops immediately.
     *
     * Algorithmic Strategy:
     * - Step 1: Scan the 8x8 matrix board layout to locate the starting coordinates $(r, c)$ of 'R'.
     * - Step 2: From the Rook's location, launch four independent directional linear cross-ray scans:
     *   - North/Up:    $\Delta r = -1, \Delta c =  0$
     *   - East/Right:  $\Delta r =  0, \Delta c =  1$
     *   - South/Down:  $\Delta r =  1, \Delta c =  0$
     *   - West/Left:   $\Delta r =  0, \Delta c = -1$
     * - Step 3: For each direction, advance step-by-step. If a 'p' is encountered, increment the 
     *   capture tracker and break out of the loop. If a 'B' is encountered, break out immediately 
     *   without capturing.
     */
    int numRookCaptures(vector<vector<char>>& board) {
        int rookRow = -1;
        int rookCol = -1;
        const int BOARD_SIZE = 8;

        // Step 1: Locate the white Rook ('R') on the matrix board coordinate layout
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                if (board[static_cast<size_t>(r)][static_cast<size_t>(c)] == 'R') {
                    rookRow = r;
                    rookCol = c;
                    break;
                }
            }
            if (rookRow != -1) break;
        }

        // If no Rook is detected on the grid, return 0 captures immediately
        if (rookRow == -1) return 0;

        int structuralCapturesCount = 0;

        // Step 2: Define tracking direction vectors for the four primary cross-ray paths
        int delRow[] = {-1, 0, 1, 0};
        int delCol[] = {0, 1, 0, -1};

        // Step 3: Launch independent directional scanning steps
        for (int i = 0; i < 4; ++i) {
            int currentR = rookRow + delRow[i];
            int currentC = rookCol + delCol[i];

            while (currentR >= 0 && currentR < BOARD_SIZE && currentC >= 0 && currentC < BOARD_SIZE) {
                char currentPiece = board[static_cast<size_t>(currentR)][static_cast<size_t>(currentC)];

                // Block encountered: White Bishop blocks the path without contributing a capture
                if (currentPiece == 'B') {
                    break;
                }

                // Target encountered: Black pawn captured successfully. Terminate current ray path.
                if (currentPiece == 'p') {
                    structuralCapturesCount++;
                    break;
                }

                // Advance further along the active vector trajectory ray line
                currentR += delRow[i];
                currentC += delCol[i];
            }
        }

        return structuralCapturesCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Absolute Invariant - The chessboard matrix setup size is locked 
 *                  at an 8x8 cell layout footprint. Finding the Rook scans 64 items, and 
 *                  the 4 cross-ray linear scanning steps trace a maximum of 4 * 7 cell targets, 
 *                  ensuring constant runtime upper bounds.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The cross-ray vector matching logic works entirely 
 *                   in place, requiring only scalar coordinate index tracking registers.
 */
int main() {
    const int GRID_LIMIT = 8;
    vector<vector<char>> board(GRID_LIMIT, vector<char>(GRID_LIMIT));

    cout << "Enter the 8x8 chessboard matrix row by row:\n";
    cout << "('.' for empty cells, 'R' for white Rook, 'B' for white Bishop, 'p' for black Pawn)\n\n";

    for (int r = 0; r < GRID_LIMIT; ++r) {
        cout << "Row [" << r + 1 << "]: ";
        for (int c = 0; c < GRID_LIMIT; ++c) {
            cin >> board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nLaunching cross-ray matrix coordinate tracking scans..." << endl;
    int totalPawnCaptures = solver.numRookCaptures(board);

    cout << "Total available secure pawn capture options matching the Rook path: " << totalPawnCaptures << endl;

    return 0;
}