#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: KMP FAILURE FUNCTION PREFIX-SUFFIX VERIFICATION
     * -------------------------------------------------------------------------------------
     * The objective is to convert a string `s` into a palindrome by adding characters 
     * at the front. To do this with the minimum number of additions, we must identify 
     * the longest palindromic prefix already present inside `s`.
     * 
     * 1. THE STRATEGIC KMP PATTERN MATCH MAPPING:
     *    Finding the longest palindromic prefix in a string `s` is equivalent to finding 
     *    the longest matching proper prefix of `s` that is also a proper suffix of its 
     *    reversed variant `rev_s`.
     *    We construct a combined dynamic execution string: `combined = s + '#' + rev_s`.
     *    The delimiter '#' is critical because it acts as a sentinel barrier, preventing 
     *    the prefix matching values from spilling across the individual string structures 
     *    if the length of `s` matches perfectly.
     * 
     * 2. KMP FAILURE FUNCTION (LPS TABLE) GENERATION:
     *    We compute the Longest Prefix Suffix (LPS) array for the `combined` string layout.
     *    `lps[i]` tracks the length of the longest proper prefix of `combined[0...i]` 
     *    that is also a proper suffix of that same segment.
     *    The value stored at the very last index of our LPS table (`lps.back()`) represents 
     *    the exact character width of the longest palindromic prefix inside `s`.
     * 
     * 3. STRUCTURAL SUFFIX REFLECTION:
     *    Once we know the length of the longest palindromic prefix (`palPrefixLen`), the 
     *    remaining portion at the end of the string (`s.substr(palPrefixLen)`) is the unmatched 
     *    suffix. We reverse this unmatched suffix and prepend it directly to `s` to complete 
     *    the shortest possible palindrome structure.
     */
    string shortestPalindrome(string s) {
        if (s.empty()) return "";

        // Step 1: Generate the reversed string configuration
        string rev_s = s;
        reverse(rev_s.begin(), rev_s.end());

        // Construct the combined token layout string with a safe sentinel separator
        string combined = s + '#' + rev_s;
        size_t totalLen = combined.length();

        // Step 2: Build the KMP Longest Prefix Suffix (LPS) failure function array
        vector<int> lps(totalLen, 0);
        
        for (size_t i = 1; i < totalLen; ++i) {
            int j = lps[i - 1];
            
            // Backtrack if a mismatch happens, using historical prefix overlaps
            while (j > 0 && combined[i] != combined[static_cast<size_t>(j)]) {
                j = lps[static_cast<size_t>(j - 1)];
            }
            
            // If elements match, increment the matching pattern indicator length
            if (combined[i] == combined[static_cast<size_t>(j)]) {
                j++;
            }
            
            lps[i] = j;
        }

        // The last element in the LPS table indicates the size of the longest palindromic prefix
        int palPrefixLen = lps.back();

        // Step 3: Extract the non-palindromic suffix, invert it, and build the final structural sequence
        string nonPalindromicSuffix = s.substr(static_cast<size_t>(palPrefixLen));
        string prefixAddition = nonPalindromicSuffix;
        reverse(prefixAddition.begin(), prefixAddition.end());

        // FIXED: Fully qualified std::move call to optimize memory allocation operations
        return prefixAddition + std::move(s);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the original string s. 
 *                  Building the combined string takes linear time, and the KMP failure function loop 
 *                  runs in linear time because the pointer `j` increments at most once per iteration 
 *                  and undergoes bounded decrements.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The KMP failure table grid size scales linearly with the length 
 *                   of the combined string layout (exactly 2N + 1 elements).
 */
int main() {
    string sourceInput;
    cout << "=== Shortest Palindrome KMP Failure Function Console ===\n";
    cout << "Enter a string sequence to process:\n";
    if (!(cin >> sourceInput)) return 1;

    Solution solver;
    cout << "\nBuilding string combinations and processing KMP prefix-suffix tables..." << endl;
    string completedPalindrome = solver.shortestPalindrome(sourceInput);

    cout << "\nShortest Valid Palindrome Achieved:\n\"" << completedPalindrome << "\"\n";
    cout << "Total Character Sequence Width: " << completedPalindrome.length() << " characters.\n";

    return 0;
}