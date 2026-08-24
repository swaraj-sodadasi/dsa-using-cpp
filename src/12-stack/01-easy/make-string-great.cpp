#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ADJACENT CASE-INSENSITIVE DISCREPANCY STACK POPS
     * -------------------------------------------------------------------------------------
     * The objective is to make the string "great" by removing adjacent characters that are
     * the same letter but have different cases (e.g., 'a' and 'A').
     * 
     * 1. THE CASE DISCREPANCY EQUATION:
     *    In the standard ASCII table character layout mapping matrix, the absolute numeric
     *    distance between any lowercase letter and its uppercase complement is exactly 32.
     *    Thus, two adjacent characters `c1` and `c2` violate our condition if:
     *    abs(c1 - c2) == 32
     * 
     * 2. IN-PLACE STRING STACK ARCHITECTURE:
     *    Instead of loading characters into an external storage container and incurring
     *    O(N) space overhead, we treat the input string itself as a dynamic stack buffer.
     *    We maintain a tracking index pointer `topIndex` initialized to -1.
     * 
     * 3. CONDITIONAL COLLAPSE SWEEP:
     *    We loop through the string with a forward scanner `i`. For each character `s[i]`:
     *      - If `topIndex >= 0` and `abs(s[topIndex] - s[i]) == 32`, we have hit a case mismatch.
     *        We trigger a stack pop by decrementing `topIndex`.
     *      - Otherwise, the character is safe. We push it onto the stack by incrementing `topIndex`
     *        and writing it down: `s[++topIndex] = s[i]`.
     *    Finally, we crop the string to `topIndex + 1` to clean up residual characters in-place.
     */
    string makeGood(string s) {
        int topIndex = -1;
        int stringLength = static_cast<int>(s.length());

        for (int i = 0; i < stringLength; ++i) {
            if (topIndex >= 0 && abs(s[static_cast<size_t>(topIndex)] - s[static_cast<size_t>(i)]) == 32) {
                // Case discrepancy detected; pop the top item from our in-place stack framework
                topIndex--;
            } else {
                // Stable character matched; push it forward onto the tracking index pointer
                topIndex++;
                s[static_cast<size_t>(topIndex)] = s[static_cast<size_t>(i)];
            }
        }

        // Resize the string in-place to drop the collapsed trailing character remnants
        s.resize(static_cast<size_t>(topIndex + 1));
        return s;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of the input string.
 *                  The single-pass linear sweep processes each character pointer exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The mutation operates entirely inside the string's existing
 *                   memory buffer space, avoiding external heap storage configurations.
 */
int main() {
    cout << "=== Case-Insensitive Discrepancy Stack Elimination Engine ===\n";
    cout << "Enter the target string sequence to clean up: ";
    
    string inputString;
    if (!(cin >> inputString)) return 1;

    cout << "\nOriginal String Configuration: [" << inputString << "]\n";
    
    Solution solver;
    cout << "Processing character boundaries and executing value matrix collapses..." << endl;
    string optimizedResult = solver.makeGood(inputString);

    cout << "Processed Outcome Result:     [" << optimizedResult << "]\n";

    return 0;
}