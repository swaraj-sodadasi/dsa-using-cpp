#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MINIMUM OBSTACLE REMOVAL TO REACH CORNER (0-1 BFS DEQUE OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum number of obstacles (1s) to remove to traverse 
     * from (0,0) to (m-1, n-1).
     * 
     * 1. THE 0-1 EDGE WEIGHT MODEL:
     *    - Moving into an empty cell (0) costs 0.
     *    - Moving into an obstacle cell (1) costs 1 (representing a removal).
     *    This transforms the grid into an unweighted graph with edge weights of strictly 0 or 1.
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
    int minimumObstacles(const vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());

        // Initialize cost matrix with infinity (unvisited/unrelaxed)
        vector<vector<int>> dist(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n), 1e9));
        deque<pair<int, int>> dq;

        dq.push_front({0, 0});
        dist[0][0] = 0;

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        while (!dq.empty()) {
            auto [r, c] = dq.front();
            dq.pop_front();

            // Destination reached
            if (r == m - 1 && c == n - 1) {
                return dist[static_cast<size_t>(r)][static_cast<size_t>(c)];
            }

            for (int i = 0; i < 4; ++i) {
                int nr = r + dRow[i];
                int nc = c + dCol[i];

                if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                    int cost = grid[static_cast<size_t>(nr)][static_cast<size_t>(nc)];
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
    cout << "=== Minimum Obstacle Removal Pathfinder via 0-1 BFS Deque ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int m, n;
    if (!(cin >> m >> n) || m <= 0 || n <= 0) return 1;

    vector<vector<int>> grid(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n)));
    cout << "Enter the grid layout (0=Empty, 1=Obstacle):\n";
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nResolving state-space routing paths and minimum obstacle eliminations..." << endl;
    int ans = solver.minimumObstacles(grid);

    cout << "Minimum obstacles needed to be removed: [" << ans << "]\n";
    return 0;
}