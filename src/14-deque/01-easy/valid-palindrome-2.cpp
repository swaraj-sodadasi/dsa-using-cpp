#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: VALID PALINDROME (TWO-POINTER OUTER-TO-INNER EMULATION)
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a string is a palindrome, considering only 
     * alphanumeric characters and ignoring cases.
     * 
     * 1. OUTER-TO-INNER TWO-POINTER SWEEP:
     *    We maintain `left` at index 0 and `right` at the last index.
     *    - Advance `left` until it points to an alphanumeric character.
     *    - Decrement `right` until it points to an alphanumeric character.
     * 
     * 2. IN-PLACE CASE-INSENSITIVE COMPARISON:
     *    Compare `tolower(s[left])` and `tolower(s[right])`. If they mismatch, return false.
     *    This achieves O(N) time and O(1) space without allocating intermediate strings.
     */
    bool isPalindrome(const string& s) {
        int left = 0;
        int right = static_cast<int>(s.length()) - 1;

        while (left < right) {
            // Skip non-alphanumeric characters from left boundary
            while (left < right && !isalnum(static_cast<unsigned char>(s[static_cast<size_t>(left)]))) {
                left++;
            }
            // Skip non-alphanumeric characters from right boundary
            while (left < right && !isalnum(static_cast<unsigned char>(s[static_cast<size_t>(right)]))) {
                right--;
            }

            if (left < right) {
                char charLeft = static_cast<char>(tolower(static_cast<unsigned char>(s[static_cast<size_t>(left)])));
                char charRight = static_cast<char>(tolower(static_cast<unsigned char>(s[static_cast<size_t>(right)])));

                if (charLeft != charRight) {
                    return false;
                }
                left++;
                right--;
            }
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of string s.
 *                  Each character is inspected at most once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - In-place boundary checks without string duplication.
 */
int main() {
    cout << "=== Valid Palindrome Two-Pointer Engine ===\n";
    cout << "Enter input string s: ";
    string s;
    if (!getline(cin, s)) return 1;

    Solution solver;
    bool result = solver.isPalindrome(s);

    if (result) {
        cout << "String is a valid palindrome! Output: [true]\n";
    } else {
        cout << "String is NOT a valid palindrome. Output: [false]\n";
    }

    return 0;
}