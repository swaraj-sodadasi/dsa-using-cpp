#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: 8-DIRECTIONAL GRID EDGE RELAXATION VIA QUEUE (BFS)
     * -------------------------------------------------------------------------------------
     * The objective is to find the length of the shortest clear path in an N x N binary matrix.
     * A clear path starts at (0, 0) and ends at (N-1, N-1) traversing only 0s 8-directionally.
     * 
     * 1. THE GEOMETRIC ADJACENCY RULE:
     *    Movement can happen 8-directionally (horizontally, vertically, or diagonally).
     *    We specify this using coordinate difference offsets covering all adjacent cells.
     * 
     * 2. BREADTH-FIRST SEARCH SHORTEST PATH GUARANTEE:
     *    BFS naturally processes nodes in increasing order of distance from the source.
     *    The first time we hit the destination (N-1, N-1), we are guaranteed to have 
     *    traversed the absolute shortest path.
     * 
     * 3. GUARDING & REDUNDANCY PREVENTION:
     *    - If the start cell (0, 0) or end cell (N-1, N-1) is blocked (value 1), we immediately return -1.
     *    - To avoid revisiting nodes and prevent infinite cycles, we write over visited cells 
     *      with 1 (acting as walls) as soon as they are pushed into the queue.
     */
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return -1;
        
        int n = static_cast<int>(grid.size());
        
        // Return -1 if start or end nodes are blocked
        if (grid[0][0] == 1 || grid[static_cast<size_t>(n - 1)][static_cast<size_t>(n - 1)] == 1) {
            return -1;
        }
        
        // Single cell matrix edge case
        if (n == 1) return 1;

        // BFS queue holding pairs of {row, col}
        queue<pair<int, int>> bfsQueue;
        bfsQueue.push({0, 0});
        grid[0][0] = 1; // Mark as visited in-place

        // Offset arrays for all 8 adjacent directions
        int dRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        int pathLength = 1;

        while (!bfsQueue.empty()) {
            int levelSize = static_cast<int>(bfsQueue.size());

            for (int i = 0; i < levelSize; ++i) {
                auto [r, c] = bfsQueue.front();
                bfsQueue.pop();

                // If target reached, return current path length
                if (r == n - 1 && c == n - 1) {
                    return pathLength;
                }

                for (int d = 0; d < 8; ++d) {
                    int nextR = r + dRow[d];
                    int nextC = c + dCol[d];

                    // Check boundaries and check if the cell is open (0)
                    if (nextR >= 0 && nextR < n && nextC >= 0 && nextC < n) {
                        if (grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] == 0) {
                            grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] = 1; // Mark visited
                            bfsQueue.push({nextR, nextC});
                        }
                    }
                }
            }
            pathLength++;
        }

        return -1; // Destination is unreachable
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2) - Where N is the dimension of the square grid.
 *                  Every grid cell is visited and processed at most once.
 *
 * SPACE COMPLEXITY: O(N^2) - In the worst-case scenario, the BFS queue may hold 
 *                   up to O(N^2) elements.
 */
int main() {
    cout << "=== Shortest Path in Binary Matrix 8-Directional BFS ===\n";
    cout << "Enter the dimension of the square grid (N): ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<vector<int>> grid(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n)));
    cout << "Enter the grid layout sequentially (0 = open, 1 = blocked):\n";
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nCalculating shortest path using 8-directional exploration..." << endl;
    int path = solver.shortestPathBinaryMatrix(grid);

    if (path != -1) {
        cout << "Length of the shortest clear path: [" << path << "]\n";
    } else {
        cout << "No clear path exists. Output: [-1]\n";
    }

    return 0;
}