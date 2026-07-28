#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * RECURSIVE DEPTH-FIRST SEARCH EXTENSION (CORE SEARCH SPACE TRAVERSAL)
     * ------------------------------------------------------------------
     * This helper function systematically explores the configuration space to see if 
     * the remaining matchsticks can fit into the four bucket boundaries.
     *
     * State Parameters:
     *  - matchsticks: The input list of stick lengths, sorted in descending order.
     *  - index: The array tracking marker identifying which matchstick we are currently processing.
     *  - sides: An array of size 4 tracking the current accumulated length of each side.
     *  - targetSideLength: The required uniform length target for each side of the square.
     */
    bool dfs(const vector<int>& matchsticks, size_t index, vector<int>& sides, int targetSideLength) {
        
        // ==========================================
        // BASE CASE: SUCCESS CONVERGENCE REACHED
        // ==========================================
        // If our index pointer matches the size of the array, it means every single 
        // matchstick has been successfully placed into a valid side without exceeding 
        // the target constraints. A valid square configuration is confirmed.
        if (index == matchsticks.size()) {
            return true;
        }

        // ==========================================
        // TRANSITION LOOP: CARDINAL SIDE BRANCHING
        // ==========================================
        // For the current matchstick at 'index', we attempt to place it into each of 
        // the 4 candidate slots (representing the four unique perimeter segments).
        for (size_t i = 0; i < 4; ++i) {
            
            // -------------------------------------------------------------
            // PRUNING OPTIMIZATION: DUPLICATE SIDE ELIMINATION
            // -------------------------------------------------------------
            // If the current side we are evaluating has the exact same length as the 
            // previous side we just checked in this frame, we skip it.
            // Why? Because trying to place the active matchstick into an identical 
            // capacity window creates a redundant search branch. If it failed in the 
            // first one, it will yield the exact same failure in the clone path.
            if (i > 0 && sides[i] == sides[i - 1]) {
                continue;
            }

            // -------------------------------------------------------------
            // BOUNDARY CONSTRAINT VALIDATION
            // -------------------------------------------------------------
            // Check if adding the current matchstick length causes the selected side 
            // to overshoot the targetSideLength limit. If it overshoots, we prune early.
            if (sides[i] + matchsticks[index] <= targetSideLength) {
                
                // STEP 1: CHOOSE (Provisionally allocate length resources)
                sides[i] += matchsticks[index];

                // STEP 2: RECURSE (Advance down the decision tree for the next stick)
                if (dfs(matchsticks, index + 1, sides, targetSideLength)) {
                    return true; // Propagation link: bubble 'true' up if a leaf succeeded
                }

                // STEP 3: UNCHOOSE / BACKTRACK (Revert allocation to clean up state)
                // If the recursive call returned false, it implies that this branch led 
                // to a dead end down the line. We subtract the value and try alternative sides.
                sides[i] -= matchsticks[index];
            }
        }

        // FAILURE RESOLUTION: If the stick cannot fit cleanly into any side, return false
        return false;
    }

public:
    /*
     * INITIAL MATHEMATICAL VALIDATION AND OPTIMIZATION SETUP
     * ------------------------------------------------------
     * Performs structural consistency assertions, applies sorting heuristics, 
     * sets up structural trackers, and triggers the recursive backbone.
     */
    bool makesquare(vector<int>& matchsticks) {
        // Geometric Restriction Check: A square must contain at least 4 individual bounds segments.
        if (matchsticks.size() < 4) {
            return false;
        }

        // Accumulate total perimeter size using 64-bit integer space to safely protect against overflow bugs.
        long long totalLength = accumulate(matchsticks.begin(), matchsticks.end(), 0LL);
        
        // Geometric Divisibility Constraint: A true square must have a perimeter 
        // perfectly divisible by 4. If a remainder exists, equal distribution is impossible.
        if (totalLength % 4 != 0) {
            return false;
        }

        int targetSideLength = static_cast<int>(totalLength / 4);

        // -------------------------------------------------------------
        // HEURISTIC OPTIMIZATION: SEARCH ORDER SORTING (DESCENDING)
        // -------------------------------------------------------------
        // Sorting the array in descending order is our most impactful heuristic.
        // By handling the largest matchsticks first, the algorithm rapidly fills up side 
        // capacities. If a large matchstick cannot fit, the code triggers a failure state 
        // high up in the recursive tree, saving millions of unnecessary branch evaluations.
        sort(matchsticks.rbegin(), matchsticks.rend());

        // Baseline Magnitude Constraint Check: If any single matchstick has a length 
        // greater than the target segment dimension, it can never participate in a valid solution.
        if (matchsticks[0] > targetSideLength) {
            return false;
        }

        // Instantiation of the 4 independent tracker sides initialized to zero length capacity.
        vector<int> sides(4, 0);

        // Execute search space traversal starting at index 0
        return dfs(matchsticks, 0, sides, targetSideLength);
    }
};

/*
 * COMPLEXITY ANALYTICAL METRICS:
 * ------------------------------
 * TIME COMPLEXITY: O(4^N) Worst-Case. Each of the N matchsticks can branch out into 
 *                  up to 4 independent path options. However, due to the descending sort 
 *                  ordering and duplicate structural state pruning, the active execution footprint 
 *                  is pruned drastically, allowing it to complete within milliseconds.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary. Bounded by the maximum execution depth context of the 
 *                   runtime call stack, which reaches a peak depth of exactly N frames.
 */
int main() {
    int n;
    cout << "Enter the total number of matchsticks: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid count parameter value input entry." << endl;
        return 1;
    }

    vector<int> matchsticks(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter the length of each matchstick separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> matchsticks[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nExecuting recursive search order sorting optimization check..." << endl;
    
    if (solver.makesquare(matchsticks)) {
        cout << "Result: Yes! The matchsticks can be arranged to form a valid square." << endl;
    } else {
        cout << "Result: No! The matchsticks CANNOT be arranged to form a square." << endl;
    }

    return 0;
}