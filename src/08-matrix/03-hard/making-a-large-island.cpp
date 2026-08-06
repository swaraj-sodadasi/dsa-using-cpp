#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * COMPONENT FLOOD FILL DISCOVERY ROUTINE
     * --------------------------------------
     * Explores an island component using BFS, labels all its constituent land cells 
     * with a unique component ID hashing key, and returns the total area calculated.
     */
    int floodFillComponent(vector<vector<int>>& grid, int startR, int startC, int islandId, int n) {
        queue<pair<int, int>> q;
        q.push({startR, startC});
        grid[static_cast<size_t>(startR)][static_cast<size_t>(startC)] = islandId;
        int componentArea = 0;

        int delRow[] = {-1, 0, 1, 0};
        int delCol[] = {0, 1, 0, -1};

        while (!q.empty()) {
            auto [currR, currC] = q.front();
            q.pop();
            componentArea++;

            for (int i = 0; i < 4; ++i) {
                int neighborR = currR + delRow[i];
                int neighborC = currC + delCol[i];

                if (neighborR >= 0 && neighborR < n && neighborC >= 0 && neighborC < n) {
                    if (grid[static_cast<size_t>(neighborR)][static_cast<size_t>(neighborC)] == 1) {
                        grid[static_cast<size_t>(neighborR)][static_cast<size_t>(neighborC)] = islandId;
                        q.push({neighborR, neighborC});
                    }
                }
            }
        }
        return componentArea;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: COMPONENT AREA ID HASHING + BOUNDARY EVALUATION
     * -------------------------------------------------------------------------------------
     * The goal is to maximize island area sizes by flipping at most one '0' water cell to '1'.
     *
     * 1. PRE-COMPUTED ID MAP HASHING PASS:
     *    We scan the N x N grid layout. When an unvisited land cell ('1') is encountered, we assign
     *    it a unique component `islandId` starting from 2 (since 0 and 1 are reserved). A fast BFS 
     *    flood-fill maps all matching interconnected cells to this ID and computes the island's total area. 
     *    The resulting size is indexed inside an auxiliary tracking `islandAreaMap` lookup table.
     *
     * 2. SHORELINE CROSS-BOUNDARY COMBINATORICS:
     *    We perform a second systematic sweep tracking all water cells ('0'). For each water cell, 
     *    we inspect its 4 orthogonal neighboring cells. We gather their structural `islandId` tags 
     *    into an `unordered_set` to prevent double-counting the same component if it touches multiple sides.
     *    We then sum the hashed areas of these unique neighboring islands plus 1 (the flipped water cell itself).
     *
     * 3. REDUCTION UPPER BOUND EXTENSION:
     *    We maintain a running global maximum of these calculated configurations to return the absolute answer.
     */
    int largestIsland(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int n = static_cast<int>(grid.size());
        unordered_map<int, int> islandAreaMap;
        int islandIdCounter = 2;
        int maxIslandSize = 0;

        // Step 1: Discover connected land masses and construct the area lookup registry
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1) {
                    int area = floodFillComponent(grid, r, c, islandIdCounter, n);
                    islandAreaMap[islandIdCounter] = area;
                    maxIslandSize = max(maxIslandSize, area);
                    islandIdCounter++;
                }
            }
        }

        int delRow[] = {-1, 0, 1, 0};
        int delCol[] = {0, 1, 0, -1};

        // Step 2 & 3: Run cross-boundary intersection checking on every water node position
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 0) {
                    unordered_set<int> neighboringIslands;
                    int potentialMergedArea = 1; 

                    for (int i = 0; i < 4; ++i) {
                        int adjR = r + delRow[i];
                        int adjC = c + delCol[i];

                        if (adjR >= 0 && adjR < n && adjC >= 0 && adjC < n) {
                            int neighborId = grid[static_cast<size_t>(adjR)][static_cast<size_t>(adjC)];
                            if (neighborId > 1) {
                                neighboringIslands.insert(neighborId);
                            }
                        }
                    }

                    for (int id : neighboringIslands) {
                        potentialMergedArea += islandAreaMap[id];
                    }

                    maxIslandSize = max(maxIslandSize, potentialMergedArea);
                }
            }
        }

        return maxIslandSize;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2) - Where N represents the row/column layout length scale. The component 
 *                  discovery sweeps visit each node a bounded number of times, and the subsequent 
 *                  boundary lookup pass verifies 4 orthogonal sides per water cell, keeping total 
 *                  runtime strictly linear relative to matrix dimensions.
 *
 * SPACE COMPLEXITY: O(N^2) - Consumed by the internal BFS tracking FIFO queues in the worst-case 
 *                   propagation scenario, alongside the mapping hash space containing the pre-computed component sizes.
 */
int main() {
    int n;
    cout << "Enter the layout dimension size capacity (N) for the square grid matrix: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid boundary grid configuration options entered." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n)));
    cout << "Enter binary matrix elements row by row (0 for Water, 1 for Land):\n";
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != 0 && 
                grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != 1) {
                cout << "Constraint Error: Grid element configurations must be strictly 0 or 1." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting component hashing and cross-boundary validation sweeps..." << endl;
    int optimalSizeResult = solver.largestIsland(grid);

    cout << "Calculated maximum size area threshold achievable: " << optimalSizeResult << endl;

    return 0;
}