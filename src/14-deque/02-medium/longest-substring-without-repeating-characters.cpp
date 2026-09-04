#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS (SLIDING WINDOW)
     * -------------------------------------------------------------------------------------
     * The objective is to find the length of the longest substring without duplicate characters.
     * 
     * 1. OPTIMIZED SLIDING WINDOW WITH DIRECT INDEX MAP:
     *    We maintain a expanding window `[left, right]`.
     *    Instead of incrementally shrinking `left` step-by-step using a deque or hash set, 
     *    we store the last seen 0-based index of each ASCII character in an array `lastSeen`.
     * 
     * 2. INSTANT BOUNDARY SHIFT:
     *    When character `s[right]` was previously seen at index `prevIndex`:
     *      - If `prevIndex >= left`, it means duplicate character exists within current window.
     *      - We jump `left` directly to `prevIndex + 1` in O(1) time.
     * 
     * 3. MAXIMUM LENGTH TRACKING:
     *    At each step, window length is `right - left + 1`. We update `maxLen` accordingly.
     */
    int lengthOfLongestSubstring(const string& s) {
        vector<int> lastSeen(256, -1); // Direct index tracking for ASCII characters
        int left = 0;
        int maxLen = 0;

        for (int right = 0; right < static_cast<int>(s.length()); ++right) {
            unsigned char currentChar = static_cast<unsigned char>(s[static_cast<size_t>(right)]);

            // If duplicate exists inside active sliding window, advance left boundary
            if (lastSeen[currentChar] >= left) {
                left = lastSeen[currentChar] + 1;
            }

            lastSeen[currentChar] = right;
            maxLen = max(maxLen, right - left + 1);
        }

        return maxLen;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of string s.
 *                  Single linear pass over characters.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Fixed-size lookup table of size 256 for ASCII.
 */
int main() {
    cout << "=== Longest Substring Without Repeating Characters Engine ===\n";
    cout << "Enter input string s: ";
    string s;
    if (!getline(cin, s)) return 1;

    Solution solver;
    int ans = solver.lengthOfLongestSubstring(s);

    cout << "Length of longest substring without repeating characters: [" << ans << "]\n";

    return 0;
}