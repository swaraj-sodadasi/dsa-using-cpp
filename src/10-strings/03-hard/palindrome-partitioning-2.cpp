#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: PALINDROME PARTITIONING II (MIN-CUT DYNAMIC PROGRAMMING OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * The goal is to determine the minimum number of cuts needed to partition a string `s`
     * such that every subsegment is a palindrome.
     * 
     * 1. THE NAIVE DP OVERHEAD VS DUAL PASS APPROACH:
     *    A simple recursion with memoization checks substrings repeatedly, resulting in an 
     *    O(N^3) time footprint due to continuous palindrome validation. 
     *    Instead, we break this down into two decoupled O(N^2) layers:
     *      - Pass A: Precompute a 2D boolean array `isPal[i][j]` tracking if `s[i...j]` is a palindrome.
     *      - Pass B: Build a 1D DP lookup array `minCuts[i]` storing the min cuts for `s[0...i]`.
     * 
     * 2. PALINDROME CHECKS VIA MANBER-MYERS CONCEPT:
     *    `isPal[i][j]` is true if `s[i] == s[j]` AND either the substring length is less than or equal to 2 
     *    (i.e., `j - i <= 2`) or the inner core is already palindromic (`isPal[i+1][j-1]`).
     * 
     * 3. MIN-CUT LINEAR STEP REDUCTION:
     *    For each index `i` from 0 to N-1:
     *      - If `isPal[0][i]` is true, `minCuts[i] = 0` (no cuts needed for the entire prefix).
     *      - Otherwise, we test all possible internal cut split points `j` from 0 to `i-1`. If `s[j+1...i]` 
     *        forms a palindrome, we can place a cut right after index `j`. The cost becomes `minCuts[j] + 1`.
     *        We minimize this across all valid values of `j`.
     */
    int minCut(string s) {
        int n = static_cast<int>(s.length());
        if (n <= 1) return 0;

        // Step 1: Precompute palindrome states across all possible pairs of indices
        vector<vector<bool>> isPal(static_cast<size_t>(n), vector<bool>(static_cast<size_t>(n), false));

        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                if (s[static_cast<size_t>(i)] == s[static_cast<size_t>(j)] && (j - i <= 2 || isPal[static_cast<size_t>(i + 1)][static_cast<size_t>(j - 1)])) {
                    isPal[static_cast<size_t>(i)][static_cast<size_t>(j)] = true;
                }
            }
        }

        // Step 2: Linear optimization pass using the computed palindrome matrix
        vector<int> minCuts(static_cast<size_t>(n), 0);

        for (int i = 0; i < n; ++i) {
            if (isPal[0][static_cast<size_t>(i)]) {
                minCuts[static_cast<size_t>(i)] = 0;
            } else {
                int localizedMin = i; // Max possible cuts for a prefix of length i+1 is i cuts
                for (int j = 0; j < i; ++j) {
                    if (isPal[static_cast<size_t>(j + 1)][static_cast<size_t>(i)]) {
                        localizedMin = min(localizedMin, minCuts[static_cast<size_t>(j)] + 1);
                    }
                }
                minCuts[static_cast<size_t>(i)] = localizedMin;
            }
        }

        return minCuts[static_cast<size_t>(n - 1)];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2) - Where N represents the total character length of string s. 
 *                  Both the 2D palindrome matrix configuration loop and the 1D structural 
 *                  cut optimization loop use two nested layers, yielding a strict quadratic footprint.
 *
 * SPACE COMPLEXITY: O(N^2) Auxiliary - The system allocates a 2D lookup table of size N x N 
 *                   to cache substring palindromic validations.
 */
int main() {
    string sourceInput;
    cout << "=== Palindrome Partitioning II Min-Cut Engine ===\n";
    cout << "Enter the string to evaluate: ";
    if (!(cin >> sourceInput)) return 1;

    Solution solver;
    cout << "\nCalculating substring combinations and minimizing partitioning boundaries..." << endl;
    int minimumCutsComputed = solver.minCut(sourceInput);

    cout << "\nMinimum cut operations required: " << minimumCutsComputed << " splits.\n";

    return 0;
}