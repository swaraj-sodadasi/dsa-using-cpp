#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * LINEARIZED INDEX SHIFT TRANSFORMATION ENGINE
     * --------------------------------------------
     * The objective is to shift a 2D grid of size M x N by k elements sequentially. 
     * In each shift operation:
     * - The element grid[r][c] moves to grid[r][c+1].
     * - The element grid[r][n-1] moves to grid[r+1][0].
     * - The element grid[m-1][n-1] moves to grid[0][0].
     *
     * Algorithmic & Flattening Strategy:
     * - Rather than executing individual dynamic item movements k times (which results 
     *   in a slow O(k * m * n) routine), we view the 2D grid as a flattened 1D array 
     *   of size totalElements = m * n.
     * - Shifting the 2D grid by k steps corresponds to shifting the flattened 1D array 
     *   to the right by (k % totalElements) positions.
     * - We track coordinates directly using mathematical index mappings. Any 2D matrix 
     *   location (r, c) maps to a flat 1D position via:
     *   linearIndex = r * n + c
     * - The new shifted 1D position is calculated as:
     *   newLinearIndex = (linearIndex + k) % totalElements
     * - Finally, we unpack `newLinearIndex` back into 2D coordinates for the destination grid:
     *   targetRow = newLinearIndex / n
     *   targetCol = newLinearIndex % n
     *
     * Execution Protocol:
     * - Step 1: Capture matrix sizes, calculate total elements, and normalize `k = k % totalElements`.
     * - Step 2: Allocate a new destination matrix `shiftedGrid` matching original dimensions.
     * - Step 3: Loop through the source matrix cells, compute their new shifted 2D positions, 
     *   and populate the target layout matrix directly in O(m * n) time.
     */
    vector<vector<int>> shiftGrid(vector<vector<int>>& grid, int k) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        int totalElements = m * n;

        // Normalize k to avoid redundant full-cycle matrix traversals
        k = k % totalElements;

        // Step 2: Allocate the target output matrix structure space
        vector<vector<int>> shiftedGrid(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n)));

        // Step 3: Flatten coordinates and perform linear index mapping transformations
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                // Calculate the original flat 1D index location
                int linearIndex = r * n + c;

                // Derive the target shifted 1D position
                int newLinearIndex = (linearIndex + k) % totalElements;

                // Deconstruct 1D index back to 2D target matrix layout coordinates
                size_t targetRow = static_cast<size_t>(newLinearIndex / n);
                size_t targetCol = static_cast<size_t>(newLinearIndex % n);

                shiftedGrid[targetRow][targetCol] = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            }
        }

        return shiftedGrid;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * N) - Where M represents row count and N represents column count 
 *                  of the input grid layout. The index translation engine maps each grid item 
 *                  exactly once in linear time relative to matrix capacity.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Excluding the memory structure initialized to store 
 *                   and return the output dataset, the remapping math runs strictly in place.
 */
int main() {
    int m, n;
    cout << "Enter matrix grid dimensions (Rows followed by Columns): ";
    if (!(cin >> m >> n) || m <= 0 || n <= 0) {
        cout << "Invalid boundary parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(m), vector<int>(static_cast<size_t>(n)));
    cout << "Enter matrix element values row by row:\n";
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    int k;
    cout << "\nEnter total shift operation offset step count (k): ";
    cin >> k;
    if (k < 0) {
        cout << "Invalid parameter choice. Shift count cannot hold negative values." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting linearized transformation re-indexing pipeline loops..." << endl;
    vector<vector<int>> resultGrid = solver.shiftGrid(grid, k);

    cout << "\nProcessed matrix structural output layout visualization:\n";
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            cout << resultGrid[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}