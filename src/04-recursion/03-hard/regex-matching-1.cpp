#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
private:
    /*
     * MEMOIZATION STRUCTURE FOR TWO-POINTER SELECTION STATES
     * ------------------------------------------------------
     * `memoTable[i][j]` caches results for the suffix sub-problem matching 
     * the substring `s[i...]` against the pattern template `p[j...]`.
     * 
     * Value Mappings:
     *   -1 : Unvisited/uncomputed state.
     *    0 : Evaluation resolved to False.
     *    1 : Evaluation resolved to True.
     */
    vector<vector<int>> memoTable;

    /*
     * RECURSIVE TOKENIZATION & WILDCARD BRANCHING SEARCH ENGINE
     * ---------------------------------------------------------
     * Evaluates character sequence structural matching by handling literal characters,
     * single-character wildcards ('.'), and zero-or-more quantifiers ('*').
     *
     * State Parameters:
     *  - s: The source text sequence string under evaluation.
     *  - p: The pattern template rule constraint string.
     *  - i: String index tracking text parsing progression.
     *  - j: Pattern index tracking expression tokenization layout.
     */
    bool matchState(const string& s, const string& p, size_t i, size_t j) {
        
        // BASE CASE: PATTERN EXHAUSTION CONVERGENCE
        // If the pattern index reaches the end of string 'p', matching succeeds 
        // if and only if the text parsing index 'i' has also consumed all characters in 's'.
        if (j == p.length()) {
            return i == s.length();
        }

        // MEMOIZATION CACHE CHECK
        // If this precise state lookup combination has already been solved, return it instantly.
        if (memoTable[i][j] != -1) {
            return memoTable[i][j] == 1;
        }

        // STEP 1: FIRST CHARACTER MATCH ASSERTER
        // Validate if the text character matches the pattern token at position 'j'.
        // This is true if text is not exhausted AND (characters match exactly OR pattern character is '.').
        bool firstCharMatch = (i < s.length() && (p[j] == s[i] || p[j] == '.'));

        bool result = false;

        // STEP 2: WILDCARD QUANTIFIER '*' LOOK-AHEAD BRANCHING
        // If the next consecutive pattern position contains a kleene star wildcard ('*'),
        // we hit a tokenization branch point. We must explore two distinct functional choices:
        if (j + 1 < p.length() && p[j + 1] == '*') {
            
            // CHOICE A: MATCH ZERO TIMES (Skip the current pattern token entirely)
            // We ignore the character followed by '*' and advance the pattern index by two positions.
            bool matchZeroTimes = matchState(s, p, i, j + 2);

            // CHOICE B: MATCH ONE OR MORE TIMES (Consume text character and stay in state)
            // If the first character matches, we can consume the current text character at index 'i' 
            // and keep evaluating the same pattern state 'j' to see if more duplicate matches follow.
            bool matchOneOrMoreTimes = firstCharMatch && matchState(s, p, i + 1, j);

            result = matchZeroTimes || matchOneOrMoreTimes;
            
        } else {
            // STEP 3: STANDARD STEP SEQUENCE LOOKUP
            // If no quantifier look-ahead is present, we proceed down the line. We can advance 
            // both pointers by 1 if and only if the first character matches successfully.
            result = firstCharMatch && matchState(s, p, i + 1, j + 1);
        }

        // CACHE COMMIT
        // Save the result into our 2D tracking matrix before returning up the stack frame layer.
        memoTable[i][j] = result ? 1 : 0;
        return result;
    }

public:
    bool isMatch(string s, string p) {
        // Initialize the 2D matrix tracker sizes to handle indexing limits smoothly
        // Sizing requires (N+1) x (M+1) allocations to accommodate terminal null state offsets.
        memoTable.assign(s.length() + 1, vector<int>(p.length() + 1, -1));
        
        return matchState(s, p, 0, 0);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M) - Where N represents the text string length and M matches the pattern length. 
 *                  Due to memoization optimization caching, each distinct state mapping pair coordinate (i, j) 
 *                  is executed exactly once, dropping matching overhead to constant time steps.
 *
 * SPACE COMPLEXITY: O(N * M) Auxiliary - Allocation footprint utilized by the 2D memoization tracking matrix,
 *                   combined with structural recursive call stack frames bounded by O(N + M).
 */
int main() {
    string s, p;
    cout << "Enter the source text string sequence (s): ";
    if (!(cin >> s)) return 1;

    cout << "Enter the regex pattern template rule string (p): ";
    if (!(cin >> p)) return 1;

    Solution solver;
    cout << "\nExecuting recursive tokenization pattern branching search..." << endl;
    
    if (solver.isMatch(s, p)) {
        cout << "Result: Match Successful! The string fits the pattern specifications." << endl;
    } else {
        cout << "Result: Match Failed! The string breaks the pattern rules." << endl;
    }

    return 0;
}