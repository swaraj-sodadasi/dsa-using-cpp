#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>

using namespace std;

struct Tree {
    int height;
    int r;
    int c;

    bool operator<(const Tree& other) const {
        return height < other.height;
    }
};

class Solution {
private:
    /*
     * Hadlock's Algorithm (0-1 BFS) Pathfinder:
     * Computes the shortest path from (sr, sc) to (tr, tc) on a grid with obstacles (0).
     * 
     * Hadlock's algorithm reformulates the shortest path search on a grid by counting
     * "detours" (steps that move away from the target).
     *
     * Let M(A, T) be the Manhattan distance between node A and target T.
     * When moving from current cell S to adjacent cell N:
     * - If we move closer to the target, M(N, T) = M(S, T) - 1. Detour cost = 0.
     * - If we move away from the target, M(N, T) = M(S, T) + 1. Detour cost = 1.
     * 
     * Formula for detour cost: d = (M(N, T) - M(S, T) + 1) / 2
     * 
     * Since edge weights are strictly 0 or 1, we can use a double-ended queue (deque):
     * - Nodes reached via 0-cost edges are pushed to the front.
     * - Nodes reached via 1-cost edges are pushed to the back.
     * 
     * Total shortest path distance = Manhattan_Distance(Start, Target) + 2 * Detour_Count.
     */
    int hadlock(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc, int rows, int cols) {
        if (sr == tr && sc == tc) return 0;

        auto getManhattan = [](int r1, int c1, int r2, int c2) {
            return abs(r1 - r2) + abs(c1 - c2);
        };

        // 2D distance matrix to store detour counts. Initialize with -1 (unvisited).
        vector<vector<int>> detours(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols), -1));
        
        // Deque stores coordinates {row, col}
        deque<pair<int, int>> dq;
        
        dq.push_front({sr, sc});
        detours[static_cast<size_t>(sr)][static_cast<size_t>(sc)] = 0;

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        while (!dq.empty()) {
            auto [r, c] = dq.front();
            dq.pop_front();

            int currDetour = detours[static_cast<size_t>(r)][static_cast<size_t>(c)];

            if (r == tr && c == tc) {
                return getManhattan(sr, sc, tr, tc) + 2 * currDetour;
            }

            int distToTarget = getManhattan(r, c, tr, tc);

            for (int i = 0; i < 4; ++i) {
                int nr = r + dRow[i];
                int nc = c + dCol[i];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[static_cast<size_t>(nr)][static_cast<size_t>(nc)] > 0) {
                    int nextDistToTarget = getManhattan(nr, nc, tr, tc);
                    
                    // Detour formula: 0 if moving closer, 1 if moving away
                    int detourCost = (nextDistToTarget - distToTarget + 1) / 2;
                    int nextDetour = currDetour + detourCost;

                    // If unvisited or found a path with fewer detours
                    if (detours[static_cast<size_t>(nr)][static_cast<size_t>(nc)] == -1 || 
                        nextDetour < detours[static_cast<size_t>(nr)][static_cast<size_t>(nc)]) {
                        
                        detours[static_cast<size_t>(nr)][static_cast<size_t>(nc)] = nextDetour;

                        if (detourCost == 0) {
                            dq.push_front({nr, nc});
                        } else {
                            dq.push_back({nr, nc});
                        }
                    }
                }
            }
        }

        return -1; // Target unreachable
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CUT OFF TREES FOR GOLF EVENT (HADLOCK'S ROUTING & MULTI-TARGET SORTED BFS)
     * -------------------------------------------------------------------------------------
     * The objective is to cut off all trees (values > 1) in sorted order of their heights,
     * starting from (0,0). Obstructed cells are denoted by 0.
     * 
     * 1. STATE IDENTIFICATION & SORTING:
     *    We collect all cells containing a tree (height > 1), and sort them in ascending 
     *    order based on their heights.
     * 
     * 2. SEQUENTIAL TRANSITION PATHFINDING:
     *    We iteratively compute the shortest path from our current position to the next 
     *    sorted target tree.
     * 
     * 3. HADLOCK'S ROUTING HEURISTIC:
     *    Instead of executing a standard BFS for every transition (which can run up to 
     *    O(T * R * C) where T is the number of trees), we implement Hadlock's 0-1 BFS 
     *    Heuristic to drastically reduce the search space by preferring directions pointing 
     *    toward the target.
     */
    int cutOffTree(const vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        vector<Tree> trees;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int val = grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
                if (val > 1) {
                    trees.push_back({val, r, c});
                }
            }
        }

        // Sort trees by height
        sort(trees.begin(), trees.end());

        int currentR = 0;
        int currentC = 0;
        int totalSteps = 0;

        for (const auto& tree : trees) {
            int steps = hadlock(grid, currentR, currentC, tree.r, tree.c, rows, cols);
            if (steps == -1) {
                return -1; // Unreachable tree makes the golf course unsolvable
            }
            totalSteps += steps;
            currentR = tree.r;
            currentC = tree.c;
        }

        return totalSteps;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(T * R * C) - Where T is the number of trees to cut, and R, C are 
 *                  grid boundaries. In the worst case, each step in our traversal 
 *                  resolves a Hadlock path check taking O(R * C) states.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - To hold the detour status matrices 
 *                   and double-ended queue allocations.
 */
int main() {
    cout << "=== Cut Off Trees for Golf Event (Hadlock's 0-1 BFS Router) ===\n";
    cout << "Enter grid dimensions (rows columns): ";
    int rows, cols;
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) return 1;

    vector<vector<int>> grid(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter the grid layout sequentially (0 = Wall, 1 = Empty, >1 = Tree with height):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nResolving target heights and routing sequential paths..." << endl;
    int steps = solver.cutOffTree(grid);

    if (steps != -1) {
        cout << "Minimum step transitions required to clear all trees: [" << steps << "]\n";
    } else {
        cout << "It is impossible to cut off all trees. Output: [-1]\n";
    }

    return 0;
}