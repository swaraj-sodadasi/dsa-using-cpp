#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 1: STACK WEIGHT LEVEL DEPTH TRACKING (WITH DEFENSIVE STRUCTURAL GUARDS)
     * -------------------------------------------------------------------------------------
     * Maintains a running score stack. Added defensive validations protect against 
     * malformed string inputs that break standard balancing invariants.
     * 
     * TIME COMPLEXITY: O(N) 
     * SPACE COMPLEXITY: O(N)
     */
    int scoreOfParenthesesStack(string s) {
        stack<int> scoreStack;
        scoreStack.push(0); // Base level layer fallback

        for (char ch : s) {
            if (ch == '(') {
                scoreStack.push(0);
            } else if (ch == ')') {
                // Defensive guard: Ensure stack contains elements before reading top context frame
                if (scoreStack.size() < 2) {
                    continue; // Skip structural anomalies to prevent stack underflow core dumps
                }
                
                int innerScore = scoreStack.top();
                scoreStack.pop();
                
                int processedScore = max(2 * innerScore, 1);
                scoreStack.top() += processedScore;
            }
        }

        return scoreStack.empty() ? 0 : scoreStack.top();
    }

    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 2: BIT-SHIFTING MULTIPLICATION (UNDERFLOW INDEX DEFENSE)
     * -------------------------------------------------------------------------------------
     * Implements an index boundary check (`i > 0`) to prevent out-of-bounds segmentation
     * violations when checking the historical index footprint `s[i - 1]`.
     * 
     * TIME COMPLEXITY: O(N)
     * SPACE COMPLEXITY: O(1) Auxiliary
     */
    int scoreOfParenthesesBitShift(string s) {
        int totalScore = 0;
        int depth = 0;
        int n = static_cast<int>(s.length());

        for (int i = 0; i < n; ++i) {
            if (s[static_cast<size_t>(i)] == '(') {
                depth++;
            } else if (s[static_cast<size_t>(i)] == ')') {
                depth--;
                // Check if this ')' is part of a core base case "()"
                // Guard requirement: i > 0 ensures reading s[i - 1] doesn't trigger segmentation faults
                if (i > 0 && s[static_cast<size_t>(i - 1)] == '(') {
                    // Prevent undefined behavior from negative bitwise shifts on malformed syntax
                    if (depth >= 0) {
                        totalScore += (1 << depth);
                    }
                }
            }
        }

        return totalScore;
    }
};

int main() {
    cout << "=== Score of Parentheses Dual-Engine Console ===\n";
    cout << "Enter a balanced parentheses string matrix (e.g., (()(()))): ";
    
    string testString;
    // Utilize getline to safely capture string sequences containing potential whitespaces or blanks
    if (!getline(cin, testString) || testString.empty()) {
        cout << "Error: Invalid string matrix input sequence." << endl;
        return 0;
    }

    Solution solver;

    cout << "\nExecuting Engine Pass 1 (Stack Weight Level Depth Tracking)..." << endl;
    int outcome1 = solver.scoreOfParenthesesStack(testString);
    cout << "Calculated Score Metric (Engine 1): [" << outcome1 << "]\n";

    cout << "\nExecuting Engine Pass 2 (Bit-Shifting Multiplication)..." << endl;
    int outcome2 = solver.scoreOfParenthesesBitShift(testString);
    cout << "Calculated Score Metric (Engine 2): [" << outcome2 << "]\n";

    return 0;
}