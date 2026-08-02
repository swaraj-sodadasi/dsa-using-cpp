#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * GREEDY DEQUE WITH FREQUENCY CONSTRAINTS ENGINE
     * ----------------------------------------------
     * The objective is to find the lexicographically smallest subsequence of length `k` 
     * that contains at least `repetition` occurrences of a specific `letter`.
     *
     * Algorithmic Strategy:
     * - We use a dynamic container (vector/deque behaving like a stack) to construct the result string.
     * - As we iterate through the string, we want to pop larger characters out of our stack if the 
     *   current character is smaller, improving the lexicographical order.
     * - **Safety Invariants for Popping:** We can only pop a character if:
     *   1. The remaining characters in the string, combined with what's left in our stack, 
     *      can still fill the required length `k`.
     *   2. If the character we want to pop matches `letter`, the remaining target characters in 
     *      the suffix, combined with what's left in our stack, can still meet the `repetition` quota.
     *
     * Post-processing Construction Step:
     * - After sweeping the string, if the container contains more than `k` elements, we must trim it.
     * - However, we cannot blindly slice off the tail if it means dropping the target `letter` 
     *   below the `repetition` threshold. We must selectively preserve the required count of `letter`.
     */
    string smallestSubsequence(string s, int k, char letter, int repetition) {
        // Step 1: Count total occurrences of the target letter in the entire string
        int remainingTargetLetters = 0;
        for (char c : s) {
            if (c == letter) remainingTargetLetters++;
        }

        string resultStack = "";
        int targetLettersInStack = 0;
        int n = static_cast<int>(s.length());

        // Step 2: Linear sweep with greedy pruning
        for (int i = 0; i < n; ++i) {
            char currentVal = s[i];

            // Maintain monotonic lexicographically small sequence
            while (!resultStack.empty() && resultStack.back() > currentVal) {
                char charToPop = resultStack.back();
                
                // Calculate remaining elements available in the suffix stream
                int elementsRemainingInSuffix = n - i;
                int potentialTotalLength = static_cast<int>(resultStack.length()) - 1 + elementsRemainingInSuffix;
                
                // Track availability if the character to pop is our target letter
                int isTargetLetter = (charToPop == letter) ? 1 : 0;
                int potentialTargetLettersTotal = targetLettersInStack - isTargetLetter + remainingTargetLetters;

                // Pruning Condition: Break if popping violates length or frequency constraints
                if (potentialTotalLength < k || potentialTargetLettersTotal < repetition) {
                    break;
                }

                // Safe to pop
                if (charToPop == letter) {
                    targetLettersInStack--;
                }
                resultStack.pop_back();
            }

            // Push current character onto the stack
            resultStack.push_back(currentVal);
            if (currentVal == letter) {
                targetLettersInStack++;
                remainingTargetLetters--;
            } else {
                // If it's not the target letter, it still consumes a position in the suffix pool
            }
        }

        // Step 3: Handle post-processing truncation to fit exactly length 'k'
        string finalSubsequence = "";
        int targetLettersNeeded = repetition;
        int elementsToKeep = k;

        for (char c : resultStack) {
            if (elementsToKeep == 0) break;

            // If we have excess characters but desperately need target letters, we must skip non-target letters
            if (elementsToKeep < static_cast<int>(resultStack.length()) && c != letter && elementsToKeep == targetLettersNeeded) {
                // Skip this non-target letter to save slots for required target letters downstream
                continue;
            }

            finalSubsequence.push_back(c);
            elementsToKeep--;
            if (c == letter) {
                targetLettersNeeded--;
            }
        }

        return finalSubsequence;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string `s`. 
 *                  Each character is pushed onto and popped from the stack at most once, 
 *                  ensuring linear complexity despite the nested while loop.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The stack buffer collects valid characters dynamically, 
 *                   utilizing storage proportional to the length of the string sequence.
 */
int main() {
    string s;
    cout << "Enter the input string sequence (lowercase alphabets): ";
    if (!(cin >> s)) return 1;

    int k;
    cout << "Enter the targeted subsequence length constraint (k): ";
    if (!(cin >> k) || k <= 0 || k > static_cast<int>(s.length())) {
        cout << "Constraint Error: Invalid subsequence size parameter." << endl;
        return 1;
    }

    char letter;
    cout << "Enter the required target character factor: ";
    if (!(cin >> letter)) return 1;

    int repetition;
    cout << "Enter the minimum frequency repeat limit (repetition): ";
    if (!(cin >> repetition) || repetition < 0 || repetition > k) {
        cout << "Constraint Error: Repetition limit must fall between 0 and k." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting greedy deque with frequency constraints sweep..." << endl;
    string result = solver.smallestSubsequence(s, k, letter, repetition);

    cout << "Lexicographically smallest k-length subsequence outcome: " << result << endl;

    return 0;
}