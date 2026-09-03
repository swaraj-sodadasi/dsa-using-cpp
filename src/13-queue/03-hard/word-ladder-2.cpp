#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <utility>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: WORD LADDER (BI-DIRECTIONAL BREADTH-FIRST SEARCH STATE OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * The objective is to find the length of the shortest transformation sequence from 
     * `beginWord` to `endWord` using a given dictionary `wordList`.
     * 
     * 1. BI-DIRECTIONAL SEARCH STATE REDUCTION:
     *    Standard single-source BFS has an exponential search space growth factor of O(B^D).
     *    By performing a bi-directional BFS—meeting in the middle from both the forward state 
     *    (`beginSet`) and backward state (`endSet`)—we reduce the search space to O(B^(D/2)), 
     *    substantially speeding up execution on deep pathways.
     * 
     * 2. ACTIVE SET SWAPPING INVARIANT:
     *    To maximize efficiency, we always choose to expand the smaller of the two sets 
     *    (`beginSet` and `endSet`). This keeps the branching factor and memory footprints 
     *    as low as possible.
     * 
     * 3. IN-PLACE TRANSITIONS AND O(1) LOCKOUTS:
     *    Instead of comparing the active word with every string in the word list (which is O(N)), 
     *    we mutate each character of the current word from 'a' to 'z' (O(L * 26)) and query our 
     *    hash sets. This approach scales exceptionally well for large dictionaries.
     */
    int ladderLength(const string& beginWord, const string& endWord, const vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (dict.find(endWord) == dict.end()) {
            return 0;
        }

        // Initialize forward and backward active frontier sets
        unordered_set<string> beginSet;
        unordered_set<string> endSet;

        beginSet.insert(beginWord);
        endSet.insert(endWord);

        int sequenceLength = 1;

        while (!beginSet.empty() && !endSet.empty()) {
            // Guarantee we are always expanding the smaller frontier set to minimize branching
            if (beginSet.size() > endSet.size()) {
                swap(beginSet, endSet);
            }

            unordered_set<string> nextFrontier;

            for (string word : beginSet) {
                size_t wordLen = word.length();
                for (size_t i = 0; i < wordLen; ++i) {
                    char originalChar = word[i];

                    // Mutate the character position across the English alphabet
                    for (char c = 'a'; c <= 'z'; ++c) {
                        if (c == originalChar) continue;

                        word[i] = c;

                        // Intersection check: frontiers have met in the middle
                        if (endSet.find(word) != endSet.end()) {
                            return sequenceLength + 1;
                        }

                        // Valid transition: word exists in our dictionary
                        if (dict.find(word) != dict.end()) {
                            nextFrontier.insert(word);
                            dict.erase(word); // Remove to prevent cycle re-entry
                        }
                    }
                    word[i] = originalChar; // Restore word state
                }
            }

            // Explicitly qualified call to std::move to prevent ADL issues or compiler warnings
            beginSet = std::move(nextFrontier);
            sequenceLength++;
        }

        return 0; // No valid transformation sequence exists
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * L * 26) - Where N represents the total number of words in 
 *                  the wordList, and L represents the maximum length of a word. 
 *                  For each generated state, we perform a constant number of operations.
 *
 * SPACE COMPLEXITY: O(N * L) Auxiliary - To hold the word dictionary and active 
 *                   search frontiers in memory.
 */
int main() {
    cout << "=== Bi-Directional Word Ladder BFS Optimizer ===\n";
    cout << "Enter the start word (beginWord): ";
    string beginWord;
    cin >> beginWord;

    cout << "Enter the target word (endWord): ";
    string endWord;
    cin >> endWord;

    cout << "Enter the number of words in the dictionary list: ";
    int listSize;
    if (!(cin >> listSize) || listSize < 0) return 1;

    vector<string> wordList;
    if (listSize > 0) {
        cout << "Enter the dictionary words sequentially:\n";
        for (int i = 0; i < listSize; ++i) {
            string temp;
            cin >> temp;
            wordList.push_back(temp);
        }
    }

    Solution solver;
    cout << "\nScanning combination layers and executing bi-directional frontier splits..." << endl;
    int steps = solver.ladderLength(beginWord, endWord, wordList);

    if (steps > 0) {
        cout << "Shortest transformation sequence length: [" << steps << "]\n";
    } else {
        cout << "No transformation sequence exists. Output: [0]\n";
    }

    return 0;
}