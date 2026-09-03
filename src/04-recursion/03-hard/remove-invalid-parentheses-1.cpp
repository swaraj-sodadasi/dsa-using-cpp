#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class Solution {
private:
    vector<string> validExpressions;
    unordered_set<string> visitedStates;

    /*
     * BALANCED VALIDATION CHECK UTILITY
     * ---------------------------------
     * Verifies if a given configuration string forms a structurally valid parentheses layout.
     * It uses a running balance counter: incremented for '(' and decremented for ')'.
     * If the counter drops below zero at any point, it means a closing brace appears
     * without a matching open brace, making it invalid immediately.
     */
    bool isValid(const string& s) {
        int balanceCount = 0;
        for (char c : s) {
            if (c == '(') {
                balanceCount++;
            } else if (c == ')') {
                balanceCount--;
                if (balanceCount < 0) {
                    return false; // Pruning condition: unbalanced closing bracket
                }
            }
        }
        return balanceCount == 0;
    }

    /*
     * MINIMAL-CUT MINIMIZATION DISCOVERY LAYER (PRECOMPUTATION)
     * ---------------------------------------------------------
     * Before generating combinations, we calculate the exact number of invalid open
     * and close parentheses that must be removed. 
     * This establishes an absolute minimal-cut bound, turning the search space
     * into a highly directed pruning tree.
     */
    void calculateMinimumRemovals(const string& s, int& remOpen, int& remClose) {
        remOpen = 0;
        remClose = 0;

        for (char c : s) {
            if (c == '(') {
                remOpen++;
            } else if (c == ')') {
                if (remOpen > 0) {
                    remOpen--; // Found a valid pair, decrement open counter
                } else {
                    remClose++; // No open match available, this close bracket must be cut
                }
            }
        }
    }

    /*
     * RECURSIVE BACKTRACKING PRUNING SEARCH ENGINE
     * ---------------------------------------------
     * Traverses the string expression systematically from left to right to isolate valid permutations.
     * By enforcing strict structural conditions, the algorithm guarantees minimal edits without a separate BFS grid layer.
     *
     * State Parameters:
     *  - s: The baseline source reference expression string.
     *  - startIndex: Current layout index identifying the character under evaluation.
     *  - remOpen: Remaining allocation allowance of invalid '(' items left to remove.
     *  - remClose: Remaining allocation allowance of invalid ')' items left to remove.
     */
    void backtrack(const string& s, size_t startIndex, int remOpen, int remClose) {
        
        // BASE CASE: CONVERGENCE CONSTRAINTS REACHED
        // When both removal counters hit zero, we have discarded the exact amount of characters
        // required to achieve the minimal cut. If the remaining string is structurally balanced,
        // it is a valid solution.
        if (remOpen == 0 && remClose == 0) {
            if (isValid(s)) {
                validExpressions.push_back(s);
            }
            return;
        }

        // Loop through all character choice positions starting from 'startIndex'
        for (size_t i = startIndex; i < s.length(); ++i) {
            
            // OPTIMIZATION: DUPLICATE CHOICE SKIPPER
            // If the current character is identical to the previous one, skipping it avoids
            // creating duplicate substrings and prevents redundant search paths.
            if (i > startIndex && s[i] == s[i - 1]) {
                continue;
            }

            // Check if there are still invalid characters remaining that can be pruned
            if (remOpen + remClose > static_cast<int>(s.length() - i)) {
                return; // Early Pruning: Not enough characters left to satisfy required removals
            }

            // PROCESS REMOVAL BRANCH 1: ELIMINATING AN INVALID CLOSE BRACKET
            if (remClose > 0 && s[i] == ')') {
                string nextState = s.substr(0, i) + s.substr(i + 1);
                backtrack(nextState, i, remOpen, remClose - 1);
            }

            // PROCESS REMOVAL BRANCH 2: ELIMINATING AN INVALID OPEN BRACKET
            if (remOpen > 0 && s[i] == '(') {
                string nextState = s.substr(0, i) + s.substr(i + 1);
                backtrack(nextState, i, remOpen - 1, remClose);
            }
        }
    }

public:
    vector<string> removeInvalidParentheses(string s) {
        validExpressions.clear();
        visitedStates.clear();

        int remOpen = 0;
        int remClose = 0;
        
        // Pre-calculate the targeted target threshold limits
        calculateMinimumRemovals(s, remOpen, remClose);
        
        // Trigger the pruned tracking framework
        backtrack(s, 0, remOpen, remClose);
        
        return validExpressions;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(2^N) Worst-Case - Where N matches string length. In the absolute worst scenario, 
 *                  each bracket can toggle between being kept or removed. However, minimal-cut preprocessing
 *                  and duplicate skipping limit execution to a tiny fraction of the upper bound.
 *
 * SPACE COMPLEXITY: O(N^2) Auxiliary - Allocation footprint utilized by temporary substring slicing 
 *                   operations, combined with recursive call stack depth frames bounded by O(N).
 */
int main() {
    string s;
    cout << "Enter the parental layout string expression containing random characters:\n";
    cout << "Example: ()())() or (a)())()\n";
    cout << "Input: ";
    if (!(cin >> s)) {
        cout << "Invalid string configurations entry." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting recursive backtracking minimal-cut search..." << endl;
    vector<string> result = solver.removeInvalidParentheses(s);

    cout << "\nAll unique valid configurations containing minimal removals:\n[\n";
    for (const string& expression : result) {
        cout << "  \"" << expression << "\"\n";
    }
    cout << "]\n";
    cout << "Total distinct optimized layout variations found: " << result.size() << endl;

    return 0;
}