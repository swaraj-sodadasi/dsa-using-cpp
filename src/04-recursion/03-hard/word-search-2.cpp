#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
 * TRIE NODE INFRASTRUCTURE CONFIGURATION
 * --------------------------------------
 * A standard matrix DFS would require searching the grid separately for each word,
 * leading to massive redundant path traversals.
 * To optimize this, we index all target words inside a prefix tree (Trie).
 * This allows us to search the grid for all words simultaneously.
 *
 * Struct Attributes:
 *  - children: Pointers to subsequent character layers (handles lowercase a-z).
 *  - word: Stores the complete matching string at the terminal node (empty if non-leaf).
 */
struct TrieNode {
    TrieNode* children[26];
    string word;

    TrieNode() {
        for (int i = 0; i < 26; ++i) {
            children[i] = nullptr;
        }
        word = "";
    }
};

class Solution {
private:
    vector<string> foundWords;
    int rows, cols;
    
    // Direction offsets for traversing neighbors: Up, Down, Left, Right
    const int dRows[4] = {-1, 1, 0, 0};
    const int dCols[4] = {0, 0, -1, 1};

    // Helper utility to insert target strings into the shared prefix layout structure
    void insertWord(TrieNode* root, const string& word) {
        TrieNode* curr = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!curr->children[idx]) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
        }
        curr->word = word; // Storing the full word string at the terminal leaf
    }

    /*
     * RECURSIVE TRIE-DRIVEN MATRIX DFS BACKTRACKING ENGINE
     * ----------------------------------------------------
     * Traverses the grid while simultaneously moving through the matching Trie path branches.
     *
     * State Parameters:
     *  - board: The 2D character matrix grid under evaluation.
     *  - r: Current row position coordinate tracker.
     *  - c: Current column position coordinate tracker.
     *  - node: The active tracking node pointer inside the prefix tree state.
     */
    void dfs(vector<vector<char>>& board, int r, int c, TrieNode* node) {
        char ch = board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        int letterIdx = ch - 'a';

        // PRUNING CONSTRAINTS: Check if letter branch path doesn't exist in the prefix tree
        if (letterIdx < 0 || letterIdx >= 26 || !node->children[letterIdx]) {
            return;
        }

        node = node->children[letterIdx];

        // SUCCESS CONVERGENCE: If a valid string word layout is populated at this step node
        if (!node->word.empty()) {
            foundWords.push_back(node->word);
            node->word = ""; // DE-DUPLICATION: Clear the node variable to avoid storing identical copies
        }

        // STEP 1: CHOOSE (In-place Grid Masking to prevent cell path reuse)
        board[static_cast<size_t>(r)][static_cast<size_t>(c)] = '#';

        // STEP 2: RECURSE (Spin off exploration into all 4 cardinal directions)
        for (int i = 0; i < 4; ++i) {
            int nextR = r + dRows[i];
            int nextC = c + dCols[i];

            // Boundary threshold validation filtering step
            if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                // Only descend if the target destination cell isn't already masked ('#')
                if (board[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] != '#') {
                    dfs(board, nextR, nextC, node);
                }
            }
        }

        // STEP 3: UNCHOOSE / BACKTRACK (Restore original character layout value back to the grid cell)
        board[static_cast<size_t>(r)][static_cast<size_t>(c)] = ch;
    }

    // Dynamic cleaning utility to clear dynamically allocated tree memory from the system heap
    void freeTrie(TrieNode* root) {
        if (!root) return;
        for (int i = 0; i < 26; ++i) {
            if (root->children[i]) {
                freeTrie(root->children[i]);
            }
        }
        delete root;
    }

public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        foundWords.clear();
        if (board.empty() || board[0].empty() || words.empty()) {
            return foundWords;
        }

        rows = static_cast<int>(board.size());
        cols = static_cast<int>(board[0].size());

        // Allocate root memory layer for our dictionary prefix node structure
        TrieNode* root = new TrieNode();
        for (const string& word : words) {
            insertWord(root, word);
        }

        // Grid-wide anchoring search initiation loop sweep
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                dfs(board, r, c, root);
            }
        }

        // Clear dynamic memory allocations
        freeTrie(root);

        return foundWords;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(W * L + R * C * 4^L) - Where W is the number of words, L is the maximum string length, 
 *                  and R, C match board dimensions. Building the Trie infrastructure consumes O(W * L). 
 *                  The matrix search tree branches out in 4 directions up to length L for each grid cell anchor.
 *
 * SPACE COMPLEXITY: O(W * L) Auxiliary - Storage footprint allocated to construct the structural prefix 
 *                   tree layout on the heap, combined with recursive call stack frames bounded by O(L).
 */
int main() {
    int r, c;
    cout << "Enter the grid dimensions (Rows followed by Columns): ";
    if (!(cin >> r >> c) || r <= 0 || c <= 0) {
        cout << "Invalid grid dimensions configuration parameter settings." << endl;
        return 1;
    }

    vector<vector<char>> board(static_cast<size_t>(r), vector<char>(static_cast<size_t>(c)));
    cout << "Enter the matrix characters row by row (lowercase letters separated by spaces within each row):\n";
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            cin >> board[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    int wCount;
    cout << "Enter the total number of dictionary words to search for: ";
    if (!(cin >> wCount) || wCount <= 0) {
        cout << "Invalid entry bounds parameters." << endl;
        return 1;
    }

    vector<string> words(static_cast<size_t>(wCount));
    cout << "Enter the strings list separated by spaces or newlines:\n";
    for (int i = 0; i < wCount; ++i) {
        cin >> words[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting Trie-accelerated multi-string backtracking search..." << endl;
    vector<string> result = solver.findWords(board, words);

    cout << "\nWords identified within the grid layout structure:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "\"" << result[i] << "\"";
        if (i + 1 < result.size()) cout << ", ";
    }
    cout << " ]\n";
    cout << "Total unique matches captured: " << result.size() << endl;

    return 0;
}