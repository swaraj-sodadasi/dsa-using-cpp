#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
private:
    /*
     * HADLOCK'S RETROGRADE ROUTING ALGORITHM (MINIMUM DETOUR SEARCH)
     * -------------------------------------------------------------
     * Calculates the true shortest path distance between a source cell and a target cell.
     * Hadlock's algorithm optimizes standard BFS by utilizing a priority queue or double-ended 
     * queue (deque) to sort states by "detours" rather than raw distance steps.
     * 
     * A detour is defined as moving away from the target cell relative to Manhattan distance.
     * Moving along a detour increments the cost by 1, while moving closer costs 0.
     * This expands the search frontier along promising coordinates first (similar to A*).
     */
    int hadlocksRouting(const vector<vector<int>>& forest, int startR, int startC, int targetR, int targetC, int rows, int cols) {
        if (startR == targetR && startC == targetC) return 0;

        // Deque stores: {row, col, detourCost}
        // Using a 0-1 BFS variant via Deque since edge costs are strictly 0 or 1 detour units
        deque<pair<pair<int, int>, int>> dq;
        vector<vector<bool>> visited(static_cast<size_t>(rows), vector<bool>(static_cast<size_t>(cols), false));

        dq.push_front({{startR, startC}, 0});

        int deltaRow[] = {-1, 0, 1, 0};
        int deltaCol[] = {0, 1, 0, -1};

        while (!dq.empty()) {
            auto [coords, currentDetour] = dq.front();
            auto [currR, currC] = coords;
            dq.pop_front();

            if (currR == targetR && currC == targetC) {
                // Total distance = Manhattan Distance + 2 * Detour Cost
                int manhattanDist = abs(startR - targetR) + abs(startC - targetC);
                return manhattanDist + 2 * currentDetour;
            }

            if (visited[static_cast<size_t>(currR)][static_cast<size_t>(currC)]) continue;
            visited[static_cast<size_t>(currR)][static_cast<size_t>(currC)] = true;

            // Compute current Manhattan components to target
            int currentDistToTarget = abs(currR - targetR) + abs(currC - targetC);

            for (int i = 0; i < 4; ++i) {
                int nextR = currR + deltaRow[i];
                int nextC = currC + deltaCol[i];

                if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                    // Obstacle avoidance constraint
                    if (forest[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)] == 0 || 
                        visited[static_cast<size_t>(nextR)][static_cast<size_t>(nextC)]) {
                        continue;
                    }

                    int nextDistToTarget = abs(nextR - targetR) + abs(nextC - targetC);
                    
                    // A detour occurs if the step increases the distance to the target
                    if (nextDistToTarget > currentDistToTarget) {
                        dq.push_back({{nextR, nextC}, currentDetour + 1}); // Cost 1: Push to back
                    } else {
                        dq.push_front({{nextR, nextC}, currentDetour});    // Cost 0: Push to front
                    }
                }
            }
        }

        return -1; // Target is completely isolated
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-TARGET SORTED BFS VIA HADLOCK'S ROUTING
     * -------------------------------------------------------------------------------------
     * The goal is to cut down all trees in the forest in ascending order of their heights,
     * starting from position (0, 0). 0 represents an obstacle, 1 represents grass (walkable),
     * and any value greater than 1 represents a tree height.
     *
     * 1. TARGET HEIGHT COLLECTION AND SORTING:
     *    We parse the entire grid matrix to gather all coordinates containing trees (> 1).
     *    We sort these trees into a strict sequential vector pipeline based exclusively on 
     *    their heights. This forces a deterministic order of navigation.
     *
     * 2. CHAINED ADJACENT NAVIGATION SEARCH:
     *    We walk through the sorted collection, executing a shortest path calculation between 
     *    tree[i] and tree[i+1]. If any individual tree in the sequence becomes unreachable, 
     *    the entire event fails, and we immediately return -1.
     *
     * 3. EXPEDITED SPACE SEARCH OVERHEAD:
     *    Instead of running a basic unguided BFS for every single target, we embed Hadlock's 
     *    Routing Algorithm. By tracking detour units instead of total edge lengths, the search 
     *    frontier actively targets the goal, pruning hundreds of dead-end grid steps.
     */
    int cutOffTree(vector<vector<int>>& forest) {
        if (forest.empty() || forest[0].empty()) return 0;

        int rows = static_cast<int>(forest.size());
        int cols = static_cast<int>(forest[0].size());

        // Vector holds: {height, {row, col}}
        vector<pair<int, pair<int, int>>> treesList;

        // Step 1: Collect tree targets from the 2D plane
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int height = forest[static_cast<size_t>(r)][static_cast<size_t>(c)];
                if (height > 1) {
                    treesList.push_back({height, {r, c}});
                }
            }
        }

        // Sort trees in ascending order by height value
        sort(treesList.begin(), treesList.end());

        int currentR = 0;
        int currentC = 0;
        int totalStepsAccumulator = 0;

        // Step 2 & 3: Route sequentially through the sorted targets
        for (const auto& tree : treesList) {
            int targetR = tree.second.first;
            int targetC = tree.second.second;

            int pathDistance = hadlocksRouting(forest, currentR, currentC, targetR, targetC, rows, cols);
            
            if (pathDistance == -1) {
                return -1; // Path sequence broken
            }

            totalStepsAccumulator += pathDistance;
            
            // Advance traveler positions to anchor the next path iteration segment
            currentR = targetR;
            currentC = targetC;
        }

        return totalStepsAccumulator;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(T * log(T) + T * (R * C)) - Where T represents the total number of trees,
 *                  and R, C match grid dimension scopes. Sorting the list takes O(T log T). In the
 *                  worst-case scenario, Hadlock's routing pass evaluates R * C cells per tree destination.
 *
 * SPACE COMPLEXITY: O(R * C + T) Auxiliary - Dynamic allocation supports the sorted lists containing
 *                   T elements, alongside internal boolean arrays and deques keeping tracking logs inside BFS cycles.
 */
int main() {
    int rows, cols;
    cout << "Enter forest grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary configurations configured." << endl;
        return 1;
    }

    vector<vector<int>> forest(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter grid integer layout states row by row (0: Obstacle, 1: Walkable, >1: Tree Height):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> forest[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (forest[static_cast<size_t>(r)][static_cast<size_t>(c)] < 0) {
                cout << "Constraint Error: Grid element state fields cannot be negative numbers." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nCompiling target trees and executing detour routing passes..." << endl;
    int resultingCost = solver.cutOffTree(forest);

    if (resultingCost == -1) {
        cout << "Outcome: Impossible to clear all trees due to structural isolation parameters. Result = -1" << endl;
    } else {
        cout << "Minimum path navigation steps required to finish golf layout events: " << resultingCost << endl;
    }

    return 0;
}