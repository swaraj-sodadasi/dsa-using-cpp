#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-SOURCE BREADTH-FIRST SEARCH LEVEL STATE TRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum number of minutes that must elapse until no cell 
     * has a fresh orange. If this is impossible, we return -1.
     * 
     * 1. MULTI-SOURCE INITIALIZATION:
     *    Instead of running a BFS from each rotten orange sequentially, we push *all* initially 
     *    rotten oranges (grid[i][j] == 2) into our FIFO queue. This allows us to simulate the 
     *    simultaneous (parallel) spread of rot across the grid. We also count the total number 
     *    of fresh oranges (grid[i][j] == 1) to determine when the spread process is complete.
     * 
     * 2. LEVEL-BY-LEVEL EXPLORATION:
     *    We process the queue in rounds (representing minutes). In each round:
     *      - We capture the current size of the queue (`levelSize`).
     *      - For each rotten orange processed in this batch, we inspect its 4-directional 
     *        neighbors (up, down, left, right).
     *      - If we encounter a fresh orange, we turn it rotten (grid[r][c] = 2), decrement the 
     *        fresh orange counter, and enqueue its coordinates.
     * 
     * 3. TERMINAL EVALUATION:
     *    If we successfully turn fresh oranges rotten during a round, we increment our elapsed 
     *    minutes counter. Once the queue is empty, if any fresh oranges remain, we return -1. 
     *    Otherwise, we return the total elapsed minutes.
     */
    int orangesRotting(vector<vector<int>>& grid) {
        if (grid.empty()) return 0;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());
        queue<pair<int, int>> rottenQueue;
        int freshCount = 0;

        // Step 1: Initialize states and push initial rotten sources to the queue
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 2) {
                    rottenQueue.push({r, c});
                } else if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    freshCount++;
                }
            }
        }

        // If there are no fresh oranges initially, 0 minutes are needed
        if (freshCount == 0) return 0;

        int minutesElapsed = 0;
        // Direction vectors for 4-directional cell movements
        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        // Step 2: Multi-source BFS traversal loop
        while (!rottenQueue.empty() && freshCount > 0) {
            int levelSize = static_cast<int>(rottenQueue.size());
            bool turnedRottenThisMinute = false;

            for (int i = 0; i < levelSize; ++i) {
                auto [r, c] = rottenQueue.front();
                rottenQueue.pop();

                for (int d = 0; d < 4; ++d) {
                    int neighborRow = r + dRow[d];
                    int neighborCol = c + dCol[d];

                    // Check grid boundary conditions and identify fresh oranges
                    if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                        if (grid[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] == 1) {
                            // Turn fresh orange rotten
                            grid[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] = 2;
                            freshCount--;
                            rottenQueue.push({neighborRow, neighborCol});
                            turnedRottenThisMinute = true;
                        }
                    }
                }
            }

            if (turnedRottenThisMinute) {
                minutesElapsed++;
            }
        }

        return (freshCount == 0) ? minutesElapsed : -1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents rows and C represents columns of the grid.
 *                  Every grid cell is visited and processed at most once.
 *
 * SPACE COMPLEXITY: O(R * C) - In the worst-case scenario, the queue may hold up to 
 *                   nearly all grid cells (e.g., if all oranges are rotten at the start).
 */
int main() {
    cout << "=== Rotting Oranges Multi-Source BFS Simulator ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int rows, cols;
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) return 1;

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter the grid elements sequentially (0 = empty, 1 = fresh, 2 = rotten):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nSimulating parallel rot spreading across grid nodes..." << endl;
    int timeToRot = solver.orangesRotting(grid);

    if (timeToRot != -1) {
        cout << "Minimum time required for all oranges to rot: [" << timeToRot << " minutes]\n";
    } else {
        cout << "It is impossible to rot all oranges. Output: [-1]\n";
    }

    return 0;
}