#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

/*
CORE LOGIC (BITMASK FREQUENCY MATCHING / SUBMASK ENUMERATION):
- The problem asks us to count how many words are valid for each puzzle. A word is valid for a puzzle if:
  1. The word contains the first letter of the puzzle.
  2. Every letter in the word is present in the puzzle.
- **Bitmask Conversion**:
  - Since character sequence order does not matter and we only care about unique lowercase English letters ('a'-'z'), 
    we can represent each word and puzzle as a 26-bit integer mask.
  - For example, if a word contains 'a' and 'c', its mask will have bits 0 and 2 set to 1 (`(1 << 0) | (1 << 2)`).
  - Optimization: If a word contains more than 7 unique characters, it can never be valid for *any* puzzle 
    (since puzzles always have exactly 7 unique characters). We can filter out these words immediately.
- **Frequency Matching Lookup**:
  - We precompute the bitmask for all valid words and store their frequencies in a hash map (`unordered_map<int, int> maskCounts`).
- **Submask Enumeration Trick**:
  - For each puzzle, instead of iterating through thousands of words to see if they fit the puzzle, we look at the puzzle itself.
  - A puzzle has exactly 7 unique characters, meaning there are only $2^6 = 64$ possible combinations of characters that 
    include the mandatory first letter.
  - Let `puzzleMask` represent all 7 characters of the puzzle, and `firstLetterMask` represent the mandatory first character.
  - We extract the optional characters: `submask = puzzleMask ^ firstLetterMask`.
  - We enumerate all mathematical submasks of this `submask` using the standard bitwise loop: 
    `curr = (curr - 1) & submask`.
  - For each submask, we add `firstLetterMask` back to construct a valid target word mask. If that target mask exists 
    in our `maskCounts` map, we add its frequency count to the puzzle's total match score.

TIME COMPLEXITY: O(W * L + P * 2^6) -> O(W * L + 64 * P) - Where W is the number of words, L is the average length of a word, 
                 and P is the number of puzzles. Submask enumeration handles each puzzle in exactly 64 iterations.
SPACE COMPLEXITY: O(W) auxiliary - In the worst-case scenario, the hash map stores a unique entry for every word mask.
*/

vector<int> findNumOfValidWords(const vector<string>& words, const vector<string>& puzzles) {
    unordered_map<int, int> maskCounts;

    // Step 1: Compress words into bitmasks and update frequency map
    for (const string& word : words) {
        int wordMask = 0;
        int uniqueCharCount = 0;
        
        for (char c : word) {
            int bitPosition = c - 'a';
            if ((wordMask & (1 << bitPosition)) == 0) {
                wordMask |= (1 << bitPosition);
                uniqueCharCount++;
            }
        }
        
        // A puzzle only has 7 unique characters. Skip words that exceed this threshold.
        if (uniqueCharCount <= 7) {
            maskCounts[wordMask]++;
        }
    }

    vector<int> results;
    results.reserve(puzzles.size());

    // Step 2: Evaluate each puzzle using fast submask enumeration
    for (const string& puzzle : puzzles) {
        int puzzleMask = 0;
        for (char c : puzzle) {
            puzzleMask |= (1 << (c - 'a'));
        }

        int firstLetterMask = 1 << (puzzle[0] - 'a');
        int validWordCount = 0;

        // Isolate the remaining 6 optional characters available for submask generation
        int optionalMask = puzzleMask ^ firstLetterMask;
        int currentSubmask = optionalMask;

        // Loop through all 64 variations of the optional characters
        while (true) {
            // Re-combine the mandatory first letter with the current optional submask selection
            int targetWordMask = currentSubmask | firstLetterMask;

            // Check if this specific character combination exists in our word library
            auto it = maskCounts.find(targetWordMask);
            if (it != maskCounts.end()) {
                validWordCount += it->second;
            }

            if (currentSubmask == 0) break;
            
            // Standard bitwise submask degradation step
            currentSubmask = (currentSubmask - 1) & optionalMask;
        }

        results.push_back(validWordCount);
    }

    return results;
}

int main() {
    int w, p;
    cout << "Enter the total number of words: ";
    if (!(cin >> w) || w <= 0) {
        cout << "Invalid words tracking configuration size." << endl;
        return 1;
    }

    vector<string> words(static_cast<size_t>(w));
    cout << "Enter the words separated by spaces:\n";
    for (int i = 0; i < w; ++i) {
        cin >> words[static_cast<size_t>(i)];
    }

    cout << "Enter the total number of puzzles: ";
    if (!(cin >> p) || p <= 0) {
        cout << "Invalid puzzles tracking configuration size." << endl;
        return 1;
    }

    vector<string> puzzles(static_cast<size_t>(p));
    cout << "Enter the puzzles (each exactly 7 characters) separated by spaces:\n";
    for (int i = 0; i < p; ++i) {
        cin >> puzzles[static_cast<size_t>(i)];
        if (puzzles[static_cast<size_t>(i)].length() != 7) {
            cout << "Constraint Error: Puzzles must be exactly 7 characters long." << endl;
            return 1;
        }
    }

    vector<int> results = findNumOfValidWords(words, puzzles);

    cout << "\nThe number of valid words matching each puzzle:\n";
    for (size_t i = 0; i < results.size(); ++i) {
        cout << "Puzzle \"" << puzzles[i] << "\" -> " << results[i] << " valid words.\n";
    }

    return 0;
}