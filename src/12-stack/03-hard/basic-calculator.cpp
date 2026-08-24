#include <iostream>
#include <string>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: NESTED BRACKET STRINGS EXECUTION WITH OPERATOR/SIGN STACKS
     * -------------------------------------------------------------------------------------
     * The objective is to evaluate a basic mathematical expression containing '+', '-', 
     * '(', ')', and spaces.
     * 
     * 1. THE CONTEXT-SWITCH INTERLEAVED STACKS:
     *    Parentheses represent isolated math universes. When an opening bracket '(' is encountered, 
     *    we must save our current progress. We maintain two tracking states inside a single pair of stacks:
     *      - resultStack: Stores the accumulated evaluation value prior to opening the brackets.
     *      - signStack: Stores the tracking modifier flag (+1 or -1) hanging outside the opening bracket.
     * 
     * 2. PARSING STATE MACHINE METRICS:
     *    We iterate left-to-right through the mathematical layout string:
     *      - Digit : Accumulate consecutive digits to build numbers exceeding base-10 single units.
     *      - `+` / `-` : Commit the accumulated number token into the running localized result, 
     *                   then update the sign modifier value for the next upcoming operand.
     *      - `(`   : Context Push. Stash the current state variables (`runningResult`, `activeSign`) 
     *                onto the stack arrays, then clear both registers to evaluate the sub-expression.
     *      - `)`   : Context Pop. Commit the final inner-bracket calculation, multiply it by the popped
     *                sign modifier from the stack, and add the result to the popped baseline value.
     */
    int calculate(string s) {
        stack<int> resultStack;
        stack<int> signStack;
        
        int runningResult = 0;
        int activeSign = 1; // 1 represents '+', -1 represents '-'
        int n = static_cast<int>(s.length());

        for (int i = 0; i < n; ++i) {
            char ch = s[static_cast<size_t>(i)];

            if (isdigit(ch)) {
                long long currentNumber = 0;
                // Parse numbers containing multiple digits consecutively
                while (i < n && isdigit(s[static_cast<size_t>(i)])) {
                    currentNumber = currentNumber * 10 + (s[static_cast<size_t>(i)] - '0');
                    i++;
                }
                i--; // Step back one position to counteract outer loop iteration advancement
                
                runningResult += activeSign * static_cast<int>(currentNumber);
            } else if (ch == '+') {
                activeSign = 1;
            } else if (ch == '-') {
                activeSign = -1;
            } else if (ch == '(') {
                // Stash the calculation lane details before entering deep recursive block scopes
                resultStack.push(runningResult);
                signStack.push(activeSign);
                
                // Reset standard tracking channels for the inner scope evaluation segment
                runningResult = 0;
                activeSign = 1;
            } else if (ch == ')') {
                // Liquidate active parentheses block frame scopes
                int historicalSign = signStack.top();
                signStack.pop();
                
                int historicalResult = resultStack.top();
                resultStack.pop();

                // Merge structural inner calculations with parent pipeline contexts
                runningResult = historicalResult + (historicalSign * runningResult);
            }
        }

        return runningResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the mathematical expression string.
 *                  Every text character is evaluated at most twice due to the inner numeric loop sweep.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - Stack depth scales proportionally with 
 *                   the level depth of consecutive nested bracket structures (e.g., "(((1+2)+3)+4)").
 */
int main() {
    cout << "=== Nested Bracket Strings Basic Calculator Engine ===\n";
    cout << "Enter the mathematical expression string: ";
    
    string expressionStr;
    if (!getline(cin, expressionStr) || expressionStr.empty()) return 0;

    Solution solver;
    cout << "Tokenizing symbols and resolving arithmetic priority trees..." << endl;
    int calculatedOutcome = solver.calculate(expressionStr);

    cout << "Final Evaluated Expression Output: [" << calculatedOutcome << "]\n";

    return 0;
}