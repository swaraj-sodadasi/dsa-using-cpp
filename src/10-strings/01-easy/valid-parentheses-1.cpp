#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BASIC BRACKET PAIR STATE VALIDATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine if an input string containing only the characters 
     * '(', ')', '{', '}', '[' and ']' is structurally valid.
     * 
     * 1. THE LAST-IN, FIRST-OUT (LIFO) PROPERTY:
     *    Parentheses nesting follows a strict last-in, first-out sequence. The bracket that 
     *    opens last must be closed first. A standard stack structure (`std::stack<char>`) is 
     *    the perfect tool to enforce this layout tracking.
     * 
     * 2. PUSH vs. POP DECISION ARCHITECTURE:
     *    We iterate through the string character by character:
     *      - If we encounter an opening bracket (`(`, `{`, `[`), we push it onto the stack.
     *      - If we encounter a closing bracket (`)`, `}`, `]`), we must check the stack:
     *          * If the stack is empty, it means we have a closing bracket without a matching 
     *            opening bracket, making the string invalid.
     *          * If the stack is not empty, we pop the top character and verify if it matches 
     *            the corresponding opening pair. If it is a mismatch (e.g., `[` closed by `)`), 
     *            the validation fails immediately.
     * 
     * 3. RESIDUAL LIFECYCLE CHECK:
     *    Once the loop finishes parsing the entire string, we perform a final check. If the 
     *    stack is completely empty, all opened brackets were successfully and validly closed. 
     *    If any characters remain in the stack, it means some opening brackets lacked closing 
     *    pairs, making the string invalid.
     */
    bool isValid(string s) {
        stack<char> bracketStack;

        for (char c : s) {
            // Step 2: Push opening brackets onto the stack registry
            if (c == '(' || c == '{' || c == '[') {
                bracketStack.push(c);
            } 
            // Handle closing bracket matching conditions
            else {
                if (bracketStack.empty()) {
                    return false; // Closing bracket with no opening counterpart
                }

                char topElement = bracketStack.top();
                bracketStack.pop();

                // Validate matching pair alignments
                if ((c == ')' && topElement != '(') ||
                    (c == '}' && topElement != '{') ||
                    (c == ']' && topElement != '[')) {
                    return false; // Mismatched bracket types encountered
                }
            }
        }

        // Step 3: String is valid only if no unmatched opening brackets remain
        return bracketStack.empty();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the length of the string s. The single-pass 
 *                  loop parses each character exactly once. Each stack push and pop operation 
 *                  takes true O(1) constant runtime.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - In the worst-case scenario (e.g., a string consisting entirely 
 *                   of opening brackets like "(((((((("), the stack grows proportionally to hold 
 *                   all N characters.
 */
int main() {
    string inputExpression;
    cout << "=== Valid Parentheses Stack Validator Console ===\n";
    cout << "Enter a bracket expression sequence string (using only (), {}, []):\n";
    if (!(cin >> inputExpression)) return 1;

    // Validate string layout constraints early
    for (char c : inputExpression) {
        if (c != '(' && c != ')' && c != '{' && c != '}' && c != '[' && c != ']') {
            cout << "Constraint Error: Input must contain valid bracket characters exclusively." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nLaunching linear LIFO stack balancing sweeps..." << endl;
    bool isBalanced = solver.isValid(inputExpression);

    if (isBalanced) {
        cout << "Result: The configured bracket expression sequence is VALID! (true)\n";
    } else {
        cout << "Result: The configured bracket expression sequence is INVALID. (false)\n";
    }

    return 0;
}