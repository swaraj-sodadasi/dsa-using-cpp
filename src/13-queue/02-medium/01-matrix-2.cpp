#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-SOURCE BFS DISTANCE PROPAGATION
     * -------------------------------------------------------------------------------------
     * The objective is to find the distance of the nearest 0 for each cell in a 01 matrix.
     * 
     * 1. THE INVERSE LOOKUP STRATEGY:
     *    Instead of searching from each 1 to find its nearest 0 (which results in redundant 
     *    O(R*C * R*C) paths), we run a multi-source Breadth-First Search starting from 
     *    *all* 0s simultaneously.
     * 
     * 2. INITIALIZATION:
     *    - We initialize an output distance matrix with a sentinel value (e.g., -1) to mark 
     *      unvisited states.
     *    - For every cell with a value of 0, we set its output distance to 0 and push its 
     *      coordinates onto the queue.
     * 
     * 3. MONOTONIC BREADTH-FIRST PROPAGATION:
     *    - As we pop coordinates from the queue, we check their 4-directional neighbors.
     *    - If a neighbor is unvisited (distance is -1), we compute its distance as 
     *      `current_distance + 1`, update the matrix, and enqueue the neighbor.
     *    - Due to the nature of BFS level-by-level traversal, the first time we visit any 
     *      cell, we are guaranteed to have found its absolute minimum distance.
     */
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        if (mat.empty()) return {};

        int rows = static_cast<int>(mat.size());
        int cols = static_cast<int>(mat[0].size());
        vector<vector<int>> dist(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols), -1));
        queue<pair<int, int>> bfsQueue;

        // Step 1: Push all multi-source origins (0s) onto the queue
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (mat[static_cast<size_t>(r)][static_cast<size_t>(c)] == 0) {
                    dist[static_cast<size_t>(r)][static_cast<size_t>(c)] = 0;
                    bfsQueue.push({r, c});
                }
            }
        }

        // Direction vectors for 4-directional cell movements
        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        // Step 2: Propagate distances level-by-level
        while (!bfsQueue.empty()) {
            auto [r, c] = bfsQueue.front();
            bfsQueue.pop();

            for (int d = 0; d < 4; ++d) {
                int neighborRow = r + dRow[d];
                int neighborCol = c + dCol[d];

                // Validate boundaries and check if neighbor cell is unvisited
                if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                    if (dist[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] == -1) {
                        dist[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] = 
                            dist[static_cast<size_t>(r)][static_cast<size_t>(c)] + 1;
                        bfsQueue.push({neighborRow, neighborCol});
                    }
                }
            }
        }

        return dist;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents rows and C represents columns of the matrix.
 *                  Every node is enqueued and dequeued exactly once.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - To hold the BFS queue elements and output layout registers.
 */
int main() {
    cout << "=== 01 Matrix Multi-Source BFS Range Search Engine ===\n";
    cout << "Enter matrix dimensions (rows columns): ";
    int rows, cols;
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) return 1;

    vector<vector<int>> mat(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter the matrix elements sequentially (0 or 1):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> mat[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nPropagating short-path metrics from zero configurations..." << endl;
    vector<vector<int>> result = solver.updateMatrix(mat);

    cout << "Calculated Distance Matrix Matrix:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cout << result[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}