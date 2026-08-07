#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SPACE-DELIMITED SEGMENT ROTATION (IN-PLACE TWO-POINTER SWAPPING)
     * -------------------------------------------------------------------------------------
     * The objective is to reverse the order of characters in each word within a sentence 
     * while preserving whitespace delimiters and the original word order.
     * 
     * 1. IN-PLACE STRING MANIPULATION STRADLE:
     *    A naive approach might allocate new string streams or tokenize words into an auxiliary vector,
     *    reversing them individually before stitching them back together. While simple, this creates 
     *    unnecessary heap allocations ($O(N)$ auxiliary space overhead). To achieve maximum efficiency, 
     *    we perform the reversal directly inside the input string using local boundaries.
     * 
     * 2. DYNAMIC SEGMENT BOUNDARY DETECTOR:
     *    We use a single pointer `i` to scan the string from left to right. We locate individual words 
     *    by finding their boundaries:
     *      - Word Start: The pointer anchors at index `i` when it hits a non-space character.
     *      - Word End: We use a secondary look-ahead pointer `j` that advances until it hits a space 
     *        character (`' '`) or reaches the absolute end of the string (`s.length()`).
     * 
     * 3. IN-PLACE REVERSAL LOOP:
     *    Once a word segment is isolated between indices `i` and `j - 1`, we run a nested two-pointer swap loop 
     *    (`left = i`, `right = j - 1`) to flip the characters of that specific word in place. 
     *    After reversing, we jump `i` forward to the position of `j` to search for the next word.
     */
    string reverseWords(string s) {
        int n = static_cast<int>(s.length());
        int i = 0;

        // Step 2: Linear traversal to isolate and process individual word boundaries
        while (i < n) {
            // Skip space characters if any leading anomalies exist
            if (s[static_cast<size_t>(i)] == ' ') {
                i++;
                continue;
            }

            // Move pointer j forward to find the end boundary of the current word
            int j = i;
            while (j < n && s[static_cast<size_t>(j)] != ' ') {
                j++;
            }

            // Step 3: Execute in-place character swap within the word block boundaries [i ... j-1]
            int left = i;
            int right = j - 1;
            while (left < right) {
                swap(s[static_cast<size_t>(left)], s[static_cast<size_t>(right)]);
                left++;
                right--;
            }

            // Jump pointer i directly to the next space/character position past the current word
            i = j;
        }

        return s;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of string s. Although it contains 
 *                  nested loops, the pointers `i`, `j`, `left`, and `right` process each character index 
 *                  at most twice (once to find boundaries and once to swap), maintaining linear performance.
 *
 * SPACE COMPENSIVITY: O(1) Auxiliary - The swap adjustments run in place directly within the string buffer 
 *                     without allocating extra heap containers.
 */
int main() {
    string inputSentence;
    cout << "=== Reverse Words in a String III Console ===\n";
    cout << "Enter a sentence with space-separated words:\n";
    if (!getline(cin, inputSentence)) return 1;

    Solution solver;
    cout << "\nIsolating token segments and executing in-place reversals..." << endl;
    string transformedResult = solver.reverseWords(inputSentence);

    cout << "\nTransformed Output Result:\n\"" << transformedResult << "\"\n";

    return 0;
}