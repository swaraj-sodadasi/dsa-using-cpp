#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SHORTEST PATH WITH OBSTACLE ELIMINATION (3D BFS STATE SPACE)
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum steps to walk from (0,0) to (R-1, C-1) given 
     * that we can eliminate at most `k` obstacles.
     * 
     * 1. 3D STATE REPRESENTATION:
     *    Standard BFS tracks only 2D positions (r, c). Because we have a budget of obstacle 
     *    destructions, the exact same coordinate can be reached with different numbers of 
     *    remaining destructions. We model this using a 3D state-space: (row, col, remaining_k).
     * 
     * 2. MOVEMENT TRANSITIONS:
     *    From an active state (r, c, remK), we inspect its 4-directional neighbors:
     *      - Empty space (0): Move freely to (nextR, nextC, remK).
     *      - Obstacle (1): Move only if remK > 0, decrementing the budget: (nextR, nextC, remK - 1).
     * 
     * 3. VISITED TRACKING & OPTIMIZATION:
     *    We maintain a 2D table `visited[r][c]` storing the maximum remaining elimination 
     *    budget seen so far for that coordinate. If we reach an already-visited cell with 
     *    equal or fewer remaining obstacle eliminations than recorded, we prune that path 
     *    as it is strictly suboptimal.
     */
    int shortestPath(vector<vector<int>>& grid, int k) {
        if (grid.empty() || grid[0].empty()) return -1;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        // Shortcut: If we can clear more obstacles than the Manhattan distance, we can go straight
        if (k >= rows + cols - 2) {
            return rows + cols - 2;
        }

        // BFS Queue holding: {row, col, remaining_k, steps}
        queue<tuple<int, int, int, int>> bfsQueue;
        bfsQueue.push({0, 0, k, 0});

        // visited[r][c] stores the maximum remaining k seen at (r, c)
        vector<vector<int>> visited(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols), -1));
        visited[0][0] = k;

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        while (!bfsQueue.empty()) {
            auto [r, c, remK, steps] = bfsQueue.front();
            bfsQueue.pop();

            // Destination reached
            if (r == rows - 1 && c == cols - 1) {
                return steps;
            }

            for (int d = 0; d < 4; ++d) {
                int nextR = r + dRow[d];
                int nextC = c + dCol[d];

                if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                    int nextK = remK - grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)];

                    // If we have enough elimination budget and this state yields a better remK
                    if (nextK >= 0 && nextK > visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)]) {
                        visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] = nextK;
                        bfsQueue.push({nextR, nextC, nextK, steps + 1});
                    }
                }
            }
        }

        return -1; // Destination is unreachable
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C * K) - Where R and C represent grid dimensions, 
 *                  and K is the maximum obstacles we can eliminate. 
 *                  Each cell can be visited with at most K different budgets.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - To hold the maximum remaining k tracker table, 
 *                   and queue allocations which scale with grid boundaries.
 */
int main() {
    cout << "=== Obstacle Elimination 3D BFS Engine ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int rows, cols;
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) return 1;

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter the grid layout sequentially (0 = empty, 1 = obstacle):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    cout << "Enter the maximum obstacle elimination budget (k): ";
    int k;
    cin >> k;

    Solution solver;
    cout << "\nResolving state-space transitions and routing path bounds..." << endl;
    int steps = solver.shortestPath(grid, k);

    if (steps != -1) {
        cout << "Minimum path turns needed to reach target: [" << steps << "]\n";
    } else {
        cout << "Target is unreachable under given elimination constraints. Output: [-1]\n";
    }

    return 0;
}