#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    // Disjoint Set Union (DSU) / Union-Find structure optimized with path compression and rank sizing
    class UnionFind {
    private:
        vector<int> parent;
        vector<int> size;
    public:
        UnionFind(int n) {
            parent.resize(static_cast<size_t>(n));
            size.resize(static_cast<size_t>(n), 1);
            for (int i = 0; i < n; ++i) parent[static_cast<size_t>(i)] = i;
        }

        int find(int i) {
            if (parent[static_cast<size_t>(i)] == i) return i;
            return parent[static_cast<size_t>(i)] = find(parent[static_cast<size_t>(i)]); // Path compression
        }

        void unionNodes(int i, int j) {
            int rootI = find(i);
            int rootJ = find(j);
            if (rootI != rootJ) {
                // Rank sizing integration to maintain lower tree depths
                if (size[static_cast<size_t>(rootI)] < size[static_cast<size_t>(rootJ)]) {
                    swap(rootI, rootJ);
                }
                parent[static_cast<size_t>(rootJ)] = rootI;
                size[static_cast<size_t>(rootI)] += size[static_cast<size_t>(rootJ)];
            }
        }

        int getComponentSize(int i) {
            return size[static_cast<size_t>(find(i))];
        }
    };

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: REVERSE TIME PROCESSING WITH GRID CONNECTIVITY COMPONENTS
     * -------------------------------------------------------------------------------------
     * The objective is to determine how many bricks fall immediately after each hit in a grid.
     * A brick is stable if it is attached to the top row (row 0) or connected to another stable brick.
     * 
     * 1. THE REVERSE TIME STRATEGY:
     *    Simulating hits forward requires breaking down existing connected structures, which is an 
     *    expensive O(R * C) operation per hit. Inverting the sequence (processing hits in reverse order)
     *    transforms the problem from dynamic *deletion/disconnection* into dynamic *insertion/union*, 
     *    which matches the fast capabilities of a Disjoint Set Union (DSU) engine.
     * 
     * 2. STABLE ROOT AGGREGATION CELL:
     *    We flatten the 2D grid matrix into a 1D index: `index = r * cols + c`. To track global stability,
     *    we introduce a virtual "Stable Root Node" at index `rows * cols`. Every brick situated natively on 
     *    row 0 is unioned directly to this stable master node.
     * 
     * 3. THE PROTOCOL STEPS:
     *    - Step A: Deep-copy the grid map, then clear all bricks matching the hits checklist to find the 
     *      final baseline state.
     *    - Step B: Build the initial reverse DSU structure by grouping remaining stable brick fragments.
     *    - Step C: Iterate backward through the hits queue. For each hit that originally contained a brick:
     *      - Record the stable component size *before* restoration.
     *      - Restore the brick in the grid and union it with its 4 orthogonal neighbors (and the stable root if row == 0).
     *      - Calculate the stable component size *after* restoration. The net increase (minus 1 for the hit brick itself) 
     *        yields the count of falling bricks triggered by this specific step.
     */
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());
        int numHits = static_cast<int>(hits.size());

        // Step A: Trace baseline state by subtracting hit targets from the grid
        vector<vector<int>> workingGrid = grid;
        for (int i = 0; i < numHits; ++i) {
            workingGrid[static_cast<size_t>(hits[i][0])][static_cast<size_t>(hits[i][1])]--;
        }

        // Initialize DSU workspace; index `rows * cols` serves as the structural master Stable Root Node
        int stableRoot = rows * cols;
        UnionFind dsu(stableRoot + 1);

        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        // Step B: Build the starting reverse DSU graph state using remaining brick pieces
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (workingGrid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    int currIdx = r * cols + c;
                    if (r == 0) {
                        dsu.unionNodes(currIdx, stableRoot);
                    }
                    // Connect with downstream/right structural cells to avoid double union routines
                    if (r + 1 < rows && workingGrid[static_cast<size_t>(r + 1)][static_cast<size_t>(c)] == 1) {
                        dsu.unionNodes(currIdx, (r + 1) * cols + c);
                    }
                    if (c + 1 < cols && workingGrid[static_cast<size_t>(r)][static_cast<size_t>(c + 1)] == 1) {
                        dsu.unionNodes(currIdx, r * cols + (c + 1));
                    }
                }
            }
        }

        vector<int> fallingBricksCount(static_cast<size_t>(numHits), 0);

        // Step C: Iterate backwards from the final chronological hit to restore components
        for (int i = numHits - 1; i >= 0; --i) {
            int hitR = hits[i][0];
            int hitC = hits[i][1];

            // If the original cell did not contain a brick, this hit caused no stability changes
            if (grid[static_cast<size_t>(hitR)][static_cast<size_t>(hitC)] == 0) {
                continue;
            }

            // Restore state flags inside the working grid space
            workingGrid[static_cast<size_t>(hitR)][static_cast<size_t>(hitC)]++;

            // Evaluate if restoration changes grid structure connectivity parameters
            if (workingGrid[static_cast<size_t>(hitR)][static_cast<size_t>(hitC)] == 1) {
                int preHitStableCount = dsu.getComponentSize(stableRoot);
                int currIdx = hitR * cols + hitC;

                if (hitR == 0) {
                    dsu.unionNodes(currIdx, stableRoot);
                }

                // Check and merge orthogonal neighbors
                for (int d = 0; d < 4; ++d) {
                    int adjR = hitR + deltaRow[d];
                    int adjC = hitC + deltaCol[d];

                    if (adjR >= 0 && adjR < rows && adjC >= 0 && adjC < cols) {
                        if (workingGrid[static_cast<size_t>(adjR)][static_cast<size_t>(adjC)] == 1) {
                            dsu.unionNodes(currIdx, adjR * cols + adjC);
                        }
                    }
                }

                int postHitStableCount = dsu.getComponentSize(stableRoot);
                // Subtracted difference measures newly anchored bricks (excluding the struck brick itself)
                fallingBricksCount[static_cast<size_t>(i)] = max(0, postHitStableCount - preHitStableCount - 1);
            }
        }

        return fallingBricksCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O((R * C + H) * alpha(R * C)) - Where R and C represent grid size dimensions, 
 *                  H is the total count of operations in the hits matrix, and alpha represents the inverse 
 *                  Ackermann function. Each element insertion or connectivity adjustment uses near-linear O(1) time.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - Allocated to support internal parent tracking maps, 
 *                   rank vectors, and duplicate working grid state tracking arrays.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural tracking dimensions configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix structural components row by row (0 for Empty, 1 for Brick):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    int numHits;
    cout << "Enter the absolute count number of hits to execute: ";
    cin >> numHits;
    
    vector<vector<int>> hits(static_cast<size_t>(numHits), vector<int>(2));
    cout << "Enter hit row/column indices coordinate pairs consecutively:\n";
    for (int i = 0; i < numHits; ++i) {
        cin >> hits[static_cast<size_t>(i)][0] >> hits[static_cast<size_t>(i)][1];
    }

    Solution solver;
    cout << "\nExecuting reverse timeline processing DSU tracking sweeps..." << endl;
    vector<int> results = solver.hitBricks(grid, hits);

    cout << "\nFalling bricks counts matching timeline index logs:\n";
    for (size_t i = 0; i < results.size(); ++i) {
        cout << "Hit [" << i << "]: " << results[i] << " brick(s) dropped.\n";
    }

    return 0;
}