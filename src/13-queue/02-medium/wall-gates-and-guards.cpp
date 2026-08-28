#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-SOURCE FIFO PROXIMITY MAPPING
     * -------------------------------------------------------------------------------------
     * The objective is to fill each empty room (INF) with the distance to its nearest gate. 
     * - Obstacles/Walls are represented by -1.
     * - Gates are represented by 0.
     * - Empty rooms are represented by 2147483647 (INF).
     * 
     * 1. THE INVERSE PATHWAY REVERSAL:
     *    Instead of executing a search from every single empty room (which scales poorly 
     *    to O((R*C)^2)), we initialize a multi-source BFS originating simultaneously 
     *    from all gates (value 0).
     * 
     * 2. CHRONOLOGICAL LEVEL DISPERSION:
     *    - The queue is pre-loaded with the coordinates of all gates.
     *    - Since gates represent the starting distance layer (0), expanding outward level-by-level 
     *      ensures that any empty cell visited for the first time receives its shortest possible 
     *      path distance.
     * 
     * 3. IN-PLACE PROGRESSIVE STATE COMPILING:
     *    For each popped cell, we explore its 4-directional neighbors. If a neighbor is inside 
     *    bounds and has a value of INF (unvisited), we assign its distance as 
     *    `current_distance + 1`, and enqueue its position.
     */
    void wallsAndGates(vector<vector<int>>& rooms) {
        if (rooms.empty()) return;

        int rows = static_cast<int>(rooms.size());
        int cols = static_cast<int>(rooms[0].size());
        queue<pair<int, int>> bfsQueue;

        // Step 1: Identify and push all multi-source gate origins (0) to the queue
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (rooms[static_cast<size_t>(r)][static_cast<size_t>(c)] == 0) {
                    bfsQueue.push({r, c});
                }
            }
        }

        // Direction vectors for 4-directional grid steps
        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        // Step 2: Propagate distances level-by-level
        while (!bfsQueue.empty()) {
            auto [r, c] = bfsQueue.front();
            bfsQueue.pop();

            int currentDist = rooms[static_cast<size_t>(r)][static_cast<size_t>(c)];

            for (int d = 0; d < 4; ++d) {
                int neighborRow = r + dRow[d];
                int neighborCol = c + dCol[d];

                // Check boundaries and target unvisited room nodes (INF)
                if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                    if (rooms[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] == 2147483647) {
                        rooms[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] = currentDist + 1;
                        bfsQueue.push({neighborRow, neighborCol});
                    }
                }
            }
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents rows and C represents columns of the grid.
 *                  Every node is visited and updated at most once.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - In the worst case, the BFS queue stores 
 *                   nearly all elements of the matrix.
 */
int main() {
    cout << "=== Wall Gates and Guards Multi-Source BFS Proximity Engine ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int rows, cols;
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) return 1;

    vector<vector<int>> rooms(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter the grid layout sequentially (-1 = Wall, 0 = Gate, 2147483647 = Empty Room):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> rooms[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nPropagating short-path bounds from gate nodes..." << endl;
    solver.wallsAndGates(rooms);

    cout << "\nCalculated Proximity Matrix Grid:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int val = rooms[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (val == 2147483647) {
                cout << "INF ";
            } else {
                cout << val << " ";
            }
        }
        cout << "\n";
    }

    return 0;
}