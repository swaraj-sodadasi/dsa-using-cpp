#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: 3D BREADTH-FIRST SEARCH STATE REPRESENTATION SPACE
     * -------------------------------------------------------------------------------------
     * The objective is to find the shortest path from the top-left corner (0, 0) to the
     * bottom-right corner (M-1, N-1) in a grid, given that you can eliminate up to `k` obstacles.
     * 
     * 1. STATE EXPANSION TO 3D STATE SPACE:
     *    A standard 2D BFS coordinate tracker `(row, col)` is insufficient because returning to a previously 
     *    visited cell with *more* remaining obstacle eliminations can unlock a shorter downstream path. 
     *    Therefore, we expand the state representation into a 3D coordinate framework: `(row, col, remainingK)`.
     * 
     * 2. OPTIMIZATION VISITED TRACKING BUFFER:
     *    We maintain a 3D visited structure initialized to store the maximum remaining obstacle budget 
     *    at each coordinate. If a state transition reaches a cell `(r, c)` with a remaining quota less than 
     *    or equal to a previously cached value for that cell, we prune that branch entirely.
     * 
     * 3. SHORT-CIRCUIT HEURISTIC PRUNING:
     *    If the obstacle budget `k` is large enough to construct a direct Manhattan path to the goal 
     *    (i.e., `k >= rows + cols - 2`), we bypass the entire search space and return `rows + cols - 2` directly.
     * 
     * 4. QUEUE TRAVERSAL SEQUENCE:
     *    We run a layer-by-layer BFS using a FIFO queue tracking `tuple<int, int, int>`. The first time the 
     *    bottom-right node is dequeued, it is guaranteed to be the shortest possible path length.
     */
    int shortestPath(vector<vector<int>>& grid, int k) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        // Step 1: Short-circuit optimization for large obstacle budgets
        if (k >= rows + cols - 2) {
            return rows + cols - 2;
        }

        // 3D visited state representation tracking layer cache initialized to -1
        vector<vector<int>> maxRemainingKAtCell(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols), -1));

        // Queue elements hold: {row, col, remainingK}
        queue<tuple<int, int, int>> stateQueue;

        // Initialize base entry point
        stateQueue.push({0, 0, k});
        maxRemainingKAtCell[0][0] = k;

        int pathsStepCount = 0;

        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        // Step 4: Run layer-by-layer 3D state space evaluation sweeps
        while (!stateQueue.empty()) {
            int levelSize = static_cast<int>(stateQueue.size());

            for (int i = 0; i < levelSize; ++i) {
                auto [currR, currC, currK] = stateQueue.front();
                stateQueue.pop();

                // Target condition check met
                if (currR == rows - 1 && currC == cols - 1) {
                    return pathsStepCount;
                }

                // Scan adjacent neighbors orthogonally
                for (int d = 0; d < 4; ++d) {
                    int nextR = currR + deltaRow[d];
                    int nextC = currC + deltaCol[d];

                    // Grid boundary validation
                    if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                        // Compute obstacle penalty budget impact
                        int nextK = currK - grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)];

                        // Proceed if the state transition maintains a valid, non-negative obstacle quota
                        if (nextK >= 0) {
                            // Check if this path offers a strictly better elimination quota than historical sweeps
                            if (nextK > maxRemainingKAtCell[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)]) {
                                maxRemainingKAtCell[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] = nextK;
                                stateQueue.push({nextR, nextC, nextK});
                            }
                        }
                    }
                }
            }
            pathsStepCount++;
        }

        return -1; // Target coordinates are unreachable under configured parameters
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C * K) - Where R and C represent grid matrix dimension boundaries, 
 *                  and K is the maximum obstacle elimination budget quota. The search space is bounded 
 *                  by the total number of unique states in our 3D visited matrix representation grid.
 *
 * SPACE COMPLEXITY: O(R * C * K) - Dominated by the space requirements of the FIFO state queue 
 *                   and the 3D visited structure caching parameter bounds across cell allocations.
 */
int main() {
    int rows, cols, k;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix cells row by row (0 for Empty space, 1 for Obstacle):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != 0 && 
                grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != 1) {
                cout << "Constraint Error: Element values must be strictly 0 or 1." << endl;
                return 1;
            }
        }
    }

    cout << "Enter maximum allowable obstacle elimination threshold quota (k): ";
    cin >> k;
    if (k < 0) {
        cout << "Invalid parameter. Obstacle budget cannot be negative." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting 3D BFS state space relaxation pipelines..." << endl;
    int resultingSteps = solver.shortestPath(grid, k);

    if (resultingSteps == -1) {
        cout << "Outcome: Target destination unreachable under configured threshold constraints." << endl;
    } else {
        cout << "Minimum path step count units required to reach destination target: " << resultingSteps << endl;
    }

    return 0;
}