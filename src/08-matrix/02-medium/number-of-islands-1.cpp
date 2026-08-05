#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
private:
    /*
     * BREADTH-FIRST SEARCH COMPONENT DISCOVERY PASS
     * --------------------------------------------
     * Traverses and uncovers all interconnected land blocks forming a single island cluster.
     * To maximize memory safety and performance, we modify discovered land pieces ('1')
     * to water pieces ('0') in-place instead of tracking them in an auxiliary visited matrix.
     */
    void exploreIslandBFS(vector<vector<char>>& grid, int startRow, int startCol, int rows, int cols) {
        queue<pair<int, int>> coordinateQueue;
        
        coordinateQueue.push({startRow, startCol});
        grid[static_cast<size_t>(startRow)][static_cast<size_t>(startCol)] = '0'; // Sink the land cell to mark as visited

        // Orthogonal traversal directional vectors (Up, Right, Down, Left)
        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        while (!coordinateQueue.empty()) {
            auto [currRow, currCol] = coordinateQueue.front();
            coordinateQueue.pop();

            for (int i = 0; i < 4; ++i) {
                int neighborRow = currRow + deltaRow[i];
                int neighborCol = currCol + deltaCol[i];

                // Check grid boundaries and confirm if the neighbor cell is unvisited land
                if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                    if (grid[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] == '1') {
                        grid[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] = '0'; // Sink cell
                        coordinateQueue.push({neighborRow, neighborCol});
                    }
                }
            }
        }
    }

public:
    /*
     * CONNECTED COMPONENTS TRAVERSAL MATRIX LABELING ENGINE
     * -----------------------------------------------------
     * The objective is to compute the total number of distinct islands within a 2D binary grid map.
     * An island is formed by connecting adjacent land cells ('1') horizontally or vertically.
     *
     * Algorithmic Strategy:
     * - We process the 2D matrix coordinate map as an unweighted, undirected graph framework.
     * - The task maps directly to counting the total number of disjoint connected components in the graph.
     * - We scan the matrix cell-by-cell. When an unvisited land cell ('1') is encountered, it indicates
     *   the discovery of a new independent island cluster. 
     * - We increment our counter and execute a BFS to completely submerge all reachable land cells 
     *   in that specific component, avoiding duplicate validation sweeps.
     */
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());
        int islandCounter = 0;

        // Traverse every grid coordinate index to isolate land masses
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == '1') {
                    islandCounter++;
                    // Trigger BFS to flood and mark all land cells belonging to this island
                    exploreIslandBFS(grid, r, c, rows, cols);
                }
            }
        }

        return islandCounter;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents the row capacity and C represents column capacity 
 *                  of the grid matrix. Every individual matrix coordinate cell is visited a bounded 
 *                  number of times (once during the primary loop sweep, and at most once by the BFS 
 *                  components), matching linear time complexity relative to the total elements.
 *
 * SPACE COMPLEXITY: O(Min(R, C)) Auxiliary - The queue allocation tracking memory bounds are determined 
 *                   by the maximum possible width of the BFS traversal wavefront, which in the worst-case 
 *                   scenario is bounded by the minimum dimension of the grid matrix layout.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary structural dimensions configured." << endl;
        return 1;
    }

    vector<vector<char>> grid(static_cast<size_t>(rows), vector<char>(static_cast<size_t>(cols)));
    cout << "Enter matrix binary cell characters row by row ('1' for Land, '0' for Water):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != '0' && 
                grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != '1') {
                cout << "Constraint Error: Grid element must be strictly character '0' or '1'." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting connected components grid search routines..." << endl;
    int totalIslandsCalculated = solver.numIslands(grid);

    cout << "Total number of isolated island structures identified: " << totalIslandsCalculated << endl;

    return 0;
}