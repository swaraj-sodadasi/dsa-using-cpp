#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
private:
    vector<string> allExpressions;

    /*
     * RECURSIVE BACKTRACKING PRECEDENCE PARSING ENGINE
     * ------------------------------------------------
     * The objective is to insert operators ('+', '-', '*') into a string of digits 
     * so that the expression evaluates to a target value.
     * Because multiplication '*' has higher precedence than addition and subtraction, 
     * a simple left-to-right evaluation breaks down. To handle operator precedence in 
     * a single linear pass without an expensive expression parser, we track the 
     * mathematical impact of the *most recent term* dynamically.
     *
     * State Tracking Variables:
     *  - numStr: The source string containing numeric digits.
     *  - target: The final calculation objective value we need to match.
     *  - index: The current pointer position in the digit string.
     *  - currentExpression: The structural string buffer tracking the active path.
     *  - currentEvaluatedValue: The running arithmetic total evaluated so far.
     *  - previousTermValue: The standalone value of the last operational term processed.
     *
     * Precedence Scaling Mechanic:
     *  - If we choose '+', the new value is `currentEvaluatedValue + currentOperand`. 
     *    The new `previousTermValue` becomes `currentOperand`.
     *  - If we choose '-', the new value is `currentEvaluatedValue - currentOperand`. 
     *    The new `previousTermValue` becomes `-currentOperand`.
     *  - If we choose '*', multiplication binds tightly to the previous term. We must 
     *    mathematically "undo" the last term's individual contribution and merge it with 
     *    the new operand: `currentEvaluatedValue - previousTermValue + (previousTermValue * currentOperand)`.
     *    The new `previousTermValue` becomes `previousTermValue * currentOperand`.
     */
    void generateExpressions(const string& numStr, long long target, size_t index, 
                             string currentExpression, long long currentEvaluatedValue, long long previousTermValue) {
        
        // BASE CASE: CONVERGENCE VALIDATION
        // If we reach the end of the input string, we check if the running evaluation 
        // matches the target value. If it does, we add this string variant to the results list.
        if (index == numStr.length()) {
            if (currentEvaluatedValue == target) {
                allExpressions.push_back(currentExpression);
            }
            return;
        }

        // TRANSITION LOOP: MULTI-DIGIT OPERAND BRANCHING
        // We evaluate every possible terminal length extending from 'index' to the end 
        // of the string to capture single or multi-digit numbers (e.g., matching '1', '12', '123').
        for (size_t i = index; i < numStr.length(); ++i) {
            
            // CRITICAL PRUNING: LEADING ZERO ELIMINATION
            // Numbers with multiple digits cannot begin with a leading '0' (e.g., "05" is invalid).
            // If the chunk starts with '0' and we try to expand past it, we prune immediately.
            if (i > index && numStr[index] == '0') {
                break;
            }

            string operandSegment = numStr.substr(index, i - index + 1);
            long long currentOperand = stoll(operandSegment);

            // ANCHOR STEP: STARTING VALUE PROCESSING
            // If we are at index 0, this is the very first number in the expression. 
            // It has no preceding operator, so we simply seed the tracking state and recurse.
            if (index == 0) {
                generateExpressions(numStr, target, i + 1, operandSegment, currentOperand, currentOperand);
            } else {
                // BRANCH DECISION 1: ADDITION OPERATOR ('+')
                generateExpressions(numStr, target, i + 1, 
                                    currentExpression + "+" + operandSegment, 
                                    currentEvaluatedValue + currentOperand, 
                                    currentOperand);

                // BRANCH DECISION 2: SUBTRACTION OPERATOR ('-')
                generateExpressions(numStr, target, i + 1, 
                                    currentExpression + "-" + operandSegment, 
                                    currentEvaluatedValue - currentOperand, 
                                    -currentOperand);

                // BRANCH DECISION 3: MULTIPLICATION OPERATOR ('*') WITH PRECEDENCE RESOLUTION
                // We factor in the tight binding property by rolling back the independent contribution 
                // of previousTermValue and grouping it directly with the new multiplier operand.
                generateExpressions(numStr, target, i + 1, 
                                    currentExpression + "*" + operandSegment, 
                                    currentEvaluatedValue - previousTermValue + (previousTermValue * currentOperand), 
                                    previousTermValue * currentOperand);
            }
        }
    }

public:
    vector<string> addOperators(string num, int target) {
        allExpressions.clear();
        if (num.empty()) {
            return allExpressions;
        }
        
        // Initialize the recursive backtracking framework
        generateExpressions(num, target, 0, "", 0, 0);
        return allExpressions;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(4^N) Worst-Case - At each boundary split between the N digits, 
 *                  we choose between 4 parallel structural directions: Join Digits, Add, 
 *                  Subtract, or Multiply. String allocations inside the branches push 
 *                  the aggregate operation bound scale to O(N * 4^N).
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal framework footprint scale is bound 
 *                   linearly by the maximum tracking depth inside the execution call stack, 
 *                   which reaches a peak value of exactly N operational execution layers.
 */
int main() {
    string num;
    int target;

    cout << "Enter the source input string of numeric digits: ";
    if (!(cin >> num)) {
        cout << "Invalid digit string parameters input entry." << endl;
        return 1;
    }

    // Input verification check to filter out non-numeric entries
    for (char c : num) {
        if (c < '0' || c > '9') {
            cout << "Constraint Error: Input string must consist entirely of numeric digits." << endl;
            return 1;
        }
    }

    cout << "Enter the targeted target math calculation value: ";
    if (!(cin >> target)) {
        cout << "Invalid integer entry format constraints." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting recursive backtracking parsing sequences..." << endl;
    vector<string> result = solver.addOperators(num, target);

    cout << "\nAll combinations resolving perfectly to target " << target << ":\n[\n";
    for (const string& expression : result) {
        cout << "  \"" << expression << "\"\n";
    }
    cout << "]\n";
    cout << "Total distinct functional layout configurations located: " << result.size() << endl;

    return 0;
}