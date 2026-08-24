#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: LIFO COMPLEMENTARY BRACKET PATTERN TRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to determine if an input string containing bracket characters 
     * '(', ')', '{', '}', '[' and ']' is structurally valid.
     * 
     * 1. THE LAST-IN-FIRST-OUT (LIFO) HYPOTHESIS:
     *    Brackets must close in the exact reverse order of their opening. The most recently 
     *    opened bracket must be the very first one to be closed. A stack data structure is 
     *    perfectly suited for this LIFO tracking behavior.
     * 
     * 2. EFFICIENT LOOK-AHEAD DRY RUN STRATEGY:
     *    Instead of pushing opening characters and running complex switch maps during pop evaluations, 
     *    we can swap the logic: the moment we encounter an opening bracket, we push its corresponding 
     *    *closing* bracket onto the stack.
     *      - Meet '(', push ')'
     *      - Meet '{', push '}'
     *      - Meet '[', push ']'
     * 
     * 3. THE VALIDATION MATRIX CONSTRAINTS:
     *    When we process a closing bracket character from the string:
     *      - If the stack is empty, it means we have an orphaned closing bracket with no opening pair 
     *        (e.g., string starts with ']'). Immediate structural invalidity.
     *      - If the top element of the stack does not match the current character, the closing order 
     *        is broken (e.g., "{]"). Immediate structural invalidity.
     *    Otherwise, a correct match is confirmed; we pop the matching character from the stack and continue.
     *    Finally, after scanning the entire string, the stack must be completely empty. If any elements 
     *    remain, it means an opening bracket was never closed (e.g., "(()").
     */
    bool isValid(string s) {
        // Base Boundary Condition: An odd-length string can never form fully closed pairs
        if (s.length() % 2 != 0) {
            return false;
        }

        stack<char> validationStack;

        for (char characterCursor : s) {
            // Push matching closing complements onto the LIFO tracking stack
            if (characterCursor == '(') {
                validationStack.push(')');
            } else if (characterCursor == '{') {
                validationStack.push('}');
            } else if (characterCursor == '[') {
                validationStack.push(']');
            } 
            // Closing bracket validation pass step
            else {
                if (validationStack.empty() || validationStack.top() != characterCursor) {
                    return false;
                }
                validationStack.pop();
            }
        }

        // Structural integrity is valid only if every tracked bracket has been cleanly closed
        return validationStack.empty();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string. 
 *                  The system evaluates each character exactly once in a single forward pass.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The stack holds at most N elements 
 *                   in worst-case layouts consisting entirely of opening sequences (e.g., "(((((()").
 */
int main() {
    cout << "=== LIFO Complementary Bracket Pattern Tracking Validation Console ===\n";
    cout << "Enter the bracket string sequences to validate (e.g., ()[]{}): ";
    
    string testString;
    if (!(cin >> testString)) return 1;

    Solution solver;
    cout << "\nScanning structural boundaries and matching complement balances..." << endl;
    bool outcomeResult = solver.isValid(testString);

    if (outcomeResult) {
        cout << "Validation Result: Structural integrity verified. String is VALID.\n";
    } else {
        cout << "Validation Result: Structural mismatch discovered. String is INVALID.\n";
    }

    return 0;
}