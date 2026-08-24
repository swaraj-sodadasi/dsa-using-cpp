#include <iostream>
#include <string>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DUAL INDEX STACKS TRACKING DYNAMIC WILDCARD STAR ALLOCATIONS
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a string containing '(', ')' and '*' is valid.
     * A '*' can act as a left parenthesis, a right parenthesis, or an empty string.
     * 
     * 1. THE DUAL-STACK ARCHITECTURE:
     *    We maintain two separate stacks that store *indices* rather than character literals:
     *      - openStack: Tracks indices of unmatched '(' brackets.
     *      - starStack: Tracks indices of dynamic wildcard '*' characters.
     *    Storing indices is critical because spatial orientation (which character comes first)
     *    determines whether a wildcard can legally resolve a structural mismatch.
     * 
     * 2. THE GREEDY LINEAR TRAVERSAL LAWS:
     *    We sweep the string from left to right:
     *      - `(` : Push the index onto openStack.
     *      - `*` : Push the index onto starStack.
     *      - `)` : Liquidation phase. First, try to match it with a standard open bracket by 
     *              popping from openStack. If openStack is empty, fallback to consuming a wildcard 
     *              by popping from starStack. If both stacks are empty, the string is instantly invalid.
     * 
     * 3. RESIDUAL BOUNDARY CLEARING INVARIANT:
     *    After parsing, unmatched open brackets may remain. We try to neutralize them using the 
     *    remaining wildcards. An open bracket `(` at index `i` can only be closed by a `*` at index `j` 
     *    if the wildcard appears *after* the opening bracket (`i < j`). We pop from both stacks 
     *    and evaluate this index distance property. If any `(` cannot be matched, the validation fails.
     */
    bool checkValidString(string s) {
        stack<int> openStack;
        stack<int> starStack;
        int n = static_cast<int>(s.length());

        for (int i = 0; i < n; ++i) {
            char ch = s[static_cast<size_t>(i)];
            if (ch == '(') {
                openStack.push(i);
            } else if (ch == '*') {
                starStack.push(i);
            } else if (ch == ')') {
                if (!openStack.empty()) {
                    openStack.pop();
                } else if (!starStack.empty()) {
                    starStack.pop();
                } else {
                    return false; // Closing bracket lacks an opening partner or wildcard buffer
                }
            }
        }

        // Match residual open parentheses with trailing wildcards
        while (!openStack.empty() && !starStack.empty()) {
            // An open parenthesis can only be canceled by a star that appears downstream
            if (openStack.top() > starStack.top()) {
                return false; 
            }
            openStack.pop();
            starStack.pop();
        }

        return openStack.empty();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of string s.
 *                  Every character index is pushed and popped from the stacks at most once.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The stacks hold up to N elements 
 *                   under high concentrations of open brackets or wildcards (e.g., "(((***").
 */
int main() {
    cout << "=== Wildcard Index Dual Stack Parenthesis Validation Engine ===\n";
    cout << "Enter the target string to evaluate (composed of '(', ')' and '*'): ";
    
    string inputStr;
    if (!(cin >> inputStr)) return 1;

    Solution solver;
    cout << "Analyzing index alignment distributions and evaluating wildcard paths..." << endl;
    bool isValidPattern = solver.checkValidString(inputStr);

    cout << "Validation Verdict Result: [" << (isValidPattern ? "VALID" : "INVALID") << "]\n";

    return 0;
}