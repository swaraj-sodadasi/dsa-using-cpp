#include <iostream>
#include <vector>

using namespace std;

class Solution {
private:
    /*
     * REVERSE-TRAVERSAL DISCOVERY PASS
     * --------------------------------
     * Recursively traces water flow paths backward from ocean shores into the continent.
     * Water flows into a neighbor if the neighbor's height is greater than or equal to 
     * the current cell's height.
     */
    void dfsReverseFlow(int r, int c, int previousHeight, const vector<vector<int>>& heights, 
                        vector<vector<bool>>& oceanReachable, int rows, int cols) {
        // Boundary check, visited check, and validation of the elevation constraint
        if (r < 0 || r >= rows || c < 0 || c >= cols || 
            oceanReachable[static_cast<size_t>(r)][static_cast<size_t>(c)] || 
            heights[static_cast<size_t>(r)][static_cast<size_t>(c)] < previousHeight) {
            return;
        }

        // Flag the cell as reachable by the current ocean component
        oceanReachable[static_cast<size_t>(r)][static_cast<size_t>(c)] = true;

        // Orthogonal navigation steps (Up, Right, Down, Left)
        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        for (int i = 0; i < 4; ++i) {
            dfsReverseFlow(r + deltaRow[i], c + deltaCol[i], 
                           heights[static_cast<size_t>(r)][static_cast<size_t>(c)], 
                           heights, oceanReachable, rows, cols);
        }
    }

public:
    /*
     * SIMULTANEOUS REVERSE-BOUNDARY TRAVERSAL CONFLUENCE ENGINE
     * -----------------------------------------------------------
     * Given an M x N matrix of heights, find all coordinates from which water can flow
     * to both the Pacific Ocean (Top and Left borders) and Atlantic Ocean (Bottom and Right borders).
     *
     * Execution Protocol:
     * - Step 1: Pre-allocate two separate boolean matrices (`pacificReachable`, `atlanticReachable`) 
     *           to keep track of independent ocean reachability states.
     * - Step 2: Trigger DFS from the top/bottom borders (horizontal shores) and the left/right 
     *           borders (vertical shores).
     * - Step 3: Scan the full grid matrix. If a cell coordinate is marked true in both tracking maps, 
     *           it is added to the final results matrix.
     */
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        if (heights.empty() || heights[0].empty()) return {};

        int rows = static_cast<int>(heights.size());
        int cols = static_cast<int>(heights[0].size());

        // Step 1: Initialize independent reachability structures
        vector<vector<bool>> pacificReachable(static_cast<size_t>(rows), vector<bool>(static_cast<size_t>(cols), false));
        vector<vector<bool>> atlanticReachable(static_cast<size_t>(rows), vector<bool>(static_cast<size_t>(cols), false));

        // Step 2: Traverse from horizontal shores (Top Row -> Pacific, Bottom Row -> Atlantic)
        for (int c = 0; c < cols; ++c) {
            dfsReverseFlow(0, c, heights[0][static_cast<size_t>(c)], heights, pacificReachable, rows, cols);
            dfsReverseFlow(rows - 1, c, heights[static_cast<size_t>(rows - 1)][static_cast<size_t>(c)], heights, atlanticReachable, rows, cols);
        }

        // Traverse from vertical shores (Left Column -> Pacific, Right Column -> Atlantic)
        for (int r = 0; r < rows; ++r) {
            dfsReverseFlow(r, 0, heights[static_cast<size_t>(r)][0], heights, pacificReachable, rows, cols);
            dfsReverseFlow(r, cols - 1, heights[static_cast<size_t>(r)][static_cast<size_t>(cols - 1)], heights, atlanticReachable, rows, cols);
        }

        // Step 3: Collect intersection points forming the confluence coordinates
        vector<vector<int>> confluenceCells;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (pacificReachable[static_cast<size_t>(r)][static_cast<size_t>(c)] && 
                    atlanticReachable[static_cast<size_t>(r)][static_cast<size_t>(c)]) {
                    confluenceCells.push_back({r, c});
                }
            }
        }

        return confluenceCells;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity. 
 *                  Each cell coordinate is visited a bounded number of times during the 
 *                  two separate boundary-driven graph traversals.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - Consumed by the two structural boolean tracking matrices 
 *                   as well as the implicit recursive call stack, which in the worst case (a flat grid) 
 *                   can scale up to the total number of cells.
 */
int main() {
    int rows, cols;
    cout << "Enter continent grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid matrix boundary parameter options configured." << endl;
        return 1;
    }

    vector<vector<int>> heights(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter cell terrain elevations row by row:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> heights[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nLaunching simultaneous reverse-boundary traversal scans..." << endl;
    vector<vector<int>> results = solver.pacificAtlantic(heights);

    cout << "\nCoordinates capable of flowing into both the Pacific & Atlantic Oceans:\n[ ";
    for (size_t i = 0; i < results.size(); ++i) {
        cout << "[" << results[i][0] << ", " << results[i][1] << "]" 
             << (i + 1 < results.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}