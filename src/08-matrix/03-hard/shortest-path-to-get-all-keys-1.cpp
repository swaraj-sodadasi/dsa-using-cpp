#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BITMASK STATES INTEGRATED INTO MULTI-SOURCE GRID BFS
     * -------------------------------------------------------------------------------------
     * The objective is to find the shortest path from the starting position '@' to collect
     * all keys in a grid matrix. Lowercase letters ('a'-'f') represent keys, and uppercase 
     * letters ('A'-'F') represent matching locked doors that require the corresponding key.
     * 
     * 1. 3D STATE SPACE EXPANSION VIA BITMASKS:
     *    A standard 2D visited array `(row, col)` is insufficient because we frequently need 
     *    to backtrack and cross the same cell multiple times after acquiring new keys. We expand 
     *    the state tracking space into 3 dimensions: `(row, col, keyBitmask)`.
     *    Since there are at most 6 keys, the maximum key bitmask value is 2^6 - 1 = 63. We use a 
     *    compact integer bitmask where the i-th bit set to 1 signifies ownership of the i-th key.
     * 
     * 2. COMBINATORIAL DOOR VERIFICATION:
     *    When walking into a door cell, we check if we hold the correct key using a fast bitwise 
     *    shift and AND operation: `(keyBitmask & (1 << (grid[r][c] - 'A'))生存)`. If the bit evaluates 
     *    to 0, the path branch is blocked, and we prune it.
     * 
     * 3. THE QUEUE STEP EVOLUTION:
     *    We execute a standard layer-by-layer Breadth-First Search (BFS) using a FIFO queue tracking 
     *    structures containing `{row, col, keyBitmask}`. The first time the queue encounters a 
     *    `keyBitmask == (1 << totalKeysCount) - 1`, the current path count is guaranteed to be the 
     *    absolute shortest path distance.
     */
    int shortestPathAllKeys(vector<string>& grid) {
        if (grid.empty() || grid[0].empty()) return -1;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        int startRow = 0, startCol = 0;
        int totalKeysCount = 0;

        // Step 1: Scan the grid to calculate total keys and extract starting coordinate entry point
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                char cell = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
                if (cell == '@') {
                    startRow = r;
                    startCol = c;
                } else if (cell >= 'a' && cell <= 'f') {
                    totalKeysCount = max(totalKeysCount, cell - 'a' + 1);
                }
            }
        }

        int finalTargetBitmask = (1 << totalKeysCount) - 1;

        // 3D visited state representation tracking: [row][col][keyBitmask]
        // Size: rows x cols x 64 (since max keys = 6, 2^6 = 64)
        vector<vector<vector<bool>>> visited(
            static_cast<size_t>(rows), 
            vector<vector<bool>>(static_cast<size_t>(cols), vector<bool>(64, false))
        );

        // Queue elements hold: {row, col, keyBitmask}
        queue<pair<pair<int, int>, int>> stateQueue;

        stateQueue.push({{startRow, startCol}, 0});
        visited[static_cast<size_t>(startRow)][static_cast<size_t>(startCol)][0] = true;

        int structuralStepCount = 0;

        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        // Step 3: Run layer-by-layer BFS transitions across the state space matrix
        while (!stateQueue.empty()) {
            int currentLevelSize = static_cast<int>(stateQueue.size());

            for (int i = 0; i < currentLevelSize; ++i) {
                auto [coords, currMask] = stateQueue.front();
                auto [currR, currC] = coords;
                stateQueue.pop();

                // Target state condition fully satisfied
                if (currMask == finalTargetBitmask) {
                    return structuralStepCount;
                }

                // Check all 4 orthogonal directions
                for (int d = 0; d < 4; ++d) {
                    int nextR = currR + deltaRow[d];
                    int nextC = currC + deltaCol[d];
                    int nextMask = currMask;

                    // Grid boundary filter check
                    if (nextR < 0 || nextR >= rows || nextC < 0 || nextC >= cols) continue;

                    char cellChar = grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)];

                    // Wall collision obstacle check
                    if (cellChar == '#') continue;

                    // Locked door constraint check
                    if (cellChar >= 'A' && cellChar <= 'F') {
                        int lockIndex = cellChar - 'A';
                        if (!(nextMask & (1 << lockIndex))) {
                            continue; // Key bit missing, path trajectory rejected
                        }
                    }

                    // Key inventory collection update
                    if (cellChar >= 'a' && cellChar <= 'f') {
                        int keyIndex = cellChar - 'a';
                        nextMask |= (1 << keyIndex);
                    }

                    // State expansion push to queue if path scenario is unexplored
                    if (!visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)][static_cast<size_t>(nextMask)]) {
                        visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)][static_cast<size_t>(nextMask)] = true;
                        stateQueue.push({{nextR, nextC}, nextMask});
                    }
                }
            }
            structuralStepCount++;
        }

        return -1; // Unreachable destination target or missing key components
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C * 2^K) - Where R and C represent matrix dimensions, and K represents 
 *                  the absolute number of keys present in the grid layout map (K <= 6). The search space 
 *                  is bounded by the total number of unique states in the 3D tracking lookup grid array.
 *
 * SPACE COMPLEXITY: O(R * C * 2^K) - Scales linearly to accommodate the 3D visited matrix space allocation 
 *                   and the structural queue element trajectories.
 */
int main() {
    int rows;
    cout << "Enter the count size total of rows for the matrix grid: ";
    if (!(cin >> rows) || rows <= 0) {
        cout << "Invalid boundary grid configuration options." << endl;
        return 1;
    }

    vector<string> grid(static_cast<size_t>(rows));
    cout << "Enter matrix strings row by row ('.'=Floor, '#'=Wall, '@'=Start, 'a'-'f'=Keys, 'A'-'F'=Doors):\n";
    for (int r = 0; r < rows; ++r) {
        cin >> grid[static_cast<size_t>(r)];
    }

    Solution solver;
    cout << "\nExecuting bitmask state grid path search routines..." << endl;
    int optimalStepsResult = solver.shortestPathAllKeys(grid);

    if (optimalStepsResult == -1) {
        cout << "Outcome: Impossible to acquire all keys under configured parameters. Result = -1" << endl;
    } else {
        cout << "Minimum dynamic path steps required to gather all keys: " << optimalStepsResult << endl;
    }

    return 0;
}