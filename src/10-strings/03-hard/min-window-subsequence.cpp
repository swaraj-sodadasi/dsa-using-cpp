#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: TWO-POINTER DOUBLE-PASS ALIGNMENT SCAN (DYNAMIC WINDOW CONTRACTION)
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum contiguous substring of `s1` that contains `s2` 
     * as a sequential subsequence. If there are multiple such windows, we return the one 
     * with the smallest starting index.
     * 
     * 1. THE CHALLENGE OF STANDARD SLIDING WINDOWS:
     *    A standard one-way sliding window handles set membership (e.g., finding a window 
     *    containing characters in any order). However, since `s2` requires a strict 
     *    sequential order, a single forward pass often creates oversized windows that 
     *    miss the optimal (tightest) starting boundary.
     * 
     * 2. THE TWO-POINTER DOUBLE-PASS ALIGNMENT ENGINE:
     *    Instead of expensive multi-dimensional DP matrix lookups, we use a greedy forward 
     *    scan coupled with an immediate reverse alignment contraction pass:
     *      - Forward Pass: Advance an index cursor `idx1` through `s1` trying to match `s2` 
     *        sequentially via index `idx2`. Loop until `idx2` reaches the end of `s2`.
     *      - Reverse Pass: Once a valid subsequence window is found, we freeze `idx1` at its 
     *        terminal match point. We then step both cursors backward from the end of `s2` to 
     *        the beginning. This reverse traversal pinpoints the absolute latest possible 
     *        starting position in `s1` that can fulfill this specific match sequence, 
     *        optimally shrinking the window size.
     *      - Reset: Record the window length, then reset `idx2 = 0` and position `idx1` 
     *        just one step past our discovered start point to scan for subsequent alternatives.
     */
    string minWindow(string s1, string s2) {
        size_t len1 = s1.length();
        size_t len2 = s2.length();
        
        // Edge boundary constraints validation
        if (len1 < len2 || len1 == 0 || len2 == 0) {
            return "";
        }

        size_t idx1 = 0, idx2 = 0;
        size_t minWindowLength = len1 + 1;
        int targetStartPoint = -1;

        // Step 2: Forward scanning matrix loop
        while (idx1 < len1) {
            if (s1[idx1] == s2[idx2]) {
                idx2++;
                
                // A valid match sequence has been completed
                if (idx2 == len2) {
                    size_t endWindow = idx1;
                    idx2--; // Position cursor back onto the final matched character of s2
                    
                    // Reverse contraction pass to find the optimal starting position
                    while (idx2 < len2) { // Relying on underflow break boundary control inside the block
                        if (s1[idx1] == s2[idx2]) {
                            if (idx2 == 0) break; // Start position reached safely
                            idx2--;
                        }
                        idx1--;
                    }
                    
                    // Calculate current window attributes
                    size_t currentWindowLen = endWindow - idx1 + 1;
                    if (currentWindowLen < minWindowLength) {
                        minWindowLength = currentWindowLen;
                        targetStartPoint = static_cast<int>(idx1);
                    }
                    
                    // Reset pointer layout trackers to discover alternative paths downstream
                    idx1++; 
                    idx2 = 0;
                }
            }
            idx1++;
        }

        return (targetStartPoint == -1) ? "" : s1.substr(static_cast<size_t>(targetStartPoint), minWindowLength);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M) Worst-Case / O(N) Average-Case - Where N represents the character length 
 *                  of s1, and M represents the length of s2. In worst-case highly redundant structures 
 *                  (e.g., s1="aaaaa...", s2="aaa"), the backward alignment pass can execute repeatedly. 
 *                  However, typical stream checks evaluate in near-linear time execution states.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sliding alignment cursors perform all evaluations directly 
 *                   in place on the stack without allocating supplementary lookup grids.
 */
int main() {
    string containerStr, sequencePattern;
    cout << "=== Minimum Window Subsequence Two-Pointer Engine ===\n";
    cout << "Enter the container search string (s1): ";
    if (!(cin >> containerStr)) return 1;
    cout << "Enter the sequential target pattern string (s2): ";
    if (!(cin >> sequencePattern)) return 1;

    Solution solver;
    cout << "\nLaunching forward greedy sweeps and reverse contraction arrays..." << endl;
    string computedWindow = solver.minWindow(containerStr, sequencePattern);

    if (!computedWindow.empty()) {
        cout << "\nOptimal Continuous Substring Minimum Window Found:\n\"" << computedWindow << "\"\n";
        cout << "Total Subsequence Width: " << computedWindow.length() << " characters.\n";
    } else {
        cout << "\nOutcome: No valid sequential matching subsequence windows could be resolved.\n";
    }

    return 0;
}