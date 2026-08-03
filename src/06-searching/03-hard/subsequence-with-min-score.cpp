#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * TWO-POINTER PREFIX/SUFFIX DELETION SEARCH ENGINE
     * ------------------------------------------------
     * The objective is to find the minimum score to make string `t` a subsequence of `s` 
     * by deleting a contiguous substring of `t` from index `left` to `right`.
     * The score of this deletion is defined as `right - left + 1`. If no characters are 
     * deleted, the score is 0.
     *
     * Algorithmic Strategy:
     * - Deleting a contiguous block of `t` leaves us with two segments: a prefix `t[0...i]` 
     *   and a suffix `t[j...n-1]`. The missing middle slice represents the deleted window.
     * - We can determine whether a combined prefix and suffix can form a valid subsequence 
     *   within `s` using precomputed positional matching records:
     *     1. `leftmost[k]`: The smallest index in `s` required to fully match the prefix `t[0...k]`.
     *     2. `rightmost[k]`: The largest index in `s` required to fully match the suffix `t[k...n-1]`.
     *
     * Two-Pointer Optimization Sequence:
     * - Step 1: Precompute the `leftmost` matching array by scanning forward through `s` 
     *   and greedily matching characters of `t`. If `t[0...k]` cannot be formed, we mark it as -1.
     * - Step 2: Precompute the `rightmost` matching array by scanning backward through `s` 
     *   and greedily matching the tail of `t`. 
     * - Step 3: Initialize the baseline answer assuming we delete the entire string or everything 
     *   except the valid suffix component.
     * - Step 4: Use a sliding two-pointer approach to check all valid combinations of prefix and suffix. 
     *   For each prefix length ending at `i`, we increment the suffix pointer `j` until the condition 
     *   `leftmost[i] < rightmost[j]` is satisfied (ensuring no structural collision or overlap inside `s`).
     * - The score for this configuration is `j - i - 1` (the number of deleted characters between them). 
     *   We minimize this score across all potential boundary splits.
     */
    int minimumScore(string s, string t) {
        int sLen = static_cast<int>(s.length());
        int tLen = static_cast<int>(t.length());

        // leftmost[k] stores the earliest index in 's' that covers prefix t[0...k]
        vector<int> leftmost(static_cast<size_t>(tLen), -1);
        // rightmost[k] stores the latest index in 's' that covers suffix t[k...tLen-1]
        vector<int> rightmost(static_cast<size_t>(tLen), -1);

        // Populate the forward prefix matching boundaries
        int tIdx = 0;
        for (int sIdx = 0; sIdx < sLen; ++sIdx) {
            if (s[static_cast<size_t>(sIdx)] == t[static_cast<size_t>(tIdx)]) {
                leftmost[static_cast<size_t>(tIdx)] = sIdx;
                tIdx++;
                if (tIdx == tLen) break;
            }
        }

        // Populate the reverse suffix matching boundaries
        tIdx = tLen - 1;
        for (int sIdx = sLen - 1; sIdx >= 0; --sIdx) {
            if (s[static_cast<size_t>(sIdx)] == t[static_cast<size_t>(tIdx)]) {
                rightmost[static_cast<size_t>(tIdx)] = sIdx;
                tIdx--;
                if (tIdx == -1) break;
            }
        }

        // Baseline Edge Case: Assume we completely delete a prefix chunk to match the suffix.
        // We find the first index where t[tIdx...] becomes a valid subsequence of s.
        int minScore = tLen;
        for (int k = 0; k < tLen; ++k) {
            if (rightmost[static_cast<size_t>(k)] != -1) {
                minScore = k; // Score corresponds to deleting everything before index k
                break;
            }
        }

        // If the entire string t is already a subsequence, no deletions are required
        if (tIdx == -1) return 0;

        // Sliding window two-pointer scan over the calculated bounds
        int suffixPtr = 0;
        for (int prefixPtr = 0; prefixPtr < tLen; ++prefixPtr) {
            if (leftmost[static_cast<size_t>(prefixPtr)] == -1) {
                break; // This prefix cannot be formed, so larger prefixes are also impossible
            }

            // Adjust the suffix pointer until it sits past the current prefix boundary in 's'
            while (suffixPtr < tLen && (rightmost[static_cast<size_t>(suffixPtr)] == -1 || 
                   leftmost[static_cast<size_t>(prefixPtr)] >= rightmost[static_cast<size_t>(suffixPtr)])) {
                suffixPtr++;
            }

            // If a valid prefix/suffix combination is found, compute the score
            if (suffixPtr < tLen) {
                if (prefixPtr >= suffixPtr) {
                    return 0; // The segments overlap or touch, meaning the full string is covered
                }
                minScore = min(minScore, suffixPtr - prefixPtr - 1);
            }
        }

        return minScore;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N is the length of string s and M is the length of string t. 
 *                  Building the arrays takes linear sweeps across both strings. The final 
 *                  two-pointer alignment passes through the prefix and suffix arrays at most once.
 *
 * SPACE COMPLEXITY: O(M) Auxiliary - The optimization strategy requires two indexing vectors 
 *                   (`leftmost` and `rightmost`) proportional to the size of string t.
 */
int main() {
    string s, t;
    cout << "Enter the source context string (s): ";
    if (!(cin >> s)) return 1;

    cout << "Enter the target subsequence string (t): ";
    if (!(cin >> t)) return 1;

    Solution solver;
    cout << "\nExecuting two-pointer prefix/suffix deletion search sweep..." << endl;
    int resultingScore = solver.minimumScore(s, t);

    cout << "Minimum possible substring deletion score: " << resultingScore << endl;

    return 0;
}