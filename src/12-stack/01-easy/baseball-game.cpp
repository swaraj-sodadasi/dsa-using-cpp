#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DYNAMIC INTEGER STACK HISTORY ADJUSTMENTS
     * -------------------------------------------------------------------------------------
     * The objective is to record scores based on a series of character operations.
     * 
     * 1. OPERATIONAL TOKEN TRANSITION LAWS:
     *    We maintain a track record of valid scores using a dynamic vector as a stack:
     *      - Integer String : Parse the value directly and push it onto the stack.
     *      - "+"            : Record a new score that is the sum of the top two scores.
     *      - "D"            : Record a new score that is double the previous score.
     *      - "C"            : Invalidate the last score, popping it off the stack.
     * 
     * 2. ACCUMULATION LAYER:
     *    Once all historical log tokens are processed, we calculate the absolute total 
     *    sum of all remaining scores in the stack.
     */
    int calPoints(const vector<string>& operations) {
        vector<int> scoreHistoryStack;

        for (const string& opToken : operations) {
            if (opToken == "+") {
                size_t historyLength = scoreHistoryStack.size();
                int combinedScore = scoreHistoryStack[historyLength - 1] + scoreHistoryStack[historyLength - 2];
                scoreHistoryStack.push_back(combinedScore);
            } else if (opToken == "D") {
                int doubledScore = scoreHistoryStack.back() * 2;
                scoreHistoryStack.push_back(doubledScore);
            } else if (opToken == "C") {
                scoreHistoryStack.pop_back();
            } else {
                // Parse the numerical string directly into an integer value token
                scoreHistoryStack.push_back(stoi(opToken));
            }
        }

        // Aggregate all structural remaining items to yield the final sum matrix output
        return accumulate(scoreHistoryStack.begin(), scoreHistoryStack.end(), 0);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements inside the operations log vector.
 *                  Every string entry is processed in constant time, and accumulation runs linearly.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The score tracker holds at most N elements concurrently.
 */
int main() {
    cout << "=== Baseball Game Dynamic Score History Stack Engine ===\n";
    cout << "Enter the total number of operations: ";
    int opCount;
    if (!(cin >> opCount) || opCount < 0) return 1;

    vector<string> operations(static_cast<size_t>(opCount));
    if (opCount > 0) {
        cout << "Enter the operational game tokens sequentially (e.g., 5 2 C D +):\n";
        for (int i = 0; i < opCount; ++i) {
            cin >> operations[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nParsing operation tokens and running historical stack corrections..." << endl;
    int grandTotal = solver.calPoints(operations);

    cout << "Calculated Score Record Output: [" << grandTotal << "]\n";

    return 0;
}