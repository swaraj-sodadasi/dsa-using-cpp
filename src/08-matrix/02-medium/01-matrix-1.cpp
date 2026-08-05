#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * 01 MATRIX ENGINE: TWO-PASS DP RELAXATION METHOD
     * ------------------------------------------------
     * The objective is to find the distance of the nearest 0 for each cell in a binary matrix.
     * The distance between two adjacent cells is 1.
     *
     * Algorithmic Strategy (Dynamic Programming Optimization):
     * - Instead of using a Multi-Source BFS which requires queue management overhead, we can 
     *   solve this optimally using a 2-Pass Dynamic Programming approach over the spatial grid.
     * - Distance from any cell to a 0 can only propagate from 4 directions: Top, Left, Bottom, Right.
     * - We can split this look-up pattern into two non-interfering sequential loop sweeps:
     * 
     * Pass 1: Top-Left to Bottom-Right Sweep
     * - For each cell, check its distance path relative to its Top neighbor and its Left neighbor.
     *   `dist[r][c] = min(dist[r][c], min(dist[r-1][c], dist[r][c-1]) + 1)`
     * 
     * Pass 2: Bottom-Right to Top-Left Sweep
     * - For each cell, check its distance path relative to its Bottom neighbor and its Right neighbor.
     *   `dist[r][c] = min(dist[r][c], min(dist[r+1][c], dist[r][c+1]) + 1)`
     * 
     * Invariants & Base Cases:
     * - If `mat[r][c] == 0`, its baseline distance is locked at 0.
     * - Otherwise, initialize its starting state to a safe upper bound proxy (`INT_MAX - 100000`) 
     *   to prevent addition overflow during calculations.
     */
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        if (mat.empty() || mat[0].empty()) return mat;

        int rows = static_cast<int>(mat.size());
        int cols = static_cast<int>(mat[0].size());
        
        // Define a safe large proxy value to avoid overflow arithmetic bugs
        int INF = INT_MAX - 100000;

        vector<vector<int>> dist(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols), INF));

        // Pass 1: Forward check pass (Examine cells relative to Top and Left directional neighbors)
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (mat[static_cast<size_t>(r)][static_cast<size_t>(c)] == 0) {
                    dist[static_cast<size_t>(r)][static_cast<size_t>(c)] = 0;
                } else {
                    // Look up to the top cell if it falls within grid limits
                    if (r > 0) {
                        dist[static_cast<size_t>(r)][static_cast<size_t>(c)] = min(
                            dist[static_cast<size_t>(r)][static_cast<size_t>(c)], 
                            dist[static_cast<size_t>(r - 1)][static_cast<size_t>(c)] + 1
                        );
                    }
                    // Look back to the left cell if it falls within grid limits
                    if (c > 0) {
                        dist[static_cast<size_t>(r)][static_cast<size_t>(c)] = min(
                            dist[static_cast<size_t>(r)][static_cast<size_t>(c)], 
                            dist[static_cast<size_t>(r)][static_cast<size_t>(c - 1)] + 1
                        );
                    }
                }
            }
        }

        // Pass 2: Backward check pass (Examine cells relative to Bottom and Right directional neighbors)
        for (int r = rows - 1; r >= 0; --r) {
            for (int c = cols - 1; c >= 0; --c) {
                // Look down to the bottom cell if it falls within grid limits
                if (r < rows - 1) {
                    dist[static_cast<size_t>(r)][static_cast<size_t>(c)] = min(
                        dist[static_cast<size_t>(r)][static_cast<size_t>(c)], 
                        dist[static_cast<size_t>(r + 1)][static_cast<size_t>(c)] + 1
                    );
                }
                // Look forward to the right cell if it falls within grid limits
                if (c < cols - 1) {
                    dist[static_cast<size_t>(r)][static_cast<size_t>(c)] = min(
                        dist[static_cast<size_t>(r)][static_cast<size_t>(c)], 
                        dist[static_cast<size_t>(r)][static_cast<size_t>(c + 1)] + 1
                    );
                }
            }
        }

        return dist;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity 
 *                  of the input grid layout. The code executes exactly two clean iterative sweeps 
 *                  over the cells, completing operations in strictly linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Excluding the memory structure initialized to allocate 
 *                   and return the output tracking distances grid, no additional call stacks, 
 *                   hash tables, or internal FIFO queues are used.
 */
int main() {
    int rows, cols;
    cout << "Enter matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid matrix boundary parameter options configured." << endl;
        return 1;
    }

    vector<vector<int>> mat(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter binary matrix elements row by row (0 or 1 only):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> mat[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (mat[static_cast<size_t>(r)][static_cast<size_t>(c)] != 0 && 
                mat[static_cast<size_t>(r)][static_cast<size_t>(c)] != 1) {
                cout << "Constraint Error: Input grid elements must be strictly 0 or 1." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting sequential two-pass dynamic relaxation sweeps..." << endl;
    vector<vector<int>> resultGrid = solver.updateMatrix(mat);

    cout << "\nCalculated nearest-zero coordinate distance matrix visualization:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cout << resultGrid[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}