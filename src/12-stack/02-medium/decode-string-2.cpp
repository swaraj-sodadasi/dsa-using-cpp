#include <iostream>
#include <string>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTIPLIER STACK INTERLEAVED WITH CHARACTER SEGMENT BUFFER STACKS
     * -------------------------------------------------------------------------------------
     * The objective is to decode an encoded string following the formula pattern "k[encoded_string]".
     * 
     * 1. THE INTERLEAVED DUAL-STACK ARCHITECTURE:
     *    Nested brackets require tracking context layers. We maintain two distinct stacks:
     *      - countsStack: Stores the repeat factor integers (`k`).
     *      - resultStack: Stores the string segment tracks built before opening brackets (`[`).
     * 
     * 2. LOGICAL SCAN TRANSITION RULES:
     *    We parse the input string from left to right, matching four unique character tokens:
     *      - Digit : Build the integer multiplier factor value dynamically (`currentK`).
     *      - `[`   : Context Switch. Push the completed `currentK` onto `countsStack` and the 
     *                active working buffer `currentString` onto `resultStack`. Reset both variables.
     *      - `]`   : Evaluation & Liquidation. Pop the parent string track from `resultStack` and 
     *                the multiplier count from `countsStack`. Repeat the current string buffer `count` 
     *                times, append it onto the parent track, and make it our new working buffer.
     *      - Alpha : Append alphabetical letters directly onto our `currentString` buffer.
     */
    string decodeString(string s) {
        stack<int> countsStack;
        stack<string> resultStack;
        string currentString = "";
        int currentK = 0;

        for (char ch : s) {
            if (isdigit(ch)) {
                // Accumulate base-10 integer values from consecutive numerical tokens
                currentK = currentK * 10 + (ch - '0');
            } else if (ch == '[') {
                // Push current execution tracks onto their respective stack frames
                countsStack.push(currentK);
                resultStack.push(currentString);
                
                // Reset lane states to process fresh inner bracket scopes
                currentK = 0;
                currentString = "";
            } else if (ch == ']') {
                // Extract parent context boundaries
                string decodedSegment = resultStack.top();
                resultStack.pop();
                
                int repeatCount = countsStack.top();
                countsStack.pop();

                // Append the localized buffer sequence repeatCount times onto the parent string
                while (repeatCount > 0) {
                    decodedSegment += currentString;
                    repeatCount--;
                }
                
                // Update the main operational workspace tracking channel
                currentString = decodedSegment;
            } else {
                // Accumulate direct raw character arrays
                currentString += ch;
            }
        }

        return currentString;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(MaxK * N) - Where N represents the length of string s and MaxK is the largest 
 *                  multiplier value. In the worst-case, character allocations scale based on the 
 *                  total number of characters generated in the final decoded matrix.
 *
 * SPACE COMPLEXITY: O(M + K) - Where M is the maximum bracket nesting depth (stack frame sizes) 
 *                   and K is the absolute size configuration of the resulting string output buffer.
 */
int main() {
    cout << "=== Dual Stack Decoded String Parsing Engine ===\n";
    cout << "Enter the raw encoded string matrix (e.g., 3[a]2[bc]): ";
    
    string rawInput;
    if (!(cin >> rawInput)) return 1;

    Solution solver;
    cout << "Parsing multiplier indices and decompressing string segment buffers..." << endl;
    string decodedResult = solver.decodeString(rawInput);

    cout << "Final Decoded Outcome Matrix: [" << decodedResult << "]\n";

    return 0;
}