#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
private:
    // Direction vectors for moving up, down, left, and right in the grid
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    /*
     * BFS PATH CONNECTIVITY CHECKER
     * -----------------------------
     * Validates whether a continuous water path exists from the top-left corner (0, 0)
     * to the bottom-right corner (N-1, N-1) without stepping on any cell whose elevation
     * exceeds the given maximum water level threshold `maxTime`.
     */
    bool hasValidPath(const vector<vector<int>>& grid, int maxTime) {
        int n = static_cast<int>(grid.size());
        
        // Early failure condition: if starting or ending elevation exceeds threshold, path is impossible
        if (grid[0][0] > maxTime || grid[static_cast<size_t>(n - 1)][static_cast<size_t>(n - 1)] > maxTime) {
            return false;
        }

        vector<vector<bool>> visited(static_cast<size_t>(n), vector<bool>(static_cast<size_t>(n), false));
        queue<pair<int, int>> q;

        // Initialize BFS structure at the grid origin
        q.push({0, 0});
        visited[0][0] = true;

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            // Destination reached successfully
            if (r == n - 1 && c == n - 1) {
                return true;
            }

            // Evaluate all 4 orthogonal grid directions
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];

                // Check grid bounds and visited state
                if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                    size_t unr = static_cast<size_t>(nr);
                    size_t unc = static_cast<size_t>(nc);

                    if (!visited[unr][unc] && grid[unr][unc] <= maxTime) {
                        visited[unr][unc] = true;
                        q.push({nr, nc});
                    }
                }
            }
        }

        return false;
    }

public:
    /*
     * BINARY SEARCH ON THRESHOLD RANGE + BFS CONNECTIVITY CHECK ENGINE
     * ----------------------------------------------------------------
     * The objective is to find the minimum time platform configuration threshold 
     * required to swim from (0,0) to (N-1,N-1) in an N x N elevation grid.
     *
     * Mathematical & Algorithmic Strategy:
     * - The time threshold possesses monotonic qualities: if a water level `T` allows 
     *   for a valid path across the grid, any level greater than `T` will also contain 
     *   at least that same valid path. If it fails, any lower level is guaranteed to fail.
     * - This allows us to search the range of possible grid values using binary search 
     *   instead of running an incremental linear check.
     *
     * Range Bounds Allocation:
     * - Lower Bound (`left`): The elevation at the starting cell `grid[0][0]`. You cannot 
     *   start swimming until the water rises at least to this height.
     * - Upper Bound (`right`): The highest elevation cell present anywhere in the matrix, 
     *   which acts as the absolute maximum value Koko or the swimmer would ever need to wait for.
     */
    int swimInWater(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int n = static_cast<int>(grid.size());
        
        int left = grid[0][0];
        int right = 0;
        
        // Find the absolute maximum ceiling value inside the grid boundaries
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                right = max(right, grid[static_cast<size_t>(i)][static_cast<size_t>(j)]);
            }
        }

        int optimalTimeResult = right;

        // Execute binary search across the monotonic threshold interval spectrum
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (hasValidPath(grid, mid)) {
                optimalTimeResult = mid; // Register the valid time candidate path layout
                right = mid - 1;         // Compress search frame downward to look for earlier times
            } else {
                left = mid + 1;          // Elevation threshold too restrictive; shift lower bound up
            }
        }

        return optimalTimeResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2 * log(MaxElevation)) - Where N^2 represents total grid node cells. 
 *                  The binary search space covers the range between minimum and maximum cell elevations. 
 *                  Each decision step runs an O(N^2) linear BFS connectivity sweep.
 *
 * SPACE COMPLEXITY: O(N^2) Auxiliary - The internal matrix connectivity checks instantiate a 
 *                   visited boolean structure mapping sizes proportional to the grid layout.
 */
int main() {
    int n;
    cout << "Enter the matrix size grid dimension (N for an N x N grid): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The grid size dimension must be greater than 0." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n)));
    cout << "Enter the grid matrix elevation values row by row:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting threshold binary search range adjustments with BFS validations..." << endl;
    int optimalTimeOutcome = solver.swimInWater(grid);

    cout << "Minimum required time parameter to complete swim path: " << optimalTimeOutcome << " units." << endl;

    return 0;
}