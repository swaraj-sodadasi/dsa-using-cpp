#include <iostream>
#include <vector>

using namespace std;

class Solution {
private:
    int uniquePathsCount;
    int targetEmptyCellsCount;
    int rows, cols;

    // Cardinal direction grid offset lookups: Up, Down, Left, Right
    const int dRows[4] = {-1, 1, 0, 0};
    const int dCols[4] = {0, 0, -1, 1};

    /*
     * RECURSIVE HAMILTONIAN PATH GRID COVER DFS ENGINE
     * ------------------------------------------------
     * The problem asks us to find the number of 4-directional walks from the starting square
     * to the ending square that walk over every non-obstacle square exactly once.
     * This is an exact Hamiltonian Path problem bounded within a 2D matrix graph framework.
     *
     * State Parameters:
     *  - grid: The active 2D grid matrix configuration layout.
     *  - r: Current row coordinate tracker.
     *  - c: Current column coordinate tracker.
     *  - visitedCount: The running count of empty cells successfully stepped on so far.
     */
    void findPaths(vector<vector<int>>& grid, int r, int c, int visitedCount) {
        
        // BASE CASE: ENDING CELL CONVERGENCE ASSERTER
        // If we step onto the target terminal cell (marked by value 2), we verify if
        // our path visited all required non-obstacle squares. If `visitedCount` matches 
        // `targetEmptyCellsCount`, we have a valid Hamiltonian path layout -> Increment result.
        if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 2) {
            if (visitedCount == targetEmptyCellsCount) {
                uniquePathsCount++;
            }
            return;
        }

        // STEP 1: CHOOSE (In-place Grid Masking to track cell state)
        // Cache the original item value layout and overwrite the block with an obstacle value (-1)
        // to prevent subsequent recursive branches in the current path sequence from re-stepping here.
        int originalValue = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        grid[static_cast<size_t>(r)][static_cast<size_t>(c)] = -1;

        // STEP 2: RECURSE (Spin off exploration paths into all 4 cardinal directions)
        for (int i = 0; i < 4; ++i) {
            int nextR = r + dRows[i];
            int nextC = c + dCols[i];

            // Boundary validation threshold checks
            if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                // Pruning Condition: Only advance if the cell is not an obstacle (-1)
                if (grid[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] != -1) {
                    findPaths(grid, nextR, nextC, visitedCount + 1);
                }
            }
        }

        // STEP 3: UNCHOOSE / BACKTRACK (Restore original grid value layout state)
        // Revert the cell to its original value before returning up the execution stack.
        grid[static_cast<size_t>(r)][static_cast<size_t>(c)] = originalValue;
    }

public:
    int uniquePathsIII(vector<vector<int>>& grid) {
        uniquePathsCount = 0;
        targetEmptyCellsCount = 0;
        
        if (grid.empty() || grid[0].empty()) return 0;

        rows = static_cast<int>(grid.size());
        cols = static_cast<int>(grid[0].size());

        int startR = 0, startC = 0;

        // PRECOMPUTATION SWEEP: Locate anchors and count non-obstacle cells
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    startR = r;
                    startC = c;
                    targetEmptyCellsCount++; // The starting cell is included as a visited step
                } else if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 0) {
                    targetEmptyCellsCount++; // Accumulate standard empty cell spaces
                } else if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 2) {
                    targetEmptyCellsCount++; // The ending cell is included as a visited step
                }
            }
        }

        // Trigger the Hamiltonian walk starting from the grid anchor position
        findPaths(grid, startR, startC, 1);
        
        return uniquePathsCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(4^(R * C)) Worst-Case - Where R and C represent grid row and column counts.
 *                  At each cell step block, the search path branches out in up to 4 parallel paths.
 *                  However, because the grid contains fixed structural obstacle barriers (-1) and paths 
 *                  cannot intersect themselves, the practical execution space handles maximum sizes easily.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - Bounded by the maximum depth of the recursive execution stack 
 *                   which scales linearly with the total element square cell surface area.
 */
int main() {
    int r, c;
    cout << "Enter the grid dimensions (Rows followed by Columns): ";
    if (!(cin >> r >> c) || r <= 0 || c <= 0) {
        cout << "Invalid grid sizing parameter configurations." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(r), vector<int>(static_cast<size_t>(c)));
    cout << "Enter the matrix cell layout values row by row:\n";
    cout << "  (1: Start, 2: End, 0: Empty, -1: Obstacle)\n";
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            cin >> grid[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting Hamiltonian grid path coverage check loops..." << endl;
    int result = solver.uniquePathsIII(grid);

    cout << "Total distinct complete coverage paths found: " << result << endl;

    return 0;
}