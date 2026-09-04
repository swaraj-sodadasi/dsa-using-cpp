#include <iostream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MINIMUM WINDOW SUBSTRING (SLIDING WINDOW FREQUENCY MATCHING)
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum substring of `s` that contains all characters 
     * of `t` (including duplicate character counts).
     * 
     * 1. CHARACTER FREQUENCY MAPS:
     *    - We construct a target frequency map `targetFreq` for string `t`.
     *    - `requiredCount` represents the total unique characters in `t` that must be satisfied.
     * 
     * 2. TWO-POINTER SLIDING WINDOW:
     *    - Expand `right` pointer and update `windowFreq`.
     *    - When `windowFreq[char] == targetFreq[char]`, increment `formedCount`.
     * 
     * 3. WINDOW SHRINKING & MINIMUM TRACKING:
     *    - While `formedCount == requiredCount`, we have a valid window.
     *    - Track the smallest window length and update starting index `minStart`.
     *    - Advance `left` pointer to shrink window until it becomes invalid.
     */
    string minWindow(const string& s, const string& t) {
        if (s.empty() || t.empty() || s.length() < t.length()) {
            return "";
        }

        vector<int> targetFreq(128, 0);
        int requiredCount = 0;

        for (char c : t) {
            if (targetFreq[static_cast<size_t>(c)] == 0) {
                requiredCount++;
            }
            targetFreq[static_cast<size_t>(c)]++;
        }

        vector<int> windowFreq(128, 0);
        int formedCount = 0;

        int left = 0;
        int minLen = INT_MAX;
        int minStart = 0;

        for (int right = 0; right < static_cast<int>(s.length()); ++right) {
            char rightChar = s[static_cast<size_t>(right)];
            windowFreq[static_cast<size_t>(rightChar)]++;

            if (targetFreq[static_cast<size_t>(rightChar)] > 0 && 
                windowFreq[static_cast<size_t>(rightChar)] == targetFreq[static_cast<size_t>(rightChar)]) {
                formedCount++;
            }

            // Shrink window from the left as long as all target characters are matched
            while (formedCount == requiredCount) {
                int currentLen = right - left + 1;
                if (currentLen < minLen) {
                    minLen = currentLen;
                    minStart = left;
                }

                char leftChar = s[static_cast<size_t>(left)];
                windowFreq[static_cast<size_t>(leftChar)]--;

                if (targetFreq[static_cast<size_t>(leftChar)] > 0 && 
                    windowFreq[static_cast<size_t>(leftChar)] < targetFreq[static_cast<size_t>(leftChar)]) {
                    formedCount--;
                }

                left++;
            }
        }

        return (minLen == INT_MAX) ? "" : s.substr(static_cast<size_t>(minStart), static_cast<size_t>(minLen));
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N is length of string s and M is length of string t.
 *                  Both left and right pointers traverse s at most once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Fixed frequency lookup vectors of size 128 for ASCII.
 */
int main() {
    cout << "=== Minimum Window Substring Engine ===\n";
    cout << "Enter source string s: ";
    string s;
    if (!(cin >> s)) return 1;

    cout << "Enter target string t: ";
    string t;
    if (!(cin >> t)) return 1;

    Solution solver;
    string result = solver.minWindow(s, t);

    cout << "Minimum window substring: [" << result << "]\n";

    return 0;
}