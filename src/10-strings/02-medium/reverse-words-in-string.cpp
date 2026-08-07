#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: IN-PLACE WORD SEQUENCE VECTOR INVERSION
     * -------------------------------------------------------------------------------------
     * The objective is to reverse the ordering of words in a string while removing all 
     * leading, trailing, and redundant spaces, executing in strict O(1) auxiliary space.
     * 
     * 1. THE IN-PLACE COMPACTION & ROTATION TRADEOFF:
     *    A standard approach might split the string by spaces into an array of words, 
     *    reverse the array, and join them back together. However, allocating an auxiliary 
     *    array of strings violates our O(1) memory constraint and triggers heavy heap churn.
     *    Instead, we perform a dual-inversion strategy directly on the existing string memory.
     * 
     * 2. PHASE A: IN-PLACE SPACE FILTERING AND LOCAL WORD REVERSAL
     *    We use two pointers: a `read` pointer to scan the original string and a `write` 
     *    pointer to overwrite characters at the front of the string buffer.
     *      - We skip all leading and consecutive spaces.
     *      - When we find a word, we copy it to the `write` pointer. If it's not the first 
     *        word, we manually prepend a single space block.
     *      - Immediately after copying a word, we reverse that specific isolated word in-place 
     *        (e.g., "Hello" becomes "olleH").
     * 
     * 3. PHASE B: TRUNCATION AND GLOBAL INVERSION
     *    - Once the `read` pointer finishes, the `write` pointer indicates the exact length 
     *      of the newly compacted string. We use `s.resize(write)` to discard the leftover 
     *      garbage characters at the end of the buffer.
     *    - Finally, we reverse the entire compacted string matrix. Because the individual 
     *      words were already reversed in Phase A, reversing the whole string puts the words 
     *      into the correct reversed sequence while restoring the characters of each word to 
     *      their original readable orientation.
     */
    string reverseWords(string s) {
        int n = static_cast<int>(s.length());
        int writeIndex = 0;
        int readIndex = 0;

        // Phase A: Compact spaces and reverse each individual word locally
        while (readIndex < n) {
            // Skip redundant spaces in the read stream
            while (readIndex < n && s[static_cast<size_t>(readIndex)] == ' ') {
                readIndex++;
            }
            
            // If we reached the end of the string after trailing spaces, break early
            if (readIndex >= n) {
                break;
            }

            // Append a single space separator if this is not the first word processed
            if (writeIndex > 0) {
                s[static_cast<size_t>(writeIndex++)] = ' ';
            }

            // Track the starting boundary of the current word for local reversal
            int wordStartIndex = writeIndex;

            // Copy the valid word characters into the compacted contiguous buffer layout
            while (readIndex < n && s[static_cast<size_t>(readIndex)] != ' ') {
                s[static_cast<size_t>(writeIndex++)] = s[static_cast<size_t>(readIndex++)];
            }

            // Reverse the individual token segment in-place (e.g., "word" -> "drow")
            reverse(s.begin() + wordStartIndex, s.begin() + writeIndex);
        }

        // Phase B: Truncate the string to remove trailing matrix garbage from the old buffer
        s.resize(static_cast<size_t>(writeIndex));

        // Perform a global string inversion to flip the word order sequence correctly
        reverse(s.begin(), s.end());

        return s;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of string s. 
 *                  The `read` pointer touches each character once. The words are reversed 
 *                  locally and globally, meaning each character undergoes exactly two 
 *                  swap operations. This resolves to a strict linear runtime profile.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The compaction and dual-reversals execute exclusively 
 *                   within the memory bounds of the original input string, completely bypassing 
 *                   dynamic vector or string allocations.
 */
int main() {
    string inputSentence;
    cout << "=== Reverse Words in a String (In-place Vector Inversion) Console ===\n";
    cout << "Enter a sentence sequence (can include leading, trailing, or multiple spaces):\n";
    if (!getline(cin, inputSentence)) return 1;

    Solution solver;
    cout << "\nExecuting in-place buffer compactions and dual-inversion rotations..." << endl;
    string formattedOutcome = solver.reverseWords(inputSentence);

    cout << "\nReversed Word Sequence Output Matrix:\n\"" << formattedOutcome << "\"\n";

    return 0;
}