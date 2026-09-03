#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: 0-1 BFS DEQUE QUEUE OPTIMIZATION
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum cost to make at least one valid path from 
     * (0, 0) to (m-1, n-1) on a grid where cell values indicate default travel directions:
     *   1: Right, 2: Left, 3: Down, 4: Up.
     * Modifying a cell's default direction to a different direction costs exactly 1.
     * 
     * 1. THE 0-1 EDGE WEIGHT MODEL:
     *    - Moving in the direction indicated by the cell has a cost of 0.
     *    - Moving in any other of the 4 directions has a cost of 1.
     *    This transforms the grid into a directed graph with edge weights of strictly 0 or 1.
     * 
     * 2. 0-1 BFS TRAVERSAL (DIJKSTRA ALTERNATIVE):
     *    Instead of a O((M*N) log(M*N)) Priority Queue (Dijkstra), we use a Double-Ended 
     *    Queue (std::deque) to achieve true O(M*N) linear complexity.
     *    - 0-cost transitions: Pushed to the FRONT of the deque (processed immediately).
     *    - 1-cost transitions: Pushed to the BACK of the deque (processed later).
     * 
     * 3. RELAXATION AND VISITED GUARANTEE:
     *    We maintain a 2D array `dist` storing the minimum cost to reach each cell. If we 
     *    find a path to a neighbor with a lower cost than its recorded value, we relax the 
     *    edge and enqueue the neighbor.
     */
    int minCost(const vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());

        // Direction offsets corresponding to values: 1 (Right), 2 (Left), 3 (Down), 4 (Up)
        int dRow[] = {0, 0, 0, 1, -1};
        int dCol[] = {0, 1, -1, 0, 0};

        // Initialize cost matrix with infinity (unvisited/unrelaxed)
        vector<vector<int>> dist(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n), 1e9));
        deque<pair<int, int>> dq;

        dq.push_front({0, 0});
        dist[0][0] = 0;

        while (!dq.empty()) {
            auto [r, c] = dq.front();
            dq.pop_front();

            // Destination reached
            if (r == m - 1 && c == n - 1) {
                return dist[static_cast<size_t>(r)][static_cast<size_t>(c)];
            }

            int currentDirection = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];

            for (int d = 1; d <= 4; ++d) {
                int nr = r + dRow[d];
                int nc = c + dCol[d];

                if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                    // Cost is 0 if moving along the matching grid arrow direction, 1 otherwise
                    int cost = (currentDirection == d) ? 0 : 1;
                    int nextDist = dist[static_cast<size_t>(r)][static_cast<size_t>(c)] + cost;

                    if (nextDist < dist[static_cast<size_t>(nr)][static_cast<size_t>(nc)]) {
                        dist[static_cast<size_t>(nr)][static_cast<size_t>(nc)] = nextDist;

                        if (cost == 0) {
                            dq.push_front({nr, nc});
                        } else {
                            dq.push_back({nr, nc});
                        }
                    }
                }
            }
        }

        return dist[static_cast<size_t>(m - 1)][static_cast<size_t>(n - 1)];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * N) - Where M and N are the dimensions of the grid.
 *                  Every cell is popped from the deque and processed at most once.
 *
 * SPACE COMPLEXITY: O(M * N) Auxiliary - To store the distance matrix and deque structures.
 */
int main() {
    cout << "=== Minimum Cost Path Finder via 0-1 BFS Deque ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int m, n;
    if (!(cin >> m >> n) || m <= 0 || n <= 0) return 1;

    vector<vector<int>> grid(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n)));
    cout << "Enter the grid layout (1=Right, 2=Left, 3=Down, 4=Up):\n";
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nResolving low-cost state-space routing paths..." << endl;
    int ans = solver.minCost(grid);

    cout << "Minimum cost needed to construct a valid path: [" << ans << "]\n";
    return 0;
}