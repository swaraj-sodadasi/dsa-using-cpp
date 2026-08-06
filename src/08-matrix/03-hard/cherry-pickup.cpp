#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SIMULTANEOUS TWO-PATH 3D MATRIX DYNAMIC PROGRAMMING COORDINATION
     * -------------------------------------------------------------------------------------
     * The objective is to maximize collected cherries by traveling from (0,0) to (N-1,N-1) 
     * and returning to (0,0). A thorn ('-1') blocks passage, and cherries ('1') become empty ('0') once picked.
     * 
     * 1. RE-FRAMING AS TWO SIMULTANEOUS FORWARD PATHS:
     *    A sequential approach (Path 1 down, then Path 2 up) fails because greedy choices in Path 1 
     *    can destroy optimal configurations for Path 2. Instead, we model this as **two separate travelers 
     *    moving forward simultaneously** from (0,0) to (N-1,N-1).
     * 
     * 2. STEP FOOTPRINT REDUCTION (3D STATE COMPRESSION):
     *    Let Traveler 1 be at $(r_1, c_1)$ and Traveler 2 be at $(r_2, c_2)$. Because both move 1 step 
     *    simultaneously per tick, their Manhattan distance tracking step counts match: $r_1 + c_1 = r_2 + c_2 = step$.
     *    This mathematical invariant allows us to deduce $c_2$ dynamically: $c_2 = r_1 + c_1 - r_2$.
     *    Thus, our state collapses beautifully from 4D down into a 3D matrix array workspace: `dp[r1][c1][r2]`.
     * 
     * 3. STATE TRANSITION & CONCURRENCY CONSTRAINTS:
     *    From any valid step framework, there are four potential concurrent directional combination paths:
     *    - Traveler 1 moves Down, Traveler 2 moves Down (DD): `dp[r1+1][c1][r2+1]`
     *    - Traveler 1 moves Down, Traveler 2 moves Right (DR): `dp[r1+1][c1][r2]`
     *    - Traveler 1 moves Right, Traveler 2 moves Down (RD): `dp[r1][c1+1][r2+1]`
     *    - Traveler 1 moves Right, Traveler 2 moves Right (RR): `dp[r1][c1+1][r2]`
     *    
     *    If both travelers land on the exact same coordinate cell $(r_1 == r_2)$, they only count the 
     *    cherry once to respect structural allocation limits. Otherwise, they accumulate both.
     */
    int cherryPickup(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int n = static_cast<int>(grid.size());
        
        // Initialize 3D tracking DP lookup buffer filled with INT_MIN (unreachable)
        // Dimensions: n x n x n
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(n, INT_MIN)));
        
        // Base case setup at the entry coordinate origin point
        dp[0][0][0] = grid[0][0];

        // Systematically iterate through matching temporal combinations of steps
        for (int r1 = 0; r1 < n; ++r1) {
            for (int c1 = 0; c1 < n; ++c1) {
                for (int r2 = 0; r2 < n; ++r2) {
                    int c2 = r1 + c1 - r2;

                    // Filter out invalid inferred grid tracking dimensions
                    if (c2 < 0 || c2 >= n || dp[r1][c1][r2] == INT_MIN) {
                        continue;
                    }

                    // Look ahead and transition into the next valid 3D coordinate layer states
                    // Try pushing state transitions out to the 4 downstream trajectory configurations
                    for (int d1 = 0; d1 < 2; ++d1) {      // 0: Down, 1: Right
                        for (int d2 = 0; d2 < 2; ++d2) {  // 0: Down, 1: Right
                            int nextR1 = r1 + (d1 == 0 ? 1 : 0);
                            int nextC1 = c1 + (d1 == 1 ? 1 : 0);
                            int nextR2 = r2 + (d2 == 0 ? 1 : 0);
                            int nextC2 = c2 + (d2 == 1 ? 1 : 0);

                            // Verify boundaries and ensure neither path collides with a thorn barrier ('-1')
                            if (nextR1 < n && nextC1 < n && nextR2 < n && nextC2 < n &&
                                grid[static_cast<size_t>(nextR1)][static_cast<size_t>(nextC1)] != -1 &&
                                grid[static_cast<size_t>(nextR2)][static_cast<size_t>(nextC2)] != -1) {

                                int cherriesGained = grid[static_cast<size_t>(nextR1)][static_cast<size_t>(nextC1)];
                                if (nextR1 != nextR2) { // Add traveler 2's distinct share if they are in separate columns
                                    cherriesGained += grid[static_cast<size_t>(nextR2)][static_cast<size_t>(nextC2)];
                                }

                                dp[nextR1][nextC1][nextR2] = max(dp[nextR1][nextC1][nextR2], dp[r1][c1][r2] + cherriesGained);
                            }
                        }
                    }
                }
            }
        }

        int finalMaxResult = dp[static_cast<size_t>(n - 1)][static_cast<size_t>(n - 1)][static_cast<size_t>(n - 1)];
        return max(0, finalMaxResult);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^3) - Where N represents the width dimension boundary size of the square grid matrix.
 *                  The state space loop evaluates combinations bounded by the 3D variable dimensions.
 *
 * SPACE COMPLEXITY: O(N^3) Auxiliary - Determined by the memory footprint footprint of our 
 *                   pre-allocated 3D dynamic programming coordination matrix array structure.
 */
int main() {
    int n;
    cout << "Enter the layout dimension size capacity (N) for the square grid matrix: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid structural boundary configurations configured." << endl;
        return 1;
    }

    vector<vector<int>> grid(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n)));
    cout << "Enter matrix values row by row (0: Empty, 1: Cherry, -1: Thorn block):\n";
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            cin >> grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
            if (grid[static_cast<size_t>(r)][static_cast<size_t>(c)] < -1 || 
                grid[static_cast<size_t>(r)][static_cast<size_t>(c)] > 1) {
                cout << "Constraint Error: Element values must be strictly -1, 0, or 1." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting concurrent dual-path 3D dynamic coordination walks..." << endl;
    int resultingCherries = solver.cherryPickup(grid);

    cout << "Maximum accumulated cherry volume harvested along path configurations: " << resultingCherries << endl;

    return 0;
}