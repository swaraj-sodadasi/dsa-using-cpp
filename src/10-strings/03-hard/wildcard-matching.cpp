#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: WILDCARD MATCHING (TWO-POINTER LINEAR OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * The objective is to evaluate if string `s` matches pattern `p` where:
     *   - '?' matches any single character.
     *   - '*' matches any sequence of characters (including the empty sequence).
     * 
     * 1. THE ARCHITECTURAL TRADEOFF (2D DP GRID VS. TWO-POINTER LINEAR OPTIMIZATION):
     *    - 2D DP Grid Evaluation: An $O(N \times M)$ time and space approach where `dp[i][j]` 
     *      tracks if `s[0...i-1]` matches `p[0...j-1]`. This requires a heavy memory grid matrix.
     *    - Two-Pointer Linear Optimization (Chosen): When a wildcard '*' is encountered, it can 
     *      match anywhere from 0 to N remaining characters. Instead of calculating all variations, 
     *      we act greedily: we assume it matches 0 characters first, record the match checkpoint 
     *      state variables (`sStarCheck` and `pStarIdx`), and continue testing forward. If a mismatch 
     *      occurs later, we back up, consume one more character from `s` into the '*' match, and 
     *      resume. This keeps the space complexity at a perfect $O(1)$.
     * 
     * 2. MATCH AND BACKTRACKING CONTROL ENGINE:
     *    We maintain four primary cursor indicators:
     *      - `sIdx`, `pIdx`: Active linear scanning cursors for string `s` and pattern `p`.
     *      - `sStarCheck`: Keeps track of the last index in `s` matched by a '*' character.
     *      - `pStarIdx`: Points to the index of the most recent '*' encountered in pattern `p`.
     * 
     * 3. THE GREEDY ITERATION RULES:
     *      - Rule A (Direct Match): If characters match or `p[pIdx] == '?'`, step both cursors forward.
     *      - Rule B (Wildcard Discovery): If `p[pIdx] == '*'`, record the checkpoint status 
     *        (`pStarIdx = pIdx`, `sStarCheck = sIdx`) and advance ONLY `pIdx`.
     *      - Rule C (Backtrack Phase): If a mismatch happens but a '*' was seen previously, we branch back. 
     *        We increment `sStarCheck`, reset `sIdx = sStarCheck`, and point `pIdx = pStarIdx + 1`.
     *      - Rule D (Absolute Mismatch): If none of these match, the strings cannot be paired.
     */
    bool isMatch(string s, string p) {
        size_t sLen = s.length();
        size_t pLen = p.length();
        
        size_t sIdx = 0, pIdx = 0;
        int sStarCheck = -1, pStarIdx = -1;

        // Step 2 & 3: Main two-pointer greedy optimization processing loop
        while (sIdx < sLen) {
            // Rule A: Characters match perfectly or pattern specifies a wildcard single '?' match
            if (pIdx < pLen && (p[pIdx] == '?' || p[pIdx] == s[sIdx])) {
                sIdx++;
                pIdx++;
            } 
            // Rule B: Multi-character wildcard '*' encountered. Record the backtrack checkpoint state.
            else if (pIdx < pLen && p[pIdx] == '*') {
                pStarIdx = static_cast<int>(pIdx);
                sStarCheck = static_cast<int>(sIdx);
                pIdx++; // Greedily assume the '*' matches 0 characters first
            } 
            // Rule C: Mismatch occurred, but we have a fallback historical checkpoint. Backtrack.
            else if (pStarIdx != -1) {
                sStarCheck++; // Expand the number of characters consumed by the '*' wildcard
                sIdx = static_cast<size_t>(sStarCheck);
                pIdx = static_cast<size_t>(pStarIdx + 1);
            } 
            // Rule D: Absolute mismatch with no valid historical wildcards to fall back on
            else {
                return false;
            }
        }

        // Clean up remaining pattern characters (trailing '*' elements can match empty sequences)
        while (pIdx < pLen && p[pIdx] == '*') {
            pIdx++;
        }

        return pIdx == pLen;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M) Worst-Case / O(N + M) Average-Case - Where N represents the length of s, 
 *                  and M represents the length of p. Highly repetitive patterns like s="aaaaa" and 
 *                  p="*aaab" trigger continuous backtracking up to O(N * M). However, standard real-world 
 *                  wildcard pattern allocations execute in linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The matching checks are processed entirely in place using inline register 
 *                   pointers, completely avoiding state grid configurations.
 */
int main() {
    string sourceText, patternExpr;
    cout << "=== Wildcard Matching Two-Pointer Linear Optimization Console ===\n";
    cout << "Enter the source string to evaluate (s): ";
    if (!(cin >> sourceText)) return 1;
    cout << "Enter the wildcard match pattern expressions (p): ";
    if (!(cin >> patternExpr)) return 1;

    Solution solver;
    cout << "\nInitializing streaming cursors and calculating state transformations..." << endl;
    bool matchOutcomeResult = solver.isMatch(sourceText, patternExpr);

    if (matchOutcomeResult) {
        cout << "\nValidation Result: Successful match achieved! (true)\n";
    } else {
        cout << "\nValidation Result: Substring match structurally rejected. (false)\n";
    }

    return 0;
}