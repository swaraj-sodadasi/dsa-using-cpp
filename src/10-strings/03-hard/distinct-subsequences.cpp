#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: TWO-DIMENSIONAL DYNAMIC PROGRAMMING PATTERN MATCHING
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the number of distinct subsequences of string `s` 
     * that equal string `t`.
     * 
     * 1. THE ARCHITECTURAL DP STATE CONTRACT:
     *    Let `dp[i][j]` represent the total number of distinct subsequences of `s[0...i-1]` 
     *    that match the prefix pattern `t[0...j-1]`.
     * 
     * 2. STATE TRANSITION RULES:
     *    For every pair of indices `i` and `j`:
     *      - Case A (Always Applicable): We can always choose to ignore the current character 
     *        `s[i-1]` and see how many ways the previous segment `s[0...i-2]` could form `t[0...j-1]`. 
     *        This inherits `dp[i-1][j]`.
     *      - Case B (Character Match): If `s[i-1] == t[j-1]`, we gain an extra choice. We can 
     *        additionally match these two characters, adding the number of ways to form the 
     *        previous prefix target `dp[i-1][j-1]` to our current count.
     *    
     *    Equation: `dp[i][j] = dp[i-1][j] + (s[i-1] == t[j-1] ? dp[i-1][j-1] : 0)`
     * 
     * 3. SPACE COMPACTION OPTIMIZATION (2D Matrix to 1D Row Vector):
     *    Notice that calculating `dp[i][j]` only requires values from the previous row `i-1`. 
     *    Instead of allocating a massive 2D array that causes cache misses and high memory footprints, 
     *    we compress the grid into a single 1D vector `dp` of size `M + 1` (where M is the length of `t`).
     *    By iterating backward from `M` down to `1`, we ensure that `dp[j-1]` represents the value 
     *    from the previous row execution step, preventing new values from overwriting historic data.
     * 
     *    *Note: Large inputs can cause results to exceed standard integer bounds. We use `unsigned long long` 
     *    to prevent data overflow errors during accumulation.*
     */
    int numDistinct(string s, string t) {
        size_t n = s.length();
        size_t m = t.length();

        // Base Boundary Condition: A longer target cannot be formed from a shorter source string
        if (n < m) return 0;

        // Step 1: Initialize a 1D state array tracking vector. 
        // Base Case: dp[0] = 1, since there is exactly 1 way to form an empty target string "".
        vector<unsigned long long> dp(m + 1, 0);
        dp[0] = 1;

        // Step 2: Run the dynamic programming sequence loops
        for (size_t i = 1; i <= n; ++i) {
            // Iterate backward to protect row indices from being overwritten prematurely
            for (size_t j = m; j >= 1; --j) {
                if (s[i - 1] == t[j - 1]) {
                    dp[j] += dp[j - 1];
                }
            }
        }

        return static_cast<int>(dp[m]);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M) - Where N represents the length of string s, and M represents the length 
 *                  of string t. The nested execution loop updates state records across all variations.
 *
 * SPACE COMPLEXITY: O(M) Auxiliary - Space compaction cuts memory requirements down from an O(N * M) matrix 
 *                   to a single row tracker array mapped strictly to the target string length.
 */
int main() {
    string sourceS, targetT;
    cout << "=== Distinct Subsequences Dynamic Programming Engine ===\n";
    cout << "Enter the primary source string (s): ";
    if (!(cin >> sourceS)) return 1;
    cout << "Enter the target subsequence pattern to match (t): ";
    if (!(cin >> targetT)) return 1;

    Solution solver;
    cout << "\nAllocating compressed state arrays and analyzing matching permutations..." << endl;
    int matchesCalculated = solver.numDistinct(sourceS, targetT);

    cout << "\nTotal distinct matching subsequence occurrences: " << matchesCalculated << "\n";

    return 0;
}