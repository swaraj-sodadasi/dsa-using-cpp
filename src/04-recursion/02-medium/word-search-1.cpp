#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
CORE LOGIC (MATRIX DFS WITH IN-PLACE GRID VISITED MASKING):
- The objective is to check if a target word exists in a 2D grid of characters. 
  The word can be constructed from letters of sequentially adjacent cells (horizontally or vertically). 
  The same letter cell may not be used more than once in a single path sequence.
- **In-place Visited Masking Strategy**:
  - Instead of creating an explicit auxiliary 2D boolean grid `visited[R][C]` which incurs $O(R \times C)$ extra space, 
    we mask the cell *directly inside the grid* during the active search depth window.
  - Before transitioning to adjacent cells, we temporarily cache the current character and overwrite the cell with a special 
    masking character (e.g., `'#'`). This ensures that subsequent recursive branches in the current path cannot reuse this cell.
  - After the deeper recursive calls finish exploring, we restore the original character back into the grid (Backtracking).
- **State Transition Strategy**:
  - We loop through every cell `(r, c)` in the grid as a potential starting anchor.
  - The recursive helper function `dfs(r, c, wordIndex)` validates state parameters:
    - **Base Success Case**: If `wordIndex == word.length()`, the entire word string has been successfully matched $\rightarrow$ return `true`.
    - **Boundary / Pruning Checks**: If the cell `(r, c)` is out of bounds, or if `board[r][c] != word[wordIndex]`, the path is invalid $\rightarrow$ return `false`.
    - **Exploration**: If the characters match, mask the grid slot, spin off 4 recursive sub-calls to check neighbors (Up, Down, Left, Right), and then unmask the slot before returning.

TIME COMPLEXITY: O(R * C * 4^L) - Where R and C are rows and columns of the board, and L is the length of the target word. 
                 At each step, the search tree branches in up to 4 directions.
SPACE COMPLEXITY: O(L) auxiliary - Maximum recursive call stack footprint is bounded by the length of the word string L.
*/

class Solution {
private:
    int rows, cols;
    // Direction offsets for traversing neighbors: Up, Down, Left, Right
    const vector<int> dRows = {-1, 1, 0, 0};
    const vector<int> dCols = {0, 0, -1, 1};

    bool dfs(vector<vector<char>>& board, const string& word, int r, int c, size_t wordIndex) {
        // Base Case: The entire target word length has been matched
        if (wordIndex == word.length()) {
            return true;
        }

        // Boundary and alignment validation checks
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[static_cast<size_t>(r)][static_cast<size_t>(c)] != word[wordIndex]) {
            return false;
        }

        // In-place Masking: Cache current value and overwrite to prevent cell reuse
        char originalChar = board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        board[static_cast<size_t>(r)][static_cast<size_t>(c)] = '#'; 

        // Branch out in all 4 cardinal directions
        for (int i = 0; i < 4; ++i) {
            int nextR = r + dRows[i];
            int nextC = c + dCols[i];
            
            if (dfs(board, word, nextR, nextC, wordIndex + 1)) {
                return true; // Terminate early if the word is found
            }
        }

        // Backtracking: Restore the original value back into the grid cell
        board[static_cast<size_t>(r)][static_cast<size_t>(c)] = originalChar;

        return false;
    }

public:
    bool exist(vector<vector<char>>& board, const string& word) {
        if (board.empty() || board[0].empty() || word.empty()) {
            return false;
        }

        rows = static_cast<int>(board.size());
        cols = static_cast<int>(board[0].size());

        // Scan the matrix to find a matching starting character anchor point
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (dfs(board, word, r, c, 0)) {
                    return true;
                }
            }
        }

        return false;
    }
};

int main() {
    int r, c;
    cout << "Enter the grid dimensions (Rows followed by Columns): ";
    if (!(cin >> r >> c) || r <= 0 || c <= 0) {
        cout << "Invalid grid dimensions parameter constraints." << endl;
        return 1;
    }

    vector<vector<char>> board(static_cast<size_t>(r), vector<char>(static_cast<size_t>(c)));
    cout << "Enter the matrix characters row by row (separated by spaces within each row):\n";
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            cin >> board[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    string word;
    cout << "Enter the target search word string: ";
    cin >> word;

    Solution solver;
    cout << "\nExecuting grid DFS with in-place visited masking search..." << endl;
    
    if (solver.exist(board, word)) {
        cout << "Result: The word \"" << word << "\" exists within the grid structure." << endl;
    } else {
        cout << "Result: The word \"" << word << "\" was NOT located within the grid structure." << endl;
    }

    return 0;
}