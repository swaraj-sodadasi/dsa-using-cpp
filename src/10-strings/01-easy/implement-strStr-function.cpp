#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
private:
    /*
     * KMP LPs (LONGEST PREFIX SUFFIX) TABLE GENERATOR
     * Pre-calculates the fallback index table for the pattern string (needle).
     * 
     * The `lps` array tracks the length of the longest proper prefix that is also 
     * a suffix for the substring `needle[0...i]`. This enables the search engine 
     * to skip redundant comparisons when a character mismatch occurs.
     */
    vector<int> computeLpsTable(const string& needle) {
        int m = static_cast<int>(needle.length());
        vector<int> lps(static_cast<size_t>(m), 0);
        
        int len = 0; // Length of the previous longest prefix suffix
        int i = 1;

        while (i < m) {
            if (needle[static_cast<size_t>(i)] == needle[static_cast<size_t>(len)]) {
                len++;
                lps[static_cast<size_t>(i)] = len;
                i++;
            } else {
                if (len != 0) {
                    // Fall back to the previous longest prefix suffix path
                    len = lps[static_cast<size_t>(len - 1)];
                } else {
                    // No prefix matching possible, reset tracking index to 0
                    lps[static_cast<size_t>(i)] = 0;
                    i++;
                }
            }
        }
        return lps;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: KMP PATTERN MATCHING ALGORITHM
     * -------------------------------------------------------------------------------------
     * The objective is to return the starting index of the first occurrence of `needle` 
     * inside `haystack`, or -1 if `needle` is not part of `haystack`.
     * 
     * 1. THE NAIVE SEARCH DRAWBACK:
     *    A standard brute-force search checks every alignment candidate. When a mismatch 
     *    occurs, it resets the search pointers completely, leading to a costly worst-case 
     *    runtime performance of $O(N \times M)$.
     * 
     * 2. THE KMP STRUCTURAL ADVANTAGE:
     *    The Knuth-Morris-Pratt (KMP) algorithm bypasses redundant checks by analyzing 
     *    the internal structure of the `needle` before scanning. It builds a Longest Prefix 
     *    Suffix (`lps`) table. 
     *    When a mismatch happens after matching several characters, the algorithm uses the 
     *    `lps` table to determine the next valid alignment, allowing the text search pointer 
     *    to advance continuously without back-tracking.
     * 
     * 3. MATCH TRACKING PIPELINE:
     *    We maintain two index counters: `i` for `haystack` and `j` for `needle`.
     *      - If characters match (`haystack[i] == needle[j]`), both pointers advance (`i++`, `j++`).
     *      - If `j` reaches `needle.length()`, a complete match is confirmed at index `i - j`.
     *      - If a mismatch occurs after some matches:
     *          * If `j != 0`, we shift `j` back using `lps[j - 1]`.
     *          * If `j == 0`, we simply advance `i++`.
     */
    int strStr(string haystack, string needle) {
        if (needle.empty()) {
            return 0; // Boundary Rule: Empty pattern template returns 0 implicitly
        }

        int n = static_cast<int>(haystack.length());
        int m = static_cast<int>(needle.length());

        if (n < m) {
            return -1; // Dimension Pruning: Pattern is longer than search text
        }

        // Step 2: Build the KMP shift fallback index table
        vector<int> lps = computeLpsTable(needle);

        int i = 0; // Text tracking marker index loop
        int j = 0; // Pattern tracking marker index loop

        while (i < n) {
            if (haystack[static_cast<size_t>(i)] == needle[static_cast<size_t>(j)]) {
                i++;
                j++;
            }

            if (j == m) {
                // Match confirmed: return starting offset location coordinates
                return i - j;
            } 
            // Handle character mismatch scenarios safely
            else if (i < n && haystack[static_cast<size_t>(i)] != needle[static_cast<size_t>(j)]) {
                if (j != 0) {
                    // Shift pattern back using the precomputed fallback rules
                    j = lps[static_cast<size_t>(j - 1)];
                } else {
                    // Advance text index directly when no partial match exists
                    i++;
                }
            }
        }

        return -1; // Fallback: No matching sequence patterns isolated
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N represents the length of haystack, and M represents the length of 
 *                  needle. Pre-processing the pattern string takes O(M) steps, and the main text scan 
 *                  completes in O(N) operations because the text pointer `i` never rolls backward.
 *
 * SPACE COMPLEXITY: O(M) Auxiliary - The system allocates space for the `lps` array proportional 
 *                   to the length of the pattern string `needle`.
 */
int main() {
    string haystack, needle;
    cout << "=== KMP Substring Linear Pattern Matcher Console ===\n";
    cout << "Enter the primary body text string (haystack): ";
    if (!getline(cin, haystack)) return 1;
    cout << "Enter the target search pattern string (needle): ";
    if (!getline(cin, needle)) return 1;

    Solution solver;
    cout << "\nExecuting KMP pattern pre-processing and text scanning..." << endl;
    int matchingIndexOffset = solver.strStr(haystack, needle);

    if (matchingIndexOffset != -1) {
        cout << "Pattern discovered! First matching segment found at index offset: " << matchingIndexOffset << endl;
    } else {
        cout << "Outcome: Substring pattern not found inside text matrix. (Result = -1)" << endl;
    }

    return 0;
}