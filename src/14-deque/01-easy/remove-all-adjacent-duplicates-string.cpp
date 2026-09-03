#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: REMOVE ALL ADJACENT DUPLICATES IN STRING (IN-PLACE STACK POINTER)
     * -------------------------------------------------------------------------------------
     * The objective is to repeatedly remove adjacent duplicate letters until none remain.
     * 
     * 1. IN-PLACE STRING STACK SIMULATION:
     *    Instead of allocating an auxiliary stack structure or doing costly string 
     *    deletions (O(N^2)), we use an output string `res` as an in-place stack.
     * 
     * 2. SINGLE-PASS TRAVERSAL:
     *    For each character `ch` in the string `s`:
     *      - If `res` is non-empty and `res.back() == ch`, we have an adjacent duplicate.
     *        We pop the top character from `res` (`res.pop_back()`).
     *      - Otherwise, we push `ch` onto `res` (`res.push_back(ch)`).
     */
    string removeDuplicates(const string& s) {
        string res;
        res.reserve(s.length());

        for (char ch : s) {
            if (!res.empty() && res.back() == ch) {
                res.pop_back();
            } else {
                res.push_back(ch);
            }
        }

        return res;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of string s.
 *                  Each character is pushed and popped from the output stack at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Output buffer allocated to construct the final string.
 */
int main() {
    cout << "=== Remove Adjacent Duplicates In String Engine ===\n";
    cout << "Enter input string s: ";
    string s;
    if (!(cin >> s)) return 1;

    Solution solver;
    string result = solver.removeDuplicates(s);

    cout << "String after removing adjacent duplicates: " << result << "\n";

    return 0;
}