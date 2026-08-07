#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: TWO-POINTER FREQUENCY GOAL-STATE COUNTER TRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to discover the minimum contiguous substring within string `s` that 
     * contains every character of string `t` (including duplicate occurrences).
     * 
     * 1. FIXED-CAPACITY INTENSITY PROFILES:
     *    Instead of heavy heap allocations via generic map frameworks, we initialize two 
     *    fixed-size tracking arrays of size 128 (`targetCounts` and `windowCounts`). This 
     *    covers all possible extended ASCII character mapping configurations in true O(1) space.
     * 
     * 2. GOAL-STATE SATISFACTION COUNTER:
     *    We count the number of *unique characters* in `t` that must meet a minimum frequency 
     *    requirement, stored as `requiredMatches`. We then maintain a running tracker `formedMatches` 
     *    to record how many distinct characters in our current sliding window have met that goal. 
     *    This avoids performing a 128-step array comparison loop at every iteration.
     * 
     * 3. THE TWO-POINTER SLIDING CONTRACT:
     *    - Expansion: We expand the window by moving the `right` pointer forward, adding characters 
     *      to `windowCounts`. When a character's frequency matches its count in `t`, we increment `formedMatches`.
     *    - Contraction: Once `formedMatches == requiredMatches`, the window is fully qualified. We update 
     *      our global minimum window records, then incrementally advance the `left` pointer to strip away 
     *      unnecessary leading characters until the window is no longer valid.
     */
    string minWindow(string s, string t) {
        if (s.empty() || t.empty() || s.length() < t.length()) {
            return "";
        }

        // Step 1: Initialize fixed 128-element ASCII occurrence maps
        vector<int> targetCounts(128, 0);
        vector<int> windowCounts(128, 0);

        // Map character frequencies required by target template string t
        for (char c : t) {
            targetCounts[static_cast<size_t>(c)]++;
        }

        // Calculate total unique characters in t that need to meet their frequency criteria
        int requiredMatches = 0;
        for (int i = 0; i < 128; ++i) {
            if (targetCounts[static_cast<size_t>(i)] > 0) {
                requiredMatches++;
            }
        }

        int left = 0;
        int formedMatches = 0;
        
        // Window bounds tracking matrices
        int minWindowLength = INT_MAX;
        int minimumStartIdx = 0;

        // Step 3: Expand the sliding window across target string s
        for (int right = 0; right < static_cast<int>(s.length()); ++right) {
            char incomingChar = s[static_cast<size_t>(right)];
            windowCounts[static_cast<size_t>(incomingChar)]++;

            // If the incoming character meets its exact frequency goal in t, count it as a match
            if (targetCounts[static_cast<size_t>(incomingChar)] > 0 && 
                windowCounts[static_cast<size_t>(incomingChar)] == targetCounts[static_cast<size_t>(incomingChar)]) {
                formedMatches++;
            }

            // Contraction Phase: Shrink the window from the left once all requirements are satisfied
            while (formedMatches == requiredMatches) {
                int currentWindowWidth = right - left + 1;
                
                // Update global min boundaries if a tighter window is found
                if (currentWindowWidth < minWindowLength) {
                    minWindowLength = currentWindowWidth;
                    minimumStartIdx = left;
                }

                char outgoingChar = s[static_cast<size_t>(left)];
                
                // If the character being removed drops below its required frequency, decrement our matches counter
                if (targetCounts[static_cast<size_t>(outgoingChar)] > 0 && 
                    windowCounts[static_cast<size_t>(outgoingChar)] == targetCounts[static_cast<size_t>(outgoingChar)]) {
                    formedMatches--;
                }
                
                windowCounts[static_cast<size_t>(outgoingChar)]--;
                left++; // Slide left boundary rightward
            }
        }

        return (minWindowLength == INT_MAX) ? "" : s.substr(static_cast<size_t>(minimumStartIdx), static_cast<size_t>(minWindowLength));
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N represents the length of string s, and M represents the length of 
 *                  string t. The initial map setup takes O(M) time. The sliding window sweep takes O(N) 
 *                  time because both the left and right pointers visit each index in string s at most twice. 
 *                  Character profile evaluations run in true O(1) constant time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The internal frequency arrays maintain a fixed capacity of 128 integers, 
 *                   independent of the lengths of strings s and t.
 */
int main() {
    string s, t;
    cout << "Enter the primary search source string (s): ";
    if (!getline(cin, s)) return 1;
    cout << "Enter the target match pattern string (t): ";
    if (!getline(cin, t)) return 1;

    Solution solver;
    cout << "\nExecuting sliding window goal-state frequency evaluations..." << endl;
    string computedWindow = solver.minWindow(s, t);

    if (computedWindow.empty()) {
        cout << "Outcome: No satisfying substring configuration discovered." << endl;
    } else {
        cout << "Minimum matching contiguous substring segment isolated: \"" << computedWindow << "\"" << endl;
        cout << "Minimum Window Width Size: " << computedWindow.length() << " characters." << endl;
    }

    return 0;
}