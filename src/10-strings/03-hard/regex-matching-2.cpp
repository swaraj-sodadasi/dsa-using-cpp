#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: REGULAR EXPRESSION MATCHING (TWO-DIMENSIONAL DYNAMIC PROGRAMMING)
     * -------------------------------------------------------------------------------------
     * The objective is to evaluate if a string `s` matches a pattern `p` supporting:
     *   - '.' matches any single character.
     *   - '*' matches zero or more of the preceding character element.
     * 
     * 1. THE ARCHITECTURAL STATE MATRIX:
     *    Let `dp[i][j]` be a boolean flag representing whether the prefix `s[0...i-1]` 
     *    matches the pattern prefix `p[0...j-1]`.
     * 
     * 2. STATE TRANSITION MATRIX RULES:
     *    For each index pair `i` (from 0 to N) and `j` (from 1 to M):
     *      - Case A (Standard Char or '.'): If `p[j-1]` is not '*', then we require the current 
     *        characters to match (`s[i-1] == p[j-1]` or `p[j-1] == '.'`) AND the previous prefixes 
     *        must match.
     *        Equation: `dp[i][j] = i > 0 && dp[i-1][j-1] && (s[i-1] == p[j-1] || p[j-1] == '.')`
     * 
     *      - Case B (Wildcard '*' Processing): If `p[j-1]` is '*', it can act in two branches:
     *        1. Zero occurrences: Ignore the '*' and its preceding character entirely. 
     *           Inherits `dp[i][j-2]`.
     *        2. One or more occurrences: If the preceding character `p[j-2]` matches `s[i-1]` 
     *           (or is '.'), the pattern can remain stationary while we consume the character in `s`.
     *           Inherits `dp[i-1][j]`.
     *        Equation: `dp[i][j] = dp[i][j-2] || (i > 0 && dp[i-1][j] && (s[i-1] == p[j-2] || p[j-2] == '.'))`
     */
    bool isMatch(string s, string p) {
        size_t sLen = s.length();
        size_t pLen = p.length();

        // Step 1: Allocate a 2D state matrix table initialized to false
        // Dimensions: (sLen + 1) rows x (pLen + 1) columns to accommodate empty string spaces
        vector<vector<bool>> dp(sLen + 1, vector<bool>(pLen + 1, false));

        // Base Case: An empty string matches an empty pattern
        dp[0][0] = true;

        // Populate matches for empty string 's' against patterns containing wildcards (e.g., "a*", "a*b*")
        for (size_t j = 2; j <= pLen; ++j) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 2];
            }
        }

        // Step 2: Execute global grid evaluation loops
        for (size_t i = 0; i <= sLen; ++i) {
            for (size_t j = 1; j <= pLen; ++j) {
                if (p[j - 1] == '*') {
                    // Check zero occurrences branch
                    dp[i][j] = dp[i][j - 2];
                    
                    // Check one-or-more occurrences branch if s is not empty and preceding char matches
                    if (i > 0 && (s[i - 1] == p[j - 2] || p[j - 2] == '.')) {
                        dp[i][j] = dp[i][j] || dp[i - 1][j];
                    }
                } else {
                    // Standard direct evaluation frame path
                    if (i > 0 && (s[i - 1] == p[j - 1] || p[j - 1] == '.')) {
                        dp[i][j] = dp[i - 1][j - 1];
                    }
                }
            }
        }

        return dp[sLen][pLen];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M) - Where N represents the total character length of string s, and M 
 *                  represents the length of pattern p. Every matrix grid node is evaluated exactly once.
 *
 * SPACE COMPLEXITY: O(N * M) Auxiliary - The system allocates a 2D lookup array layout of size 
 *                   (N + 1) x (M + 1) to track structural match branches.
 */
int main() {
    string sourceText, patternExpr;
    cout << "=== Regular Expression Dynamic Programming Matcher ===\n";
    cout << "Enter the source input string (s): ";
    if (!(cin >> sourceText)) return 1;
    cout << "Enter the regular expression pattern (p): ";
    if (!(cin >> patternExpr)) return 1;

    Solution solver;
    cout << "\nInitializing evaluation grids and resolving branching expressions..." << endl;
    bool processingResult = solver.isMatch(sourceText, patternExpr);

    if (processingResult) {
        cout << "\nValidation Outcome: Successful structural match achieved! (true)\n";
    } else {
        cout << "\nValidation Outcome: String rejected under current pattern constraints. (false)\n";
    }

    return 0;
}