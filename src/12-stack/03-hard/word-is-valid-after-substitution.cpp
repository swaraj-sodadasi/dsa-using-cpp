#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: PATTERN SUBSTRING INSERTION STACK SIMULATION
     * -------------------------------------------------------------------------------------
     * A string is valid if it can be formed by repeatedly inserting the pattern "abc" 
     * at any position. This means we can validate it by reversing the process: deleting 
     * occurrences of "abc" until the string is completely cleared.
     * 
     * 1. THE STACK MATCHING MECHANISM:
     *    Instead of modifying or searching the string repeatedly (which takes quadratic time),
     *    we build the valid string character-by-character using an in-place string stack.
     * 
     * 2. STATE TRANSITION RULES:
     *    We push each character `ch` onto our `charStack`. 
     *    Every time we push a 'c', we inspect the last two elements added before it. 
     *    If the stack length is at least 3, and the top three elements form the pattern 
     *    "abc" ('a' followed by 'b' followed by 'c'), we immediately pop all three characters.
     * 
     * 3. TERMINAL VALIDITY CHECK:
     *    If the input string was built perfectly by valid substitutions, all matching patterns
     *    will be fully cleared, leaving the `charStack` empty. If any elements remain, the 
     *    structural balance invariant was violated, meaning the string is invalid.
     */
    bool isValid(string s) {
        string charStack = "";

        for (char ch : s) {
            charStack.push_back(ch);

            // Trigger structural contraction check whenever a token boundary 'c' is encountered
            if (ch == 'c' && charStack.length() >= 3) {
                size_t len = charStack.length();
                if (charStack[len - 2] == 'b' && charStack[len - 3] == 'a') {
                    // Evaporate the complete sequential pattern sub-segment match frame
                    charStack.pop_back(); // Remove 'c'
                    charStack.pop_back(); // Remove 'b'
                    charStack.pop_back(); // Remove 'a'
                }
            }
        }

        return charStack.empty();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input validation string.
 *                  Every character is appended to the tracking stack and popped at most once.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The implicit vector array container stores 
 *                   at most N characters if no match contractions trigger (e.g., "aaaaaab").
 */
int main() {
    cout << "=== Substitution Pattern Insertion Stack Verification Engine ===\n";
    cout << "Enter the target string to validate (e.g., aabcbc): ";
    
    string inputStr;
    if (!(cin >> inputStr)) return 1;

    Solution solver;
    cout << "Evaluating sequence tracks and running pattern contraction passes..." << endl;
    bool resultFlag = solver.isValid(inputStr);

    cout << "Validation Outcome Verdict: [" << (resultFlag ? "VALID" : "INVALID") << "]\n";

    return 0;
}