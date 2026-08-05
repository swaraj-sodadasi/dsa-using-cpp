#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * COORDINATE INTERSECTION EDGE METRIC DEDUCTION ENGINE
     * ----------------------------------------------------
     * The objective is to calculate the absolute perimeter of the island represented 
     * in a 2D binary grid, where 1 represents land cells and 0 represents water cells.
     *
     * Mathematical & Algorithmic Strategy:
     * - Every isolated single land square block natively contributes exactly 4 edge bounds to the 
     *   universal total perimeter count.
     * - Whenever two land squares share a border, those two intersecting faces are submerged 
     *   internally and no longer form part of the outer edge perimeter.
     * - Therefore, for each shared edge interface discovered between adjacent land squares, we 
     *   must deduct exactly 2 units from the global perimeter equation bounds:
     *   Total Perimeter = (Total Land Cells * 4) - (Shared Interfaces * 2)
     *
     * Execution Protocol:
     * - Step 1: Scan the 2D matrix layout grid cell by cell.
     * - Step 2: When a land pixel grid cell (grid[r][c] == 1) is found, increment `landCells`.
     * - Step 3: Check the immediately adjacent neighbors. To avoid double-counting interfaces, 
     *   we look ahead or look down (e.g., check if the cell below is land, and check if the cell 
     *   to the right is land). If a match is verified, increment `sharedInterfaces`.
     * - Step 4: Evaluate the structural deduction equation formulas and return the net integer value.
     */
    int islandPerimeter(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        int landCells = 0;
        int sharedInterfaces = 0;

        // Step 1 & 2: Parse through the layout coordinates scanning for active land squares
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    landCells++;

                    // Step 3: Check the neighbor directly below within valid dimensions
                    if (r + 1 < rows && grid[static_cast<size_t>(r + 1)][static_cast<size_t>(c)] == 1) {
                        sharedInterfaces++;
                    }
                    
                    // Check the neighbor directly to the right within valid dimensions
                    if (c + 1 < cols && grid[static_cast<size_t>(r)][static_cast<size_t>(c + 1)] == 1) {
                        sharedInterfaces++;
                    }
                }
            }
        }

        // Step 4: Deduct intersecting bounds according to geometric edge rules
        return (landCells * 4) - (sharedInterfaces * 2);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity 
 *                  of the input grid layout matrix. The double nested loop inspects each pixel 
 *                  coordinate exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Processing executes entirely in place using scalar counters 
 *                   without declaring extra storage buffers or call stack tracking layers.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural boundary inputs configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter grid binary pixel values row by row (1 for Land, 0 for Water):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nExecuting coordinate intersection metric calculation loops..." << endl;
    int totalPerimeterLength = solver.islandPerimeter(grid);

    cout << "Total computed outer edge perimeter metric of the island structure: " << totalPerimeterLength << endl;

    return 0;
}