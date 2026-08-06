#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

class Solution {
private:
    // Region structure to store metrics for each isolated infected area
    struct Region {
        int freshInfectedCount = 0; // The number of uninfected cells this region will threaten next turn
        int perimeterWallsNeeded = 0; // The number of quarantine walls required to isolate this region
        vector<pair<int, int>> cells; // Core coordinates of the virus cluster cells
        vector<pair<int, int>> threatenedCells; // Distinct fresh cells neighboring this region
    };

    int rows, cols;
    int deltaRow[4] = {-1, 0, 1, 0};
    int deltaCol[4] = {0, 1, 0, -1};

    // Computes a flat unique integer hash from 2D coordinates
    inline int getHash(int r, int c) {
        return r * cols + c;
    }

    /*
     * VIRUS COMPONENT FLOOD FILL SCANNER
     * ----------------------------------
     * Explores an unvisited infected region using BFS, calculating its 
     * precise cell footprints, threatened neighbors, and boundary perimeter requirements.
     */
    Region discoverRegion(int startR, int startC, vector<vector<int>>& grid, vector<vector<bool>>& visited) {
        Region reg;
        queue<pair<int, int>> q;
        unordered_set<int> uniquelyThreatened;

        q.push({startR, startC});
        visited[static_cast<size_t>(startR)][static_cast<size_t>(startC)] = true;

        while (!q.empty()) {
            auto [currR, currC] = q.front();
            q.pop();
            reg.cells.push_back({currR, currC});

            for (int i = 0; i < 4; ++i) {
                int adjR = currR + deltaRow[i];
                int adjC = currC + deltaCol[i];

                if (adjR >= 0 && adjR < rows && adjC >= 0 && adjC < cols) {
                    if (grid[static_cast<size_t>(adjR)][static_cast<size_t>(adjC)] == 1) {
                        if (!visited[static_cast<size_t>(adjR)][static_cast<size_t>(adjC)]) {
                            visited[static_cast<size_t>(adjR)][static_cast<size_t>(adjC)] = true;
                            q.push({adjR, adjC});
                        }
                    } else if (grid[static_cast<size_t>(adjR)][static_cast<size_t>(adjC)] == 0) {
                        // A wall is needed on this boundary side regardless of whether the cell is shared
                        reg.perimeterWallsNeeded++;
                        
                        // Count unique threatened cells to measure danger growth metrics
                        int cellHash = getHash(adjR, adjC);
                        if (uniquelyThreatened.find(cellHash) == uniquelyThreatened.end()) {
                            uniquelyThreatened.insert(cellHash);
                            reg.threatenedCells.push_back({adjR, adjC});
                        }
                    }
                }
            }
        }
        reg.freshInfectedCount = static_cast<int>(reg.threatenedCells.size());
        return reg;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SIMULATION TRACKING, PERIMETER MULTIPLICATION & AREA ENCAPSULATION
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the total number of quarantine walls needed to contain a 
     * spreading virus. Each turn, you block exactly ONE infected region by surrounding it completely
     * with walls. The uncontained regions then expand into all adjacent uninfected cells.
     *
     * 1. MULTI-COMPONENT DISCOVERY PASS (EACH ROUND):
     *    We sweep the grid to identify all active viral components using BFS. For each cluster, 
     *    we calculate the exact wall count needed (`perimeterWallsNeeded`) and track the set of 
     *    uninfected cells it threatens (`freshInfectedCount`).
     *
     * 2. GREEDY QUARANTINE CHOICE:
     *    We locate the most dangerous region (the one that threatens the highest number of unique 
     *    uninfected cells). If no regions threaten fresh cells, the simulation finishes. We build walls 
     *    around this chosen region, locking its cells in-place with a value of '2' (quarantined/dead).
     *
     * 3. INFECTION SPREAD EMULATION:
     *    For all other uncontained regions, we loop through their threatened coordinate registers and 
     *    change those grid values from 0 to 1. The simulation loops until no active regions remain.
     */
    int containVirus(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        rows = static_cast<int>(grid.size());
        cols = static_cast<int>(grid[0].size());
        int totalWallsBuilt = 0;

        while (true) {
            vector<vector<bool>> visited(static_cast<size_t>(rows), vector<bool>(static_cast<size_t>(cols), false));
            vector<Region> regions;

            // Step 1: Discover all isolated virus clusters and compile their parameters
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == 1 && !visited[static_cast<size_t>(r)][static_cast<size_t>(c)]) {
                        regions.push_back(discoverRegion(r, c, grid, visited));
                    }
                }
            }

            // Termination condition: No active infected regions found
            if (regions.empty()) break;

            // Step 2: Find the region threatening the maximum number of uninfected cells
            int primaryTargetIdx = -1;
            int maxThreatenedCells = 0;

            for (size_t i = 0; i < regions.size(); ++i) {
                if (regions[i].freshInfectedCount > maxThreatenedCells) {
                    maxThreatenedCells = regions[i].freshInfectedCount;
                    primaryTargetIdx = static_cast<int>(i);
                }
            }

            // If no active regions pose an immediate expansion threat, the virus is contained
            if (primaryTargetIdx == -1) break;

            // Enclose the target region: accumulate walls and mark cells as quarantined (2)
            totalWallsBuilt += regions[static_cast<size_t>(primaryTargetIdx)].perimeterWallsNeeded;
            for (const auto& cell : regions[static_cast<size_t>(primaryTargetIdx)].cells) {
                grid[static_cast<size_t>(cell.first)][static_cast<size_t>(cell.second)] = 2;
            }

            // Step 3: Simulate the growth expansion of all remaining uncontained virus clusters
            for (size_t i = 0; i < regions.size(); ++i) {
                if (static_cast<int>(i) == primaryTargetIdx) continue;
                for (const auto& cell : regions[i].threatenedCells) {
                    grid[static_cast<size_t>(cell.first)][static_cast<size_t>(cell.second)] = 1;
                }
            }
        }

        return totalWallsBuilt;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O((R * C)^2) - Where R and C represent grid length limits. In the absolute worst case, 
 *                  a round isolates only a small piece while the remaining cells get populated. The grid 
 *                  can take up to O(R * C) rounds to fill, and each round sweeps the cells via BFS in O(R * C) time.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - Consumed by structural BFS queues, visited status trackers, 
 *                   and hash tables tracking regional boundary cells within simulation turns.
 */
int main() {
    int inputRows, inputCols;
    cout << "Enter viral threat grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> inputRows >> inputCols) || inputRows <= 0 || inputCols <= 0) {
        cout << "Invalid boundary configurations configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(inputRows), vector<int>(static_cast<size_t>(inputCols)));
    cout << "Enter grid status parameters row by row (0: Uninfected, 1: Infected):\n";
    for (int r = 0; r < inputRows; ++r) {
        for (int c = 0; c < inputCols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != 0 && 
                grid[static_cast<size_t>(r)][static_cast<size_t>(c)] != 1) {
                cout << "Constraint Error: Element values must be strictly 0 or 1." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nLaunching grid perimeter simulation tracking loops..." << endl;
    int calculatedWalls = solver.containVirus(grid);

    cout << "Total quarantine wall units required to successfully contain the infection: " << calculatedWalls << endl;

    return 0;
}