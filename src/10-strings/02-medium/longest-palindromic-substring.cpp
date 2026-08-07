#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * EXPAND AROUND CENTER HELPER METHOD
     * Explores outward from a given midpoint axis center to determine the boundary limits 
     * of the longest valid symmetrical palindromic window segment.
     * 
     * Returns the exact length width of the discovered palindrome.
     */
    int expandAroundCenter(const string& s, int left, int right) {
        // Expand outward as long as conditions match and string boundaries are respected
        while (left >= 0 && right < static_cast<int>(s.length()) && s[static_cast<size_t>(left)] == s[static_cast<size_t>(right)]) {
            left--;
            right++;
        }
        // Returns length width size: (right - 1) - (left + 1) + 1 = right - left - 1
        return right - left - 1;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: EXPAND AROUND CENTER STRATEGY
     * -------------------------------------------------------------------------------------
     * The objective is to identify the longest contiguous palindromic substring within a 
     * given string `s`.
     * 
     * 1. THE ARCHITECTURAL TRADEOFF (DP MATRIX VS. CENTER EXPANSION):
     *    - DP Matrix Approach: Standard Dynamic Programming uses an $O(N^2)$ space grid 
     *      where `dp[i][j]` tracks if `s[i...j]` is a palindrome. This creates massive heap 
     *      memory footprints and poor cache locality for large strings.
     *    - Expand Around Center (Chosen): A palindrome is symmetric around its center. 
     *      Since a string has $2N - 1$ potential centers (N single-character centers and 
     *      N-1 midpoints between characters), we can treat each as a focal point and expand 
     *      outward. This achieves the same $O(N^2)$ time efficiency as DP, but drops the auxiliary 
     *      space complexity to a perfect $O(1)$.
     * 
     * 2. THE DUAL-CENTER MAPPING PROFILE:
     *    For every index position `i` from 0 to $N-1$:
     *      - Case A (Odd-Length Palindromes): Center is anchored on a single character `expandAroundCenter(s, i, i)`.
     *      - Case B (Even-Length Palindromes): Center lies between two adjacent characters `expandAroundCenter(s, i, i + 1)`.
     * 
     * 3. GLOBAL TRACKING AND BOUNDARY SLICING:
     *    We track the absolute longest window size found across all centers. If a new center 
     *    yields a length greater than `maxLength`, we update our coordinate trackers:
     *    `startIdx = i - (len - 1) / 2`.
     */
    string longestPalindrome(string s) {
        if (s.empty()) return "";
        
        int n = static_cast<int>(s.length());
        int startIdx = 0;
        int maxLength = 0;

        // Step 2: Iterate across all 2N - 1 potential alignment center configurations
        for (int i = 0; i < n; ++i) {
            // Check for odd-length palindromes (e.g., "aba")
            int len1 = expandAroundCenter(s, i, i);
            // Check for even-length palindromes (e.g., "abba")
            int len2 = expandAroundCenter(s, i, i + 1);
            
            int activeMaxLen = max(len1, len2);

            // Step 3: If a longer palindrome is found, update the boundary trackers
            if (activeMaxLen > maxLength) {
                maxLength = activeMaxLen;
                // Calculate the start index location based on current midpoint metrics
                startIdx = i - (activeMaxLen - 1) / 2;
            }
        }

        return s.substr(static_cast<size_t>(startIdx), static_cast<size_t>(maxLength));
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2) - Where N represents the total character length of string s. The main 
 *                  loop runs N times, and each center expansion can take up to O(N) steps in the 
 *                  worst-case scenario (e.g., a string consisting of identical characters like "aaaaa").
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The expansions are calculated entirely in place using index 
 *                   pointers on the original string, avoiding the allocation of temporary tables.
 */
int main() {
    string inputStr;
    cout << "=== Longest Palindromic Substring Center Expansion Console ===\n";
    cout << "Enter the string to evaluate:\n";
    if (!(cin >> inputStr)) return 1;

    Solution solver;
    cout << "\nScanning focal centers and expanding bidirectional match paths..." << endl;
    string computedPalindrome = solver.longestPalindrome(inputStr);

    cout << "\nLongest palindromic contiguous substring isolated: \"" << computedPalindrome << "\"\n";
    cout << "Total Palindrome String Sequence Width: " << computedPalindrome.length() << " characters.\n";

    return 0;
}