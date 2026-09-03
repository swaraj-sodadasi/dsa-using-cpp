#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>

using namespace std;

class Solution {
private:
    int rows, cols;

    // Sub-BFS to check if the person can walk from 'start' to 'target' without crossing 'box' position
    bool canReach(const vector<string>& grid, int startR, int startC, int targetR, int targetC, int boxR, int boxC) {
        if (startR == targetR && startC == targetC) return true;

        queue<pair<int, int>> q;
        vector<vector<bool>> visited(static_cast<size_t>(rows), vector<bool>(static_cast<size_t>(cols), false));

        q.push({startR, startC});
        visited[static_cast<size_t>(startR)][static_cast<size_t>(startC)] = true;

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            if (r == targetR && c == targetC) return true;

            for (int i = 0; i < 4; ++i) {
                int nr = r + dRow[i];
                int nc = c + dCol[i];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    // Cannot walk through walls ('#') or the box's current position
                    if (grid[static_cast<size_t>(nr)][static_cast<size_t>(nc)] != '#' && !(nr == boxR && nc == boxC)) {
                        if (!visited[static_cast<size_t>(nr)][static_cast<size_t>(nc)]) {
                            visited[static_cast<size_t>(nr)][static_cast<size_t>(nc)] = true;
                            q.push({nr, nc});
                        }
                    }
                }
            }
        }
        return false;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DUAL-TIERED NESTED STATE BFS SIMULATION
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum box pushes to move a box 'B' to target 'T'.
     * 
     * 1. THE STATE REPRESENTATION:
     *    A simple 2D or 3D BFS is insufficient because the person's position 'S' determines 
     *    which sides of the box are reachable to push it. Therefore, a state is represented 
     *    as a 4-tuple: (boxR, boxC, personR, personC).
     * 
     * 2. DUAL-TIERED BFS FLOW:
     *    - Outer BFS: Tracks the state of the box (boxR, boxC) and the person (personR, personC).
     *      We only increment the step counter when a push occurs.
     *    - Inner BFS: Runs a connectivity search (`canReach`) to verify if the person can 
     *      walk to the push position (directly behind the direction of the push) without 
     *      stepping through the box or walls.
     * 
     * 3. CRITICAL ERROR FIX (OUT-OF-BOUNDS IN-PLACE STATE CHECK):
     *    - If 'S' (Person), 'B' (Box), or 'T' (Target) are not found in the grid or if their 
     *      initial locations are out of bounds, calculating `startStateIdx` will cause 
     *      an immediate assertion failure (`__n < this->size()`) or a segmentation fault.
     *    - We explicitly guard against missing entity markers and verify their ranges 
     *      before accessing the `visited` flat vector.
     */
    int minPushBox(vector<string>& grid) {
        rows = static_cast<int>(grid.size());
        cols = static_cast<int>(grid[0].size());

        int boxR = -1, boxC = -1;
        int personR = -1, personC = -1;
        int targetR = -1, targetC = -1;

        // Step 1: Scan grid coordinates
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                char ch = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
                if (ch == 'B') {
                    boxR = r; boxC = c;
                } else if (ch == 'S') {
                    personR = r; personC = c;
                } else if (ch == 'T') {
                    targetR = r; targetC = c;
                }
            }
        }

        // Integrity safeguard: If required elements are missing, terminate gracefully
        if (boxR == -1 || boxC == -1 || personR == -1 || personC == -1 || targetR == -1 || targetC == -1) {
            return -1;
        }

        // State: {boxR, boxC, personR, personC, pushes}
        queue<tuple<int, int, int, int, int>> q;
        q.push({boxR, boxC, personR, personC, 0});

        // Visited tracker flat array representation for O(1) checks
        // Size: rows * cols * rows * cols
        size_t totalStates = static_cast<size_t>(rows * cols * rows * cols);
        vector<bool> visited(totalStates, false);
        
        int startStateIdx = (boxR * cols + boxC) * (rows * cols) + (personR * cols + personC);
        
        // Out-of-bounds safety check for initial states
        if (startStateIdx >= 0 && static_cast<size_t>(startStateIdx) < totalStates) {
            visited[static_cast<size_t>(startStateIdx)] = true;
        } else {
            return -1;
        }

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        // Step 2: Run Outer BFS over box push transitions
        while (!q.empty()) {
            auto [bR, bC, pR, pC, pushes] = q.front();
            q.pop();

            if (bR == targetR && bC == targetC) {
                return pushes;
            }

            for (int i = 0; i < 4; ++i) {
                // To push the box in direction (dRow[i], dCol[i]), the person must stand at the opposite side
                int pushFromR = bR - dRow[i];
                int pushFromC = bC - dCol[i];
                int pushToR = bR + dRow[i];
                int pushToC = bC + dCol[i];

                // Check boundaries of the target destination and push point
                if (pushToR >= 0 && pushToR < rows && pushToC >= 0 && pushToC < cols &&
                    pushFromR >= 0 && pushFromR < rows && pushFromC >= 0 && pushFromC < cols) {
                    
                    if (grid[static_cast<size_t>(pushToR)][static_cast<size_t>(pushToC)] != '#' && 
                        grid[static_cast<size_t>(pushFromR)][static_cast<size_t>(pushFromC)] != '#') {
                        
                        int nextStateIdx = (pushToR * cols + pushToC) * (rows * cols) + (bR * cols + bC);
                        
                        if (nextStateIdx >= 0 && static_cast<size_t>(nextStateIdx) < totalStates) {
                            if (!visited[static_cast<size_t>(nextStateIdx)]) {
                                // Sub-BFS: Can the person walk to 'pushFrom' state?
                                if (canReach(grid, pR, pC, pushFromR, pushFromC, bR, bC)) {
                                    visited[static_cast<size_t>(nextStateIdx)] = true;
                                    q.push({pushToR, pushToC, bR, bC, pushes + 1});
                                }
                            }
                        }
                    }
                }
            }
        }

        return -1; // Target is unreachable
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O((R * C)^2) - Where R and C represent grid dimensions.
 *                  There are at most R * C * R * C unique states. For each state, 
 *                  we run a path connectivity check of size O(R * C).
 *
 * SPACE COMPLEXITY: O((R * C)^2) Auxiliary - To hold the flat state visited array.
 */
int main() {
    cout << "=== Box Pusher Dual-Tiered BFS Engine ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int r, c;
    if (!(cin >> r >> c) || r <= 0 || c <= 0) return 1;

    vector<string> grid(static_cast<size_t>(r));
    cout << "Enter the grid layout sequentially (S=Start, B=Box, T=Target, .=Empty, #=Wall):\n";
    for (int i = 0; i < r; ++i) {
        cin >> grid[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nAnalyzing state-space transitions and routing path bounds..." << endl;
    int pushes = solver.minPushBox(grid);

    if (pushes != -1) {
        cout << "Minimum box pushes needed to reach target: [" << pushes << "]\n";
    } else {
        cout << "Target is unreachable. Output: [-1]\n";
    }

    return 0;
}