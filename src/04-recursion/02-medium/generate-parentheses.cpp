#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
CORE LOGIC (CATALAN-BASED RECURSIVE VALIDATION):
- The problem asks us to generate all combinations of well-formed parentheses containing exactly `n` pairs.
- **Structural Constraints for Well-Formed Parentheses**:
  - A combination string under construction is guaranteed to remain valid if and only if:
    1. We have not exceeded the total allocation allowance for opening parentheses: `openCount < n`.
    2. An opening parenthesis must exist to balance out a closing parenthesis: `closeCount < openCount`.
- **Recursive State Representation**:
  - We track the generation state via:
    - `openCount`: The number of opening parentheses `'('` placed so far.
    - `closeCount`: The number of closing parentheses `')'` placed so far.
    - `currentString`: The working buffer string containing the active sequence.
- **State Transition Strategy**:
  - **Base Case**: When `currentString.length() == 2 * n`, all pairs are filled. The string is guaranteed to be well-formed due to our placement rules, so we save it.
  - **Branching Decisions**:
    - If `openCount < n`, we can push an opening parenthesis: `currentString + '('` and increment `openCount`.
    - If `closeCount < openCount`, we can push a closing parenthesis: `currentString + ')'` and increment `closeCount`.

TIME COMPLEXITY: O((4^N) / (N * sqrt(N))) - The total number of valid combinations matches the $N$-th Catalan Number:
                 $$ C_N = \frac{1}{N+1}\binom{2N}{N} $$
                 This scales well within the constraints as the logic prunes invalid paths immediately.
SPACE COMPLEXITY: O(N) auxiliary - The depth of the runtime recursion stack scales linearly with the number of pairs $2N$.
*/

class Solution {
private:
    vector<string> allCombinations;

    void buildParentheses(int n, int openCount, int closeCount, string& currentString) {
        // Base Case: The string length matches the total length of all pairs
        if (static_cast<int>(currentString.length()) == 2 * n) {
            allCombinations.push_back(currentString);
            return;
        }

        // Branching Decision 1: We can always add an opening parenthesis if we haven't hit the limit 'n'
        if (openCount < n) {
            currentString.push_back('(');
            buildParentheses(n, openCount + 1, closeCount, currentString);
            currentString.pop_back(); // Backtracking state recovery step
        }

        // Branching Decision 2: We can add a closing parenthesis only if it balances an existing open one
        if (closeCount < openCount) {
            currentString.push_back(')');
            buildParentheses(n, openCount, closeCount + 1, currentString);
            currentString.pop_back(); // Backtracking state recovery step
        }
    }

public:
    vector<string> generateParenthesis(int n) {
        allCombinations.clear();
        if (n <= 0) return allCombinations;

        string currentString = "";
        buildParentheses(n, 0, 0, currentString);
        return allCombinations;
    }
};

int main() {
    int n;
    cout << "Enter the number of pairs of parentheses (n): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input sizing configuration parameter bounds." << endl;
        return 1;
    }

    Solution solver;
    vector<string> result = solver.generateParenthesis(n);

    cout << "\nAll valid combinations generated via Catalan-bounded backtracking:\n";
    cout << "[\n";
    for (const string& combination : result) {
        cout << "  \"" << combination << "\"\n";
    }
    cout << "]\n";
    cout << "Total well-formed combinations found: " << result.size() << endl;

    return 0;
}