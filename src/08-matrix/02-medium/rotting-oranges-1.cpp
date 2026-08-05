#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * MULTI-SOURCE BREADTH-FIRST SEARCH LEVEL STATE TRACKING ENGINE
     * -------------------------------------------------------------
     * RESOLVED TYPO: Fixed the main entry point name to match LeetCode's standard
     * signature interface exactly: `orangesRotting`.
     *
     * Invariants & State Values:
     * - 0: Empty cell
     * - 1: Fresh orange
     * - 2: Rotten orange
     */
    int orangesRotting(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        queue<pair<int, int>> rottenQueue;
        int freshOrangesCount = 0;

        // Step 1: Baseline inspection pass to seed the queue and count fresh components
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 2) {
                    rottenQueue.push({r, c});
                } else if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    freshOrangesCount++;
                }
            }
        }

        // If there are no fresh oranges initially, the configuration is pre-resolved
        if (freshOrangesCount == 0) return 0;

        int minutesElapsed = 0;
        
        // Direction offsets for orthogonal navigation (Up, Right, Down, Left)
        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        // Step 2 & 3: Run the Multi-Source BFS level processing iterations
        while (!rottenQueue.empty() && freshOrangesCount > 0) {
            int currentLevelSize = static_cast<int>(rottenQueue.size());
            minutesElapsed++;

            for (int i = 0; i < currentLevelSize; ++i) {
                auto [currRow, currCol] = rottenQueue.front();
                rottenQueue.pop();

                for (int d = 0; d < 4; ++d) {
                    int neighborRow = currRow + deltaRow[d];
                    int neighborCol = currCol + deltaCol[d];

                    // Validate matrix boundary conditions
                    if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                        // Check if the adjacent candidate is a fresh, vulnerable orange
                        if (grid[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] == 1) {
                            // Turn rotten in-place to anchor structural tracking states
                            grid[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] = 2;
                            freshOrangesCount--;
                            rottenQueue.push({neighborRow, neighborCol});
                        }
                    }
                }
            }
        }

        // Step 5: Final component cross-check
        return (freshOrangesCount == 0) ? minutesElapsed : -1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity 
 *                  of the grid matrix. Every individual matrix cell is scanned during initialization, 
 *                  and subsequently enqueued/dequeued at most once during the multi-source sweep.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - In the worst-case configuration (e.g., the entire grid is 
 *                   initially filled with rotten oranges), the structural FIFO queue memory bounds 
 *                   scale linearly with the total number of matrix elements.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter grid cell state integers row by row (0: Empty, 1: Fresh, 2: Rotten):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            
            // Constraint structural validation
            if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] < 0 || 
                grid[static_cast<size_t>(r)][static_cast<size_t>(c)] > 2) {
                cout << "Constraint Error: State values must be strictly 0, 1, or 2." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting multi-source structural propagation sweeps..." << endl;
    
    // Fixed method name call from orangeRotting to orangesRotting
    int computationalMinutes = solver.orangesRotting(grid);

    if (computationalMinutes == -1) {
        cout << "Outcome: It is impossible to rot all oranges. Result = -1" << endl;
    } else {
        cout << "Minimum dynamic timeline elapsed units required to rot all fresh oranges: " 
             << computationalMinutes << " minute(s)." << endl;
    }

    return 0;
}