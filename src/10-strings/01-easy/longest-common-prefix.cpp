#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: VERTICAL CHARACTER SCANNING
     * -------------------------------------------------------------------------------------
     * The objective is to find the longest common prefix string amongst an array of strings.
     * 
     * 1. THE ALGORITHMIC ARCHITECTURE TRADEOFF:
     *    - Horizontal Scanning: Compares string 1 with string 2 to get a prefix, then compares 
     *      that prefix with string 3, and so on. If the last string is very short or completely 
     *      different, we waste significant operations scanning long words early on.
     *    - Vertical Scanning (Chosen): Inspects characters column by column across all strings 
     *      simultaneously. We check index 0 of every string, then index 1, index 2, etc. 
     *      This is highly efficient because it terminates immediately the moment a mismatch 
     *      occurs or the shortest string boundary is hit, minimizing unnecessary inspections.
     * 
     * 2. HORIZONTAL TERMINATION CONDITIONAL RULES:
     *    - We pick the first string `strs[0]` as our character blueprint template.
     *    - We iterate through each column index `i` of this template string.
     *    - For each column, we check every other string `j` in the array:
     *        * If the current index `i` matches the length of `strs[j]`, it means string `j` has 
     *          exhausted its characters. We have reached the absolute limit of the prefix.
     *        * If the character `strs[j][i]` does not match the template `strs[0][i]`, a mismatch 
     *          is confirmed.
     *    - In either termination scenario, we immediately slice and return the template string 
     *      from index 0 up to `i`.
     * 
     * 3. EMPTY BASE BOUNDARY CONDITION:
     *    If the input vector is completely empty, there is no common matrix to evaluate. We return 
     *    an empty string early.
     */
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) {
            return "";
        }

        // Step 2: Iterate vertically column by column using the first string as a guide template
        for (size_t i = 0; i < strs[0].length(); ++i) {
            char targetChar = strs[0][i];

            // Inspect the character at column index 'i' across all subsequent strings
            for (size_t j = 1; j < strs.size(); ++j) {
                // Terminate if the current string boundary is exceeded or a character mismatch occurs
                if (i == strs[j].length() || strs[j][i] != targetChar) {
                    return strs[0].substr(0, i);
                }
            }
        }

        // Base Fallback: The first string itself is entirely the common prefix matrix
        return strs[0];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(S) - Where S represents the sum of all characters across all strings in the array. 
 *                  In the worst-case scenario where all strings are completely identical, the algorithm 
 *                  scans every character. In best-to-average cases, it terminates much earlier after 
 *                  evaluating a small fraction of characters.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The scan evaluates string elements entirely in place using index 
 *                   pointers without allocating dynamic secondary string buffers.
 */
int main() {
    int totalStringsCount;
    cout << "Enter the total number of strings to evaluate: ";
    if (!(cin >> totalStringsCount) || totalStringsCount < 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    if (totalStringsCount == 0) {
        cout << "Longest common prefix on empty set: \"\"" << endl;
        return 0;
    }

    vector<string> strs(static_cast<size_t>(totalStringsCount));
    cout << "Enter the strings consecutively:\n";
    for (int i = 0; i < totalStringsCount; ++i) {
        cout << "String [" << i << "]: ";
        cin >> strs[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nLaunching vertical character matrix scanning..." << endl;
    string prefixOutcome = solver.longestCommonPrefix(strs);

    if (prefixOutcome.empty()) {
        cout << "Outcome: No shared common prefix identified among the configured strings. (Result = \"\")" << endl;
    } else {
        cout << "Longest common prefix segment isolated: \"" << prefixOutcome << "\"" << endl;
        cout << "Prefix String Sequence Width: " << prefixOutcome.length() << " characters." << endl;
    }

    return 0;
}