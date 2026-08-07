#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: TWO-POINTER ALPHANUMERIC FILTERING AND COMPARISON
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a string is a palindrome, considering only 
     * alphanumeric characters and ignoring case modifications.
     * 
     * 1. THE IN-PLACE COMPACT DESIGN TRADEOFF:
     *    A naive implementation might allocate a brand new filtered string containing 
     *    only normalized alphanumeric characters. While clean, this costs O(N) auxiliary space. 
     *    To achieve a strict O(1) space constraint, we perform filtering and comparison in place 
     *    using a dual two-pointer collision setup.
     * 
     * 2. AUTOMATED JUMP BOUNDARY OVERLEAPS:
     *    We position two indicators: `left` at the beginning of the string (index 0) and 
     *    `right` at the terminal boundary (`s.length() - 1`).
     *    - At each step, if `s[left]` is not an alphanumeric character, we advance `left` rightward.
     *    - If `s[right]` is not an alphanumeric character, we retract `right` leftward.
     *    - These dynamic skip steps continue until both pointers anchor onto valid characters 
     *      or cross paths (`left >= right`).
     * 
     * 3. CASE-INSENSITIVE RESOLUTION EQUIVALENCE:
     *    Once both pointers land on valid alphanumeric characters, we transform both elements 
     *    to lowercase using `tolower()` and compare them. If they do not match, the string is 
     *    instantly flagged as invalid, returning false. If they match, we move both pointers closer 
     *    together (`left++`, `right--`) and repeat the check.
     */
    bool isPalindrome(string s) {
        int left = 0;
        int right = static_cast<int>(s.length()) - 1;

        while (left < right) {
            // Skip non-alphanumeric characters from the left boundary
            while (left < right && !isalnum(static_cast<unsigned char>(s[static_cast<size_t>(left)]))) {
                left++;
            }
            
            // Skip non-alphanumeric characters from the right boundary
            while (left < right && !isalnum(static_cast<unsigned char>(s[static_cast<size_t>(right)]))) {
                right--;
            }

            // Case-insensitive character comparison validation sweep
            if (tolower(static_cast<unsigned char>(s[static_cast<size_t>(left)])) != 
                tolower(static_cast<unsigned char>(s[static_cast<size_t>(right)]))) {
                return false;
            }

            // Move pointers closer to check the remaining inner substring string
            left++;
            right--;
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string s. 
 *                  Every character is evaluated at most once by the pointers, 
 *                  ensuring a strictly linear one-pass traversal performance.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The pointers scan the string in place, utilizing 
 *                   constant working memory without allocating any extra storage blocks.
 */
int main() {
    string inputString;
    cout << "=== Valid Palindrome Alphanumeric Verifier Console ===\n";
    cout << "Enter the phrase string to evaluate:\n";
    if (!getline(cin, inputString)) return 1;

    Solution solver;
    cout << "\nLaunching two-pointer alphanumeric parsing and case-matching sweeps..." << endl;
    bool outcomeResult = solver.isPalindrome(inputString);

    if (outcomeResult) {
        cout << "Result: The configured input is a VALID palindrome! (true)\n";
    } else {
        cout << "Result: The configured input is NOT a valid palindrome string. (false)\n";
    }

    return 0;
}