#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DYNAMIC SLIDING WINDOW WITH CHARACTER-INDEX HASH MAPS
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the length of the longest substring without repeating 
     * characters in a strict linear time and constant space constraint profile.
     * 
     * 1. FIXED-CAPACITY INTENSITY PROFILES:
     *    Instead of deploying heavy, dynamic heap-allocated hash frameworks like 
     *    `std::unordered_map`, we initialize a fixed-size vector tracker array of size 128 
     *    (`lastSeenIndex`). This covers the entire standard ASCII character mapping sequence 
     *    in true O(1) space. We fill it with `-1` to represent that no character has been visited yet.
     * 
     * 2. AUTOMATED POINTER JUMPING CONTRACT:
     *    We iterate through the string with a sliding window bounded by pointers `left` and `right`.
     *    - The `right` pointer moves forward step-by-step, scanning the current character `c`.
     *    - If `c` was already encountered inside our current window (`lastSeenIndex[c] >= left`), 
     *      a duplicate is detected. Instead of shrinking the window pixel-by-pixel (`left++`), we 
     *      instantly teleport the `left` boundary to `lastSeenIndex[c] + 1`. This safely bypasses 
     *      the duplicate character in a single operation.
     * 
     * 3. RUNTIME METRIC RECORDING:
     *    At each step, the length of the current non-repeating window is given by `right - left + 1`. 
     *    We update our global maximum tracker `maxLength`, then update the character's last seen 
     *    position to the current index `right`.
     */
    int lengthOfLongestSubstring(string s) {
        // Step 1: Initialize fixed 128-element ASCII registry tracking arrays with -1
        vector<int> lastSeenIndex(128, -1);
        
        int n = static_cast<int>(s.length());
        int maxLength = 0;
        int left = 0;

        // Step 2 & 3: Run the dynamic sliding window loop
        for (int right = 0; right < n; ++right) {
            char currentChar = s[static_cast<size_t>(right)];

            // If the character is a duplicate inside our current window, jump the left pointer forward
            if (lastSeenIndex[static_cast<size_t>(currentChar)] >= left) {
                left = lastSeenIndex[static_cast<size_t>(currentChar)] + 1;
            }

            // Record the maximum window size achieved so far
            maxLength = max(maxLength, right - left + 1);

            // Update the index position log entry for the current character
            lastSeenIndex[static_cast<size_t>(currentChar)] = right;
        }

        return maxLength;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string s. The single-pass 
 *                  loop parses each character index exactly once. Because the left pointer jumps 
 *                  directly to its target instead of sliding incrementally, operations complete 
 *                  in strict linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The lookup array allocation is locked at a fixed capacity of 128 integers, 
 *                   remaining completely independent of the length of the input text string.
 */
int main() {
    string inputString;
    cout << "=== Longest Substring Without Repeating Characters Console ===\n";
    cout << "Enter the string to evaluate (Standard ASCII characters sequence):\n";
    if (!getline(cin, inputString)) return 1;

    Solution solver;
    cout << "\nLaunching character-index tracking maps and dynamic sliding windows..." << endl;
    int isolatedMaxLength = solver.lengthOfLongestSubstring(inputString);

    cout << "\nLength of the longest contiguous substring without duplicates: " << isolatedMaxLength << " characters.\n";

    return 0;
}