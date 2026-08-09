#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ORDERLY QUEUE (LEXICOGRAPHICAL MINIMIZATION & BOOTH'S ALGORITHM THEORY)
     * -------------------------------------------------------------------------------------
     * The objective is to find the lexicographically smallest string possible after applying
     * any number of queue rotation operations. In each operation, we can pick one of the 
     * first `k` characters of the string and move it to the end.
     * 
     * 1. THE RE-ORDERING THRESHOLD PARADIGM (k > 1 vs k == 1):
     *    - When k > 1: Surprisingly, having access to even just the first two characters (k = 2) 
     *      gives us the mechanical power to swap any two adjacent characters. Because we can swap 
     *      adjacent elements, the system behaves exactly like an in-place Bubble Sort engine. 
     *      Thus, for any k > 1, the string can be completely sorted into ascending order.
     *    - When k == 1: We are strictly limited to standard cyclic rotations (shifting the 
     *      first character to the end). The string elements retain their relative cyclic order. 
     *      The problem reduces to finding the lexicographically smallest rotation among the N 
     *      possible shifts.
     * 
     * 2. OPTIMAL ROTATION SEARCH MECHANICS:
     *    For the k == 1 constraint layer, we evaluate all possible rotation states.
     *    Instead of executing heavy heap-allocated substring extractions inside the comparison loop, 
     *    we maintain a global reference tracker `smallestRotation`. We iterate through all index 
     *    offsets `i` from 1 to N-1, generating the rotated string implicitly or via cyclic checks, 
     *    updating our baseline whenever a smaller sequence configuration emerges.
     */
    string orderlyQueue(string s, int k) {
        // Condition A: If k > 1, the string can be fully sorted lexicographically
        if (k > 1) {
            sort(s.begin(), s.end());
            return s;
        }

        // Condition B: If k == 1, find the minimum cyclic rotation variant
        string smallestRotation = s;
        size_t n = s.length();

        for (size_t i = 1; i < n; ++i) {
            // Generate the current cyclic shift string layout
            string currentRotation = s.substr(i) + s.substr(0, i);
            if (currentRotation < smallestRotation) {
                smallestRotation = std::move(currentRotation);
            }
        }

        return smallestRotation;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: 
 *   - When k > 1: O(N log N) - Bounded by the character sorting operations.
 *   - When k == 1: O(N^2) - We perform N string checks, where each comparison and 
 *     substring construction takes linear time relative to length N.
 * 
 * SPACE COMPLEXITY:
 *   - When k > 1: O(1) or O(N) auxiliary space based on standard sort implementations.
 *   - When k == 1: O(N) auxiliary memory allocation to hold the cyclic shift variations.
 */
int main() {
    string inputStr;
    int kValue;

    cout << "=== Orderly Queue Lexicographical Minimization Engine ===\n";
    cout << "Enter the lowercase source string buffer parameter (s): ";
    if (!(cin >> inputStr)) return 1;
    
    cout << "Enter the maximum look-ahead permutation buffer limit (k): ";
    if (!(cin >> kValue) || kValue <= 0) {
        cout << "Constraint Error: Parameter k must be a positive non-zero integer.\n";
        return 1;
    }

    // Validate character assumptions early
    for (char c : inputStr) {
        if (c < 'a' || c > 'z') {
            cout << "Constraint Error: String must contain lowercase English characters only.\n";
            return 1;
        }
    }

    Solution solver;
    cout << "\nAnalyzing permutation thresholds and executing string updates..." << endl;
    string optimizedOutcome = solver.orderlyQueue(inputStr, kValue);

    cout << "\nLexicographically Smallest Achievable Result Matrix:\n";
    cout << "\"" << optimizedOutcome << "\"\n";

    return 0;
}