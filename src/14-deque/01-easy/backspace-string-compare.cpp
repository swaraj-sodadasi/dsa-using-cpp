#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BACKSPACE STRING COMPARE (TWO-POINTER REVERSE TRAVERSAL)
     * -------------------------------------------------------------------------------------
     * The objective is to determine if strings 's' and 't' are equal after processing 
     * backspaces ('#'), which delete the preceding character.
     * 
     * 1. REVERSE TWO-POINTER TRAVERSAL:
     *    Processing from left to right requires knowing whether future characters are '#', 
     *    which forces stack-like memory overhead O(N). By traversing backwards from the 
     *    end of both strings, we can dynamically count active backspaces and skip 
     *    deleted characters on-the-fly without allocating auxiliary memory.
     * 
     * 2. ON-THE-FLY CHARACTER SKIPPING:
     *    We maintain a skip counter for each string:
     *      - Increment `skip` when encountering '#'.
     *      - Decrement `skip` and skip the character when `skip > 0`.
     *      - Stop at the next valid (non-deleted) character.
     * 
     * 3. CHARACTER COMPARISON & EARLY TERMINATION:
     *    Compare valid characters pointed to by `i` and `j`. If they mismatch, return false. 
     *    If one string exhausts valid characters while the other still has valid characters, 
     *    return false.
     */
    bool backspaceCompare(const string& s, const string& t) {
        int i = static_cast<int>(s.length()) - 1;
        int j = static_cast<int>(t.length()) - 1;

        int skipS = 0;
        int skipT = 0;

        while (i >= 0 || j >= 0) {
            // Find next valid character in string s
            while (i >= 0) {
                if (s[static_cast<size_t>(i)] == '#') {
                    skipS++;
                    i--;
                } else if (skipS > 0) {
                    skipS--;
                    i--;
                } else {
                    break;
                }
            }

            // Find next valid character in string t
            while (j >= 0) {
                if (t[static_cast<size_t>(j)] == '#') {
                    skipT++;
                    j--;
                } else if (skipT > 0) {
                    skipT--;
                    j--;
                } else {
                    break;
                }
            }

            // If valid characters remain in both strings, compare them
            if (i >= 0 && j >= 0) {
                if (s[static_cast<size_t>(i)] != t[static_cast<size_t>(j)]) {
                    return false;
                }
            } 
            // If one string has completed traversal while the other still has valid characters
            else if ((i >= 0) != (j >= 0)) {
                return false;
            }

            i--;
            j--;
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N and M are the lengths of strings s and t.
 *                  Each character in both strings is visited at most once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Constant extra space used for pointers and skip counters.
 */
int main() {
    cout << "=== Backspace String Compare Two-Pointer Engine ===\n";
    cout << "Enter first string s (use '#' for backspaces): ";
    string s;
    if (!(cin >> s)) return 1;

    cout << "Enter second string t (use '#' for backspaces): ";
    string t;
    if (!(cin >> t)) return 1;

    Solution solver;
    bool result = solver.backspaceCompare(s, t);

    if (result) {
        cout << "Strings match after processing backspaces! Output: [true]\n";
    } else {
        cout << "Strings do not match after processing backspaces. Output: [false]\n";
    }

    return 0;
}