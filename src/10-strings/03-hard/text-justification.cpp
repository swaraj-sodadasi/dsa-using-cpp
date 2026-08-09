#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: GREEDY LINE CONSTRUCTION WITH DISTRIBUTED SPACE LAYOUTS
     * -------------------------------------------------------------------------------------
     * The objective is to format a given set of words such that each line has exactly 
     * `maxWidth` characters and is fully (left and right) justified.
     * 
     * 1. GREEDY WINDOW SELECTION:
     *    We pack as many words as possible into each line without exceeding `maxWidth`. 
     *    For a sequence of words, the minimum length required is the sum of the word lengths 
     *    plus at least one space between each word.
     * 
     * 2. SPACE DISTRIBUTION MECHANICS:
     *    Once we determine the words belonging to a single line (from index `i` to `j - 1`), 
     *    we calculate the total spaces needed: `totalSpaces = maxWidth - wordsLength`.
     *    - Case A (Single Word Line or Last Line): Left-justify the text. Each word is 
     *      separated by a single space, and any remaining padding spaces are appended at 
     *      the very end of the line.
     *    - Case B (Standard Multi-Word Line): Fully justify the text. Spaces are distributed 
     *      as evenly as possible between words. The slots between words equal `numWords - 1`. 
     *      Each slot receives a base allocation of `totalSpaces / slots`, and the remainder 
     *      `totalSpaces % slots` is distributed one by one to the leftmost slots.
     */
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> result;
        int n = static_cast<int>(words.size());
        int i = 0;

        while (i < n) {
            int wordsLength = static_cast<int>(words[static_cast<size_t>(i)].length());
            int j = i + 1;
            
            // Greedily find all words that fit into the current line bounds
            while (j < n && wordsLength + static_cast<int>(words[static_cast<size_t>(j)].length()) + (j - i) <= maxWidth) {
                wordsLength += static_cast<int>(words[static_cast<size_t>(j)].length());
                j++;
            }

            string currentLine = "";
            int numWords = j - i;
            int slots = numWords - 1;

            // Case A: If it's the last line or the line contains only a single word
            if (j == n || numWords == 1) {
                for (int k = i; k < j; ++k) {
                    currentLine += words[static_cast<size_t>(k)];
                    if (k < j - 1) {
                        currentLine += " ";
                    }
                }
                // Append remaining trailing spaces to fill maxWidth padding requirements
                currentLine.append(static_cast<size_t>(maxWidth - currentLine.length()), ' ');
            } 
            // Case B: Standard multi-word line requires full round-robin justification
            else {
                int totalSpaces = maxWidth - wordsLength;
                int baseSpaces = totalSpaces / slots;
                int extraSpaces = totalSpaces % slots;

                for (int k = i; k < j; ++k) {
                    currentLine += words[static_cast<size_t>(k)];
                    if (k < j - 1) {
                        int spacesToApply = baseSpaces + (static_cast<int>(k - i) < extraSpaces ? 1 : 0);
                        currentLine.append(static_cast<size_t>(spacesToApply), ' ');
                    }
                }
            }

            result.push_back(std::move(currentLine));
            i = j; // Advance pointer to the next line block starting index
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of characters across all words. 
 *                  Each word is iterated over and appended into the final line configurations exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Excluding the final string array required by the problem return type, 
 *                   formatting runs entirely in place via localized cursor variables.
 */
int main() {
    int maxLineWidth;
    int wordsCount;

    cout << "=== Text Justification Balanced Packing Engine ===\n";
    cout << "Enter the target maximum line width boundary (maxWidth): ";
    if (!(cin >> maxLineWidth) || maxLineWidth <= 0) {
        cout << "Invalid width parameter metrics.\n";
        return 1;
    }

    cout << "Enter the total count of words to process: ";
    if (!(cin >> wordsCount) || wordsCount <= 0) {
        cout << "Invalid element allocation size.\n";
        return 1;
    }

    vector<string> wordsList(static_cast<size_t>(wordsCount));
    cout << "Enter the words sequentially below:\n";
    for (int i = 0; i < wordsCount; ++i) {
        cin >> wordsList[static_cast<size_t>(i)];
        if (static_cast<int>(wordsList[static_cast<size_t>(i)].length()) > maxLineWidth) {
            cout << "Constraint Error: Individual word cannot exceed maxWidth parameters.\n";
            return 1;
        }
    }

    Solution solver;
    cout << "\nDistributing word spacing metrics and rendering text frames...\n\n";
    vector<string> formattedText = solver.fullJustify(wordsList, maxLineWidth);

    // Render output within a visual bounding frame
    cout << string(static_cast<size_t>(maxLineWidth + 2), '-') << "\n";
    for (const string& line : formattedText) {
        cout << "|" << line << "|\n";
    }
    cout << string(static_cast<size_t>(maxLineWidth + 2), '-') << "\n";

    return 0;
}