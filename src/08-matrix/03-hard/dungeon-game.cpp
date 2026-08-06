#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: REVERSE DYNAMIC PROGRAMMING TARGET HP TRAVERSAL
     * -------------------------------------------------------------------------------------
     * The objective is to determine the knight's minimum initial health required to reach the
     * bottom-right room from the top-left room. The knight dies if health drops to 0 or below.
     * 
     * 1. THE NEED FOR REVERSE TRAVERSAL:
     *    A forward DP approach from top-left to bottom-right fails because the minimum health 
     *    required at a cell depends not only on the path history, but also heavily on the 
     *    downstream future paths. By reversing the process and traversing backward from the 
     *    destination `(M-1, N-1)` up to the source `(0, 0)`, we can dynamically compute the 
     *    minimum health needed to *survive the rest of the journey* from any cell.
     * 
     * 2. STATE TRANSITION RELATION:
     *    Let `dp[r][c]` represent the minimum health required *before* entering room `(r, c)`.
     *    To survive, the knight must move to either the right cell `(r, c+1)` or the bottom cell `(r+1, c)`.
     *    Therefore, the health needed upon leaving cell `(r, c)` is `min(dp[r][c+1], dp[r+1][c])`.
     *    Accounting for the dungeon's impact (damage or heal) at the current cell:
     *    `healthNeeded = min(dp[r][c+1], dp[r+1][c]) - dungeon[r][c]`
     *    Since health must always be at least 1, if `healthNeeded <= 0`, we clamp it to 1:
     *    `dp[r][c] = max(1, healthNeeded)`
     * 
     * 3. SPACE OPTIMIZATION (1D Vector Row/Column Cache):
     *    Computing the current cell requires values only from the step immediately to the right 
     *    and the step immediately below. We optimize memory allocations to a single 1D vector 
     *    cache array of size `N + 1` (where N is the number of columns).
     */
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) return 1;

        int rows = static_cast<int>(dungeon.size());
        int cols = static_cast<int>(dungeon[0].size());

        // Allocate space-optimized 1D tracking DP table with an extra boundary cell
        // Initialized to a large number representing an unreachable boundary layout state
        vector<int> dp(static_cast<size_t>(cols + 1), 1e9);
        
        // Base case setup: The target placeholder element below the bottom-right room is set to 1
        dp[static_cast<size_t>(cols - 1)] = 1;

        // Run reverse dynamic traversal loops from bottom-right up to top-left
        for (int r = rows - 1; r >= 0; --r) {
            for (int c = cols - 1; c >= 0; --c) {
                // Determine the minimum future path health requirement between down and right choices
                int nextHealthNeeded = min(dp[static_cast<size_t>(c)], dp[static_cast<size_t>(c + 1)]);
                
                // Account for current room penalty/bonus, clamping the absolute baseline floor to 1 HP
                dp[static_cast<size_t>(c)] = max(1, nextHealthNeeded - dungeon[static_cast<size_t>(r)][static_cast<size_t>(c)]);
            }
            // Reset the boundary pad item to infinity after each row finish to prevent vertical leakage
            dp[static_cast<size_t>(cols)] = 1e9;
        }

        return dp[0];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * N) - Where M represents row count and N represents column count of the grid layout.
 *                  The matrix parsing pass evaluates each dungeon coordinate cell exactly once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Space-optimized architecture drops the classic grid allocations,
 *                   preserving only a single linear row cache tracking vector of size N + 1.
 */
int main() {
    int rows, cols;
    cout << "Enter dungeon grid dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid structural parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> dungeon(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter room modifier values row by row (Negative for traps, Positive for health pools):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> dungeon[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nExecuting reverse dynamic survival strategy traversal..." << endl;
    int minimumInitialHP = solver.calculateMinimumHP(dungeon);

    cout << "Absolute minimum initial health points required to survive: " << minimumInitialHP << endl;

    return 0;
}