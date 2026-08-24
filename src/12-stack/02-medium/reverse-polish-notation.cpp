#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: POSTFIX ARITHMETIC OPERATOR STACK EVALUATION
     * -------------------------------------------------------------------------------------
     * The objective is to evaluate the value of an arithmetic expression in Reverse Polish Notation.
     * 
     * 1. THE POSTFIX EVALUATION PIPELINE:
     *    In postfix notation, operators follow their operands. We parse tokens sequentially from left to right:
     *      - Operand (Number) : Push its integer representation onto a validation stack frame.
     *      - Operator (+, -, *, /) : Pop the top two values from the stack, apply the operation,
     *        and push the resulting calculation back onto the stack.
     * 
     * 2. ORDER CONTINGENCY PRINCIPLE:
     *    Because stack operations follow LIFO ordering rules, the first popped element represents the *second* operand
     *    (`operand2`), and the second popped element represents the *first* operand (`operand1`). 
     *    This ordering is crucial for non-commutative operations like subtraction and division.
     */
    int evalRPN(const vector<string>& tokens) {
        stack<int> calculationStack;

        for (const string& token : tokens) {
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                int operand2 = calculationStack.top();
                calculationStack.pop();
                int operand1 = calculationStack.top();
                calculationStack.pop();

                if (token == "+") {
                    calculationStack.push(operand1 + operand2);
                } else if (token == "-") {
                    calculationStack.push(operand1 - operand2);
                } else if (token == "*") {
                    calculationStack.push(operand1 * operand2);
                } else if (token == "/") {
                    calculationStack.push(operand1 / operand2);
                }
            } else {
                // Parse the numerical string token directly into a signed 32-bit integer value
                calculationStack.push(stoi(token));
            }
        }

        return calculationStack.top();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total token count inside the input vector string layout.
 *                  Every individual string token is evaluated exactly once in a linear pass.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The stack depth scales linearly, holding at most
 *                   (N / 2) + 1 integer variables under highly concentrated prefix configurations.
 */
int main() {
    cout << "=== Postfix Arithmetic Reverse Polish Notation Engine ===\n";
    cout << "Enter the total token length: ";
    int totalTokens;
    if (!(cin >> totalTokens) || totalTokens <= 0) return 1;

    vector<string> tokenSequence(static_cast<size_t>(totalTokens));
    cout << "Enter expression tokens sequentially (e.g., 2 1 + 3 *):\n";
    for (int i = 0; i < totalTokens; ++i) {
        cin >> tokenSequence[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nParsing dynamic token array configurations and resolving operators..." << endl;
    int computationOutcome = solver.evalRPN(tokenSequence);

    cout << "Final Resolved Expression Value: [" << computationOutcome << "]\n";

    return 0;
}