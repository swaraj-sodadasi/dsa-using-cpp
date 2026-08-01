#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * GLOBAL MIN-FREQUENCY ARRAY INTERSECTION ENGINE
     * ----------------------------------------------
     * The problem asks us to find all characters that appear in every string of a given array,
     * including duplicates. For example, if a character appears 3 times in all strings, 
     * it must appear 3 times in the final output.
     *
     * Architectural Tracking Mechanics:
     * - We track character distributions using a fixed-size frequency array of length 26 
     *   (mapping lowercase 'a' through 'z'). This avoids the overhead of a standard hash map.
     * - `globalMinFrequencies`: A benchmark array tracking the absolute minimum frequency 
     *   of each character found across all evaluated strings. It is initialized to a large value.
     *
     * State Array Evaluation Sequence:
     * - We sweep through each string in the input vector sequentially.
     * - For each string, we compute a local frequency map (`localFrequencies`) to count its 
     *   specific character counts in a linear pass.
     * - After reading a full string, we intersect its counts with our global tracker by performing 
     *   an element-wise minimum update: `globalMinFrequencies[i] = min(globalMinFrequencies[i], localFrequencies[i])`.
     * - If a character does not exist in the current string, its local count is 0, which correctly 
     *   forces its global count to 0, pruning it from the intersection.
     * - Once all strings have been processed, any character slot in `globalMinFrequencies` that remains 
     *   greater than 0 is flattened back into individual character string copies for the final output.
     */
    vector<string> commonChars(vector<string>& words) {
        // Initialize global frequency bounds tracker to max constraint capacity values
        vector<int> globalMinFrequencies(26, 10000);

        for (const string& word : words) {
            vector<int> localFrequencies(26, 0);
            
            // Build the frequency profile map for the current word string segment
            for (char ch : word) {
                localFrequencies[static_cast<size_t>(ch - 'a')]++;
            }

            // Perform element-wise intersection tracking updates across our global bounds
            for (size_t i = 0; i < 26; ++i) {
                globalMinFrequencies[i] = min(globalMinFrequencies[i], localFrequencies[i]);
            }
        }

        vector<string> commonCharactersResult;

        // Flatten the remaining global frequencies back into our results structure
        for (size_t i = 0; i < 26; ++i) {
            while (globalMinFrequencies[i] > 0) {
                char commonChar = static_cast<char>('a' + i);
                commonCharactersResult.push_back(string(1, commonChar));
                globalMinFrequencies[i]--;
            }
        }

        return commonCharactersResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(W * L) - Where W represents the total number of words in the array 
 *                  and L matches the maximum string length character count. The logic completes 
 *                  in linear passes over the characters, updating fixed 26-element profiles in O(1) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The local and global frequency tracking allocations utilize 
 *                   fixed-size vectors capped at 26 elements, maintaining a static layout footprint.
 */
int main() {
    int wCount;
    cout << "Enter the total number of word strings to evaluate: ";
    if (!(cin >> wCount) || wCount <= 0) {
        cout << "Invalid word count parameter setup inputs." << endl;
        return 1;
    }

    vector<string> words(static_cast<size_t>(wCount));
    cout << "Enter the lowercase string segments separated by spaces:\n";
    for (int i = 0; i < wCount; ++i) {
        cin >> words[static_cast<size_t>(i)];
        // Validation check to ensure values remain within lowercase bounds constraints
        for (char c : words[static_cast<size_t>(i)]) {
            if (c < 'a' || c > 'z') {
                cout << "Constraint Error: Input strings must consist of lowercase English letters only." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting global min-frequency array intersection..." << endl;
    vector<string> result = solver.commonChars(words);

    cout << "\nCommon character intersections found across all inputs:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "\"" << result[i] << "\"";
        if (i + 1 < result.size()) cout << ", ";
    }
    cout << " ]\n";
    cout << "Total overlapping characters captured: " << result.size() << endl;

    return 0;
}