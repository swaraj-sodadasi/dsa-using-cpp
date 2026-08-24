#include <iostream>
#include <string>

using namespace std;

class Solution {
private:
    /*
     * APPROACH 1 HELPER: IN-PLACE VECTOR STACK STRING REDUCTION
     * Simulates backspace operations using the string's own memory buffer as a stack.
     */
    string buildStackRepresentation(string s) {
        int topIndex = -1;
        for (char ch : s) {
            if (ch == '#') {
                if (topIndex >= 0) {
                    topIndex--;
                }
            } else {
                topIndex++;
                s[static_cast<size_t>(topIndex)] = ch;
            }
        }
        s.resize(static_cast<size_t>(topIndex + 1));
        return s;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 1: STACK REPRESENTATION COMPARISON (IN-PLACE REDUCTION)
     * -------------------------------------------------------------------------------------
     * Processes both strings by collapsing backspace characters in-place using a stack-based
     * strategy, then compares the final cleaned string matrices directly.
     * 
     * TIME COMPLEXITY: O(M + N) - Where M and N are the lengths of strings `s` and `t`.
     * SPACE COMPLEXITY: O(1) Auxiliary - Modifies string memory allocations in-place.
     */
    bool backspaceCompareStack(string s, string t) {
        return buildStackRepresentation(s) == buildStackRepresentation(t);
    }

    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 2: DUAL-PASS REVERSE ITERATORS (TRUE ZERO-MUTATION MEMORY SPACE)
     * -------------------------------------------------------------------------------------
     * Scans both strings from back to front using two pointers. We maintain counts of 
     * active backspaces (`sSkip`, `tSkip`) to skip over characters that would be deleted.
     * 
     * TIME COMPLEXITY: O(M + N) - Single-pass reverse linear sweep across string bounds.
     * SPACE COMPLEXITY: O(1) Auxiliary - Relies entirely on pointer index coordinates.
     */
    bool backspaceCompareIterators(string s, string t) {
        int sIdx = static_cast<int>(s.length()) - 1;
        int tIdx = static_cast<int>(t.length()) - 1;

        int sSkip = 0;
        int tSkip = 0;

        while (sIdx >= 0 || tIdx >= 0) {
            // Sector 1: Find next valid character position in string s
            while (sIdx >= 0) {
                if (s[static_cast<size_t>(sIdx)] == '#') {
                    sSkip++;
                    sIdx--;
                } else if (sSkip > 0) {
                    sSkip--;
                    sIdx--;
                } else {
                    break;
                }
            }

            // Sector 2: Find next valid character position in string t
            while (tIdx >= 0) {
                if (t[static_cast<size_t>(tIdx)] == '#') {
                    tSkip++;
                    tIdx--;
                } else if (tSkip > 0) {
                    tSkip--;
                    tIdx--;
                } else {
                    break;
                }
            }

            // Check if one string exhausted its characters while the other has printable remnants
            if ((sIdx >= 0) != (tIdx >= 0)) {
                return false;
            }

            // Compare both active valid characters
            if (sIdx >= 0 && tIdx >= 0) {
                if (s[static_cast<size_t>(sIdx)] != t[static_cast<size_t>(tIdx)]) {
                    return false;
                }
            }

            sIdx--;
            tIdx--;
        }

        return true;
    }
};

int main() {
    cout << "=== Backspace String Compare Double-Engine Matrix Console ===\n";
    cout << "Enter the first raw string sequence (S): ";
    string sStr;
    if (!(cin >> sStr)) return 1;

    cout << "Enter the second raw string sequence (T): ";
    string tStr;
    if (!(cin >> tStr)) return 1;

    Solution solver;

    cout << "\nExecuting Engine Pass 1 (In-place Vector Stack Reduction)..." << endl;
    bool result1 = solver.backspaceCompareStack(sStr, tStr);
    cout << "Outcome Matrix (Engine 1): " << (result1 ? "[MATCHING EQUALITY]" : "[MISMATCH DISCOVERED]") << "\n";

    cout << "\nExecuting Engine Pass 2 (Dual-Pass Reverse Iterators)..." << endl;
    bool result2 = solver.backspaceCompareIterators(sStr, tStr);
    cout << "Outcome Matrix (Engine 2): " << (result2 ? "[MATCHING EQUALITY]" : "[MISMATCH DISCOVERED]") << "\n";

    return 0;
}