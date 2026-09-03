#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * COMPACT PALINDROME RANGE MATRIX MAPPING (O(N^2) STATIC FILTER PRECOMPUTATION)
     * ----------------------------------------------------------------------------
     * To prevent recalculating palindrome ranges repeatedly during recursive search transitions,
     * we precompute all valid palindromic boundaries using a 2D matrix structure.
     * `isPalindromeMatrix[i][j]` maps to true if the substring `s[i...j]` is a palindrome.
     *
     * DP Transition Rule:
     *  - Base Case 1: `i == j` (Single character strings are always palindromes).
     *  - Base Case 2: `s[i] == s[j]` and `j - i == 1` (Two-character repeating pairs).
     *  - Inductive Rule: `s[i] == s[j]` and `isPalindromeMatrix[i + 1][j - 1] == true`.
     */
    vector<vector<bool>> isPalindromeMatrix;
    
    /*
     * RECURSIVE CUT OPTIMIZATION ACCELERATED WITH MEMOIZATION CACHING
     * ---------------------------------------------------------------
     * The problem asks for the minimum cuts needed to partition string `s` such that
     * every subsegment is a palindrome.
     * We define a recursive relation `findMinCuts(startIndex)` that computes the minimum 
     * cuts needed to split the suffix string `s[startIndex...N-1]`.
     *
     * State Parameters:
     *  - s: The baseline reference input string.
     *  - startIndex: The active boundary tracking marker from where evaluation splits begin.
     *  - memoCache: A 1D array caching intermediate sub-problem results to ensure O(1) retrieval.
     */
    int findMinCuts(const string& s, size_t startIndex, vector<int>& memoCache) {
        size_t n = s.length();

        // BASE CASE: CONVERGENCE SEGMENT VALIDATION
        // If the index pointer reaches or exceeds the end of the string, no additional 
        // cuts are required because the remaining suffix slice is null -> Return 0.
        if (startIndex >= n) {
            return 0;
        }

        // BASE CASE: WHOLE SUFFIX PALINDROME OPTIMIZATION
        // If the entire remaining substring from `startIndex` to the end of the string `n-1`
        // is already a palindrome, no partitioning cuts are needed -> Return 0.
        if (isPalindromeMatrix[startIndex][n - 1]) {
            return 0;
        }

        // MEMOIZATION RETRIEVAL
        // If this subsegment has been analyzed before, return its cached minimum value.
        if (memoCache[startIndex] != -1) {
            return memoCache[startIndex];
        }

        int minCutsResult = static_cast<int>(n); // Initialize with maximum possible cuts boundary

        // TRANSITION LOOP: ITERATING DISCRETE PARALLEL CUT SPLITS
        // We evaluate every index position 'endIndex' extending from 'startIndex' to 'n-1'.
        for (size_t endIndex = startIndex; endIndex < n; ++endIndex) {
            
            // PRUNING ACTION BOUNDARY: Check if prefix segment `s[startIndex...endIndex]` is valid
            if (isPalindromeMatrix[startIndex][endIndex]) {
                
                // STEP 1 & 2: RECURSE AND EVALUATE COST
                // If the left slice is a palindrome, we can place a cut right after 'endIndex'.
                // The total cost for this choice is 1 (for the current cut) plus the cost of
                // solving the remaining suffix sub-problem recursively: `findMinCuts(endIndex + 1)`.
                int currentCutCost = 1 + findMinCuts(s, endIndex + 1, memoCache);

                // STEP 3: MINIMIZE STATE OVERLAPS
                // Keep the lowest cost option among all valid branching paths.
                minCutsResult = min(minCutsResult, currentCutCost);
            }
        }

        // CACHE COMMIT: Store the minimized value in the array before returning up the stack
        memoCache[startIndex] = minCutsResult;
        return minCutsResult;
    }

    /*
     * PRECOMPUTATION UTILITY
     * ----------------------
     * Fills the 2D boolean array using a bottom-up sweep strategy, tracking from the
     * end of the string backward to ensure sub-problems are solved before their parent contexts.
     */
    void precomputePalindromes(const string& s) {
        size_t n = s.length();
        isPalindromeMatrix.assign(n, vector<bool>(n, false));

        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            for (size_t j = static_cast<size_t>(i); j < n; ++j) {
                if (s[static_cast<size_t>(i)] == s[j]) {
                    if (j - static_cast<size_t>(i) <= 2 || isPalindromeMatrix[static_cast<size_t>(i + 1)][j - 1]) {
                        isPalindromeMatrix[static_cast<size_t>(i)][j] = true;
                    }
                }
            }
        }
    }

public:
    int minCut(string s) {
        if (s.empty()) return 0;
        
        // Step 1: Initialize static validation lookups
        precomputePalindromes(s);

        // Step 2: Instantiate structural memoization vector filled with unvisited flags (-1)
        vector<int> memoCache(s.length(), -1);

        // Step 3: Trigger the recursive cut optimization framework
        return findMinCuts(s, 0, memoCache);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2) - Precomputing the palindrome lookup matrix takes O(N^2) time.
 *                  The recursive function has exactly N unique states due to memoization. 
 *                  For each state, we loop up to N times, bringing the recursion step time to O(N^2).
 *
 * SPACE COMPLEXITY: O(N^2) Auxiliary - Allocation space utilized by the 2D boolean grid mapping matrix,
 *                   combined with the memoization array and call stack depth frames bounded by O(N).
 */
int main() {
    string s;
    cout << "Enter the target string to evaluate for minimal palindrome partitioning cuts: ";
    if (!(cin >> s)) {
        cout << "Invalid input string parameter constraints." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting recursive cut optimization with dynamic programming cache checks..." << endl;
    int result = solver.minCut(s);

    cout << "Minimum partitioning splits / cuts required: " << result << endl;

    return 0;
}