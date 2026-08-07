#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: TRAILING SPACE-AWARE REVERSE TRAVERSAL
     * -------------------------------------------------------------------------------------
     * The objective is to return the length of the last word in a string, consisting of 
     * non-space characters only.
     * 
     * 1. THE REVERSE TRAVERSAL TRADEOFF:
     *    A naive approach might split the string by spaces or parse it forward from the beginning.
     *    However, forward parsing wastes time processing long paragraphs when we only care about 
     *    the final word. By traversing backwards from the end of the string, we find our answer 
     *    almost instantly.
     * 
     * 2. TWO-PHASE REVERSE SCAN PROFILE:
     *    - Phase A (Skipping Trailing Garbage): The string may end with arbitrary whitespace 
     *      (e.g., "Hello World   "). We start a pointer at `s.length() - 1` and decrement it 
     *      as long as the characters are spaces (`' '`), cleaning the trailing padding.
     *    - Phase B (Word Width Accumulation): As soon as we hit the first non-space character, 
     *      we enter the target word block. We continue moving the pointer backwards, incrementing 
     *      our `wordLength` counter for every character until we either hit another space or reach 
     *      the beginning of the string (`index < 0`).
     * 
     * 3. OPTIMAL CONSTANT SPACE BOUNDS:
     *    By tracking positions using index counters directly on the source string, we achieve a 
     *    perfect O(1) auxiliary space footprint, avoiding any tokenizing allocations or string splits.
     */
    int lengthOfLastWord(string s) {
        int index = static_cast<int>(s.length()) - 1;
        int wordLength = 0;

        // Phase A: Skip all trailing whitespace padding from the right boundary
        while (index >= 0 && s[static_cast<size_t>(index)] == ' ') {
            index--;
        }

        // Phase B: Count characters of the final isolated word segment
        while (index >= 0 && s[static_cast<size_t>(index)] != ' ') {
            wordLength++;
            index--;
        }

        return wordLength;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string s. 
 *                  In the worst-case scenario (e.g., no trailing spaces and a single long word), 
 *                  the algorithm visits each character once. In average scenarios, it only evaluates 
 *                  the very end of the string, completing in sub-linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The reverse traversal happens entirely in place on the original 
 *                   string buffer using simple primitive loop counters.
 */
int main() {
    string inputPhrase;
    cout << "=== Length of Last Word Reverse Traversal Console ===\n";
    cout << "Enter a text phrase or sentence containing trailing spaces if desired:\n";
    if (!getline(cin, inputPhrase)) return 1;

    Solution solver;
    cout << "\nLaunching trailing space filters and reverse character sweeps..." << endl;
    int computedLength = solver.lengthOfLastWord(inputPhrase);

    cout << "Isolated final word character count length segment: " << computedLength << "\n";

    return 0;
}