#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
private:
    /*
     * DEPTH-FIRST SEARCH PATH MASKING ROUTINE WITH BACKTRACKING
     * ---------------------------------------------------------
     * Recursively walks the 2D matrix structure step-by-step to match characters 
     * sequentially along orthogonal trajectories.
     */
    bool discoverPathDFS(vector<vector<char>>& board, const string& word, int r, int c, size_t matchIndex, int rows, int cols) {
        // Base Case: All target characters in the sequence matched successfully
        if (matchIndex == word.length()) {
            return true;
        }

        // Boundary constraint check: evaluate structural coordinate limitations and structural parity
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[static_cast<size_t>(r)][static_cast<size_t>(c)] != word[matchIndex]) {
            return false;
        }

        // Capture original character value state before temporary structural masking
        char cachedChar = board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        
        // Step 2: Mask the cell using a dummy character proxy value to prevent duplicate self-intersection
        board[static_cast<size_t>(r)][static_cast<size_t>(c)] = '#';

        // Orthogonal traversal offsets (Up, Right, Down, Left)
        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        // Step 3: Explore all four potential orthogonal target paths
        for (int i = 0; i < 4; ++i) {
            int neighborRow = r + deltaRow[i];
            int neighborCol = c + deltaCol[i];

            if (discoverPathDFS(board, word, neighborRow, neighborCol, matchIndex + 1, rows, cols)) {
                return true; // Match path verified, propagate return flag upwards
            }
        }

        // Step 4: Backtracking step — restore initial cached state parameter value
        board[static_cast<size_t>(r)][static_cast<size_t>(c)] = cachedChar;

        return false;
    }

public:
    /*
     * MATRIX DFS PATH MASKING WITH GRID BACKTRACKING ENGINE
     * -----------------------------------------------------
     * The objective is to determine if a target word string can be constructed 
     * from letters of sequentially adjacent cells in a 2D grid matrix.
     * Cells cannot be recycled or revisited within a single word assembly loop.
     *
     * Algorithmic Strategy:
     * - This space traversal problem maps to finding a simple path in an unweighted grid graph structure.
     * - We scan the matrix systematically cell by cell. When a cell matches the initial head index character 
     *   of our word (`word[0]`), we trigger a Depth-First Search (DFS) validation routine from that cell.
     * - To respect the single-use rule per path without allocating a large O(R * C) boolean tracking 
     *   visited matrix, we use **in-place character masking**. We overwrite the current character with a special 
     *   marker ('#') before descending into recursion, and restore it (backtrack) when the path branch finishes.
     *
     * Execution Protocol:
     * - Step 1: Scan all matrix grid cells to locate the starting character entry index matches.
     * - Step 2: Upon discovering a head character cell, invoke `discoverPathDFS`.
     * - Step 3: Inside the DFS routine, mask the cell in-place, check all 4 orthogonal neighbor states, 
     *           and recursively look for the next character index.
     * - Step 4: If all downstream branches return false, backtrack by restoring the original letter 
     *           to allow other independent starting positions to reuse the cell.
     */
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty() || word.empty()) return false;

        int rows = static_cast<int>(board.size());
        int cols = static_cast<int>(board[0].size());

        // Step 1: Scan grid coordinates systematically for potential entry points
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[static_cast<size_t>(r)][static_cast<size_t>(c)] == word[0]) {
                    // Trigger DFS paths tracking from valid candidate head coordinates
                    if (discoverPathDFS(board, word, r, c, 0, rows, cols)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C * 3^L) - Where R and C represent grid matrix dimension footprints, 
 *                  and L represents the absolute character length of the target string word parameter. 
 *                  The outer search framework sweeps all R * C cells. The DFS exploration path branches 
 *                  in 3 directions at each step (excluding the cell we just arrived from), yielding 
 *                  an exponential worst-case upper bound scaled to the length of the string.
 *
 * SPACE COMPLEXITY: O(L) Auxiliary - Dynamic tracking allocations match the depth of the recursive 
 *                   DFS call stack, which scales linearly with the maximum character depth size L.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural grid parameter options configured." << endl;
        return 1;
    }

    vector<vector<char>> board(static_cast<size_t>(rows), vector<char>(static_cast<size_t>(cols)));
    cout << "Enter board single-character element values row by row:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    string targetWord;
    cout << "\nEnter the target string search word to match: ";
    cin >> targetWord;

    Solution solver;
    cout << "\nExecuting grid masking and DFS backtracking paths checks..." << endl;
    bool pathExists = solver.exist(board, targetWord);

    cout << "Search Validation Outcome: " 
         << (pathExists ? "TRUE (Word exists in grid)" : "FALSE (Word does not exist in grid)") << endl;

    return 0;
}