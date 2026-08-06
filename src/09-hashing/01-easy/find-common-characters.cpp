#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: GLOBAL MINIMUM INTERSECTING FREQUENCIES
     * -------------------------------------------------------------------------------------
     * The objective is to find all characters that appear in every string of a given array,
     * including their duplicate counts.
     * 
     * 1. TRACKING VIA INTERSECTING MINIMUM FREQUENCIES:
     *    A character belongs in the final intersection if and only if its frequency count across
     *    all words is greater than 0. The total number of times it should appear in the output 
     *    is bounded by its *minimum frequency* across all individual strings.
     * 
     * 2. FIXED-SIZE TRACKING BUFFERS:
     *    Instead of using heavy hash maps, we allocate a fixed-size array `globalMinFreq` of size 26
     *    (initialized to INT_MAX) to store the global intersection limits for letters 'a'-'z'.
     * 
     * 3. INDEPENDENT WORD FREQUENCY sweeps:
     *    For each string, we build a temporary frequency array `currentWordFreq`. After scanning the 
     *    current word, we update our global intersection limits by keeping the smaller value for each slot:
     *    `globalMinFreq[i] = min(globalMinFreq[i], currentWordFreq[i])`.
     * 
     * 4. STRING RECONSTRUCTION:
     *    Finally, we unpack `globalMinFreq`. For each character index, we append the matching letter 
     *    to our results vector as many times as its stored minimum frequency specifies.
     */
    vector<string> commonChars(vector<string>& words) {
        // Step 2: Global intersection array mapping lowercase alphabetic letters
        vector<int> globalMinFreq(26, INT_MAX);

        // Step 3: Compute frequencies for each word and update global minimums
        for (const string& word : words) {
            vector<int> currentWordFreq(26, 0);
            for (char c : word) {
                currentWordFreq[static_cast<size_t>(c - 'a')]++;
            }

            for (int i = 0; i < 26; ++i) {
                globalMinFreq[static_cast<size_t>(i)] = min(globalMinFreq[static_cast<size_t>(i)], currentWordFreq[static_cast<size_t>(i)]);
            }
        }

        // Step 4: Reconstruct results vector from intersecting frequency limits
        vector<string> sharedCharactersResult;
        for (int i = 0; i < 26; ++i) {
            while (globalMinFreq[static_cast<size_t>(i)] > 0 && globalMinFreq[static_cast<size_t>(i)] != INT_MAX) {
                sharedCharactersResult.push_back(string(1, static_cast<char>('a' + i)));
                globalMinFreq[static_cast<size_t>(i)]--;
            }
        }

        return sharedCharactersResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(W * L) - Where W represents the total number of strings in the words array 
 *                  and L represents the average character length of the strings. We parse every character 
 *                  exactly once per word, followed by a constant-bound loop of 26 steps to update frequencies.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The frequency arrays (`globalMinFreq` and `currentWordFreq`) 
 *                   stay strictly locked at a fixed capacity of 26 slots, independent of input size scaling.
 */
int main() {
    int wordCount;
    cout << "Enter the total number of words in the array: ";
    if (!(cin >> wordCount) || wordCount <= 0) {
        cout << "Invalid word count configuration value." << endl;
        return 1;
    }

    vector<string> words(static_cast<size_t>(wordCount));
    cout << "Enter the words string inputs consecutively:\n";
    for (int i = 0; i < wordCount; ++i) {
        cin >> words[static_cast<size_t>(i)];
        for (char c : words[static_cast<size_t>(i)]) {
            if (c < 'a' || c > 'z') {
                cout << "Constraint Error: Inputs must contain lowercase letters exclusively." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting global intersecting frequency lookup routines..." << endl;
    vector<string> resultList = solver.commonChars(words);

    cout << "Calculated intersecting common characters: [ ";
    for (size_t i = 0; i < resultList.size(); ++i) {
        cout << "\"" << resultList[i] << "\"" << (i + 1 < resultList.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}