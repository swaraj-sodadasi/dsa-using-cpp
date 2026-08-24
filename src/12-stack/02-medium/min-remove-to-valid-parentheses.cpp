#include <iostream>
#include <string>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BALANCED INDEX FILTERING STACK TRACK
     * -------------------------------------------------------------------------------------
     * The objective is to remove the minimum number of parentheses (either '(' or ')'),
     * in any positions, so that the resulting parentheses string is valid.
     * 
     * 1. THE INDEX-TRACKING STACK MECHANISM:
     *    Instead of storing character literals, our stack tracks the *indices* of open 
     *    parentheses. This design gives us the exact positions of structural violations 
     *    that need to be removed at the end of the scan.
     * 
     * 2. SINGLE-PASS TRAVERSAL FILTER LAWS:
     *    We iterate through the string from left to right:
     *      - Open Parenthesis '('  : Push its string array index position onto the stack.
     *      - Close Parenthesis ')' : Check the stack state. If the stack contains active 
     *                                indices, the closing bracket matches a valid open 
     *                                bracket, so we pop the top index. If the stack is 
     *                                empty, this close parenthesis has no open match; 
     *                                we overwrite it with a placeholder flag (e.g., '*').
     * 
     * 3. RESIDUAL LIQUIDATION AND IN-PLACE BUILD:
     *    Once the linear pass finishes, any indices left in the stack represent orphaned open 
     *    parentheses that were never closed. We loop through the stack and overwrite those 
     *    positions with our placeholder flag '*' as well.
     *    Finally, we build the clean output string by skipping all placeholder flags.
     */
    string minRemoveToMakeValid(string s) {
        stack<int> openIndicesStack;
        size_t n = s.length();

        // Step 1: Identify structural mismatches in a forward tracking sweep
        for (size_t i = 0; i < n; ++i) {
            if (s[i] == '(') {
                openIndicesStack.push(static_cast<int>(i));
            } else if (s[i] == ')') {
                if (!openIndicesStack.empty()) {
                    openIndicesStack.pop();
                } else {
                    // Orphaned closing bracket; flag it for deletion filtering
                    s[i] = '*';
                }
            }
        }

        // Step 2: Flag unmatched opening brackets left on the stack frame
        while (!openIndicesStack.empty()) {
            s[static_cast<size_t>(openIndicesStack.top())] = '*';
            openIndicesStack.pop();
        }

        // Step 3: Filter out placeholder tokens to construct the final valid string matrix
        string result = "";
        for (char ch : s) {
            if (ch != '*') {
                result += ch;
            }
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string.
 *                  The filtering loops sweep across the string indices in linear passes.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The system stack stores at most N index 
 *                   coordinates under string layouts composed entirely of opening items (e.g., "((((").
 */
int main() {
    cout << "=== Balanced Index Filtering Stack Parentheses Engine ===\n";
    cout << "Enter the target parentheses string to balance: ";
    
    string inputString;
    if (!(cin >> inputString)) return 1;

    cout << "\nOriginal String Configuration: [" << inputString << "]\n";
    
    Solution solver;
    cout << "Analyzing balance layouts and filtering orphaned index boundaries..." << endl;
    string validOutcome = solver.minRemoveToMakeValid(inputString);

    cout << "Processed Balanced Result:    [" << validOutcome << "]\n";

    return 0;
}