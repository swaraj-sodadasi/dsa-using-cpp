#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: NESTED MULTIPLIER STACK EXPRESSION PARSING (WITH FULLY QUALIFIED std::move)
     * -------------------------------------------------------------------------------------
     * The objective is to decode an encoded string where the pattern `k[encoded_string]` 
     * means the `encoded_string` inside the square brackets must be repeated exactly `k` times.
     * 
     * FIXED WARNING:
     *   Replaced naked 'move()' invocations with fully qualified 'std::move()' blocks.
     *   This eliminates the "Unqualified call to 'std::move'" static analysis diagnostics.
     */
    string decodeString(string s) {
        stack<int> countStack;
        stack<string> stringStack;
        
        string currentString = "";
        int currentK = 0;

        for (char c : s) {
            // Case A: Accumulate incoming numeric digit bits
            if (isdigit(static_cast<unsigned char>(c))) {
                currentK = currentK * 10 + (c - '0');
            } 
            // Case B: Push outer state and descend into a nested bracket framework layer
            else if (c == '[') {
                countStack.push(currentK);
                // FIXED: Explicitly qualified std::move call implemented here
                stringStack.push(std::move(currentString));
                
                // Clear state registers for the incoming nested context sequence
                currentK = 0;
                currentString = "";
            } 
            // Case C: Pop configurations and resolve the current nested iteration matrix
            else if (c == ']') {
                int repeatCount = countStack.top();
                countStack.pop();
                
                // FIXED: Explicitly qualified std::move call implemented here
                string decodedSegment = std::move(stringStack.top());
                stringStack.pop();

                // Append the current segment repeated 'repeatCount' times to the historical base
                decodedSegment.reserve(decodedSegment.length() + (currentString.length() * static_cast<size_t>(repeatCount)));
                for (int i = 0; i < repeatCount; ++i) {
                    decodedSegment.append(currentString);
                }
                
                // FIXED: Explicitly qualified std::move call implemented here
                currentString = std::move(decodedSegment);
            } 
            // Case D: Append basic alphabet characters directly to the tracking register
            else {
                currentString.push_back(c);
            }
        }

        return currentString;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(Max_K * N) - Where N represents the total character length of the source string s, 
 *                  and Max_K is the highest multiplier value configured. 
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The parallel execution stacks store historical states proportional 
 *                   to the max nesting depth of the bracket constraints inside the string expression.
 */
int main() {
    string encodedExpr;
    cout << "=== Decode String Nested Multiplier Stack Console ===\n";
    cout << "Enter the structural expression string to decode (e.g., 3[a]2[bc], 3[a2[c]]):\n";
    if (!(cin >> encodedExpr)) return 1;

    // Validate structure bracket layout rules early
    int structuralBalanceCheck = 0;
    for (char c : encodedExpr) {
        if (c == '[') structuralBalanceCheck++;
        if (c == ']') structuralBalanceCheck--;
        if (structuralBalanceCheck < 0) {
            cout << "Constraint Error: Unbalanced closing brackets discovered early." << endl;
            return 1;
        }
    }
    if (structuralBalanceCheck != 0) {
        cout << "Constraint Error: Missing closing brackets inside string layout parameters." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nResolving syntax expressions via nested context memory buffers..." << endl;
    string decodedOutcome = solver.decodeString(encodedExpr);

    cout << "\nSuccessfully Decoded Output String:\n\"" << decodedOutcome << "\"\n";
    cout << "Total Character Sequence Width: " << decodedOutcome.length() << " characters.\n";

    return 0;
}