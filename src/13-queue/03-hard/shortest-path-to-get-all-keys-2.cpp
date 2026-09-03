#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BITMASK STATES INTEGRATED INTO MULTI-SOURCE GRID BFS
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum number of moves to collect all keys in a grid.
     * Cells can be empty '.', walls '#', starting point '@', keys 'a'-'f', or locks 'A'-'F'.
     * 
     * 1. 3D STATE REPRESENTATION:
     *    Standard BFS only tracks 2D positions (r, c). Here, the path depends on which keys 
     *    we possess. We represent the search state as a 3D state: (r, c, keysMask), where 
     *    `keysMask` is a bitmask of collected keys.
     *    - If we have keys 'a' and 'b', the mask is binary `11` (decimal 3).
     * 
     * 2. MOVEMENT VALIDATION RULES:
     *    - Empty/Start/Key: Free traversal. When stepping on a key, we update our mask: 
     *      `keysMask | (1 << (key_char - 'a'))`.
     *    - Lock: We can step here only if we have the corresponding key: 
     *      `(keysMask & (1 << (lock_char - 'A')))` is non-zero.
     * 
     * 3. CRITICAL ERROR FIX (START STATE BOUNDS EXCLUSION):
     *    - If the start symbol '@' is completely missing from the input grid, `startR` and 
     *      `startC` remain `-1`.
     *    - Accessing `visited[startR][startC][0]` using out-of-bounds negative indices triggers 
     *      an immediate vector assertion failure (`__n < this->size()`) and core dump.
     *    - We explicitly guard against missing entity markers and verify their ranges 
     *      before initializing the BFS queue and the visited state table.
     */
    int shortestPathAllKeys(const vector<string>& grid) {
        if (grid.empty() || grid[0].empty()) return -1;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());
        
        int startR = -1, startC = -1;
        int maxKeys = 0;

        // Step 1: Scan grid for starting position and total key count
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                char ch = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
                if (ch == '@') {
                    startR = r;
                    startC = c;
                } else if (ch >= 'a' && ch <= 'f') {
                    maxKeys = max(maxKeys, ch - 'a' + 1);
                }
            }
        }

        // Integrity safeguard: If starting position was not found, terminate gracefully
        if (startR == -1 || startC == -1) {
            return -1;
        }

        int targetMask = (1 << maxKeys) - 1;

        // BFS Queue holding: {row, col, keysMask, steps}
        queue<tuple<int, int, int, int>> bfsQueue;
        bfsQueue.push({startR, startC, 0, 0});

        // 3D Visited Vector: rows x cols x (1 << maxKeys)
        vector<vector<vector<bool>>> visited(
            static_cast<size_t>(rows), 
            vector<vector<bool>>(static_cast<size_t>(cols), vector<bool>(static_cast<size_t>(1 << maxKeys), false))
        );
        visited[static_cast<size_t>(startR)][static_cast<size_t>(startC)][0] = true;

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        // Step 2: Traverse state-space
        while (!bfsQueue.empty()) {
            auto [r, c, mask, steps] = bfsQueue.front();
            bfsQueue.pop();

            if (mask == targetMask) {
                return steps;
            }

            for (int d = 0; d < 4; ++d) {
                int nextR = r + dRow[d];
                int nextC = c + dCol[d];

                if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                    char cell = grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)];

                    // Skip wall cells
                    if (cell == '#') continue;

                    int nextMask = mask;

                    // If cell is a key, update key configuration bitmask
                    if (cell >= 'a' && cell <= 'f') {
                        // Ignore keys that are out of our initially calculated maxKeys range
                        int keyBit = cell - 'a';
                        if (keyBit < maxKeys) {
                            nextMask |= (1 << keyBit);
                        }
                    }

                    // If cell is a lock, check if matching key exists in bitmask
                    if (cell >= 'A' && cell <= 'F') {
                        int lockBit = cell - 'A';
                        if (lockBit >= maxKeys || !(mask & (1 << lockBit))) {
                            continue; // Unlocked state missing or lock belongs to an invalid/non-existent key
                        }
                    }

                    // Push unvisited states to the queue
                    if (!visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)][static_cast<size_t>(nextMask)]) {
                        visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)][static_cast<size_t>(nextMask)] = true;
                        bfsQueue.push({nextR, nextC, nextMask, steps + 1});
                    }
                }
            }
        }

        return -1; // Unreachable destination with all keys
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C * 2^K) - Where R and C represent grid dimensions, 
 *                  and K is the maximum count of keys (up to 6).
 *
 * SPACE COMPLEXITY: O(R * C * 2^K) Auxiliary - To hold the 3D state visited tracker table.
 */
int main() {
    cout << "=== Shortest Path to Get All Keys BFS Engine ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int rows, cols;
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) return 1;

    vector<string> grid(static_cast<size_t>(rows));
    cout << "Enter the grid layout sequentially (row by row):\n";
    for (int r = 0; r < rows; ++r) {
        cin >> grid[static_cast<size_t>(r)];
    }

    Solution solver;
    cout << "\nResolving state-space transitions and routing path bounds..." << endl;
    int moves = solver.shortestPathAllKeys(grid);

    if (moves != -1) {
        cout << "Minimum path turns needed to collect all keys: [" << moves << "]\n";
    } else {
        cout << "It is impossible to retrieve all keys. Output: [-1]\n";
    }

    return 0;
}