#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Solution {
private:
    /*
     * HELPER STRING TOKENIZER
     * Parses a space-delimited text sentence and extracts individual words into a vector.
     */
    vector<string> tokenize(const string& sentence) {
        vector<string> words;
        stringstream ss(sentence);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        return words;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: PREFIX AND SUFFIX WORD PARTITION ELIMINATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine if two sentences, `sentence1` and `sentence2`, are 
     * similar. Two sentences are similar if we can insert an arbitrary sentence (possibly 
     * empty) inside one of these sentences such that it becomes equal to the other sentence.
     * 
     * 1. THE DEQUE-LIKE INTERSECTION PRINCIPLE:
     *    Instead of tracking complicated sliding indexes or trying brute-force splits, we 
     *    break the sentences down into arrays of individual words. 
     *    If one sentence can be transformed into the other by inserting a block of words, 
     *    then the shorter sentence must be composed entirely of a matching prefix and/or 
     *    a matching suffix from the longer sentence. 
     *    Therefore, we can simply eliminate matching words from both ends (prefix and suffix) 
     *    simultaneously.
     * 
     * 2. MATCH SWEEP ELIMINATION PIPELINE:
     *    - We parse both strings into word arrays `words1` and `words2`.
     *    - We maintain a matching index tracking setup:
     *        * `left`: Increments from 0 onward as long as `words1[left] == words2[left]`. 
     *          This consumes the identical prefix partition.
     *        * `right1` and `right2`: Point to the trailing boundaries of both arrays. We 
     *          decrement them inward as long as the trailing words match. This consumes the 
     *          identical suffix partition.
     * 
     * 3. THE COMPLETION CRITERION:
     *    If all words in the shorter sentence are successfully cleared by this prefix and 
     *    suffix elimination pass, it proves that the unmatched portion is isolated to a single 
     *    continuous segment in the middle of the longer sentence. Thus, the sentences are similar.
     */
    bool areSentencesSimilar(string sentence1, string sentence2) {
        // Step 1: Tokenize sentences into word vectors
        vector<string> words1 = tokenize(sentence1);
        vector<string> words2 = tokenize(sentence2);

        int n1 = static_cast<int>(words1.size());
        int n2 = static_cast<int>(words2.size());

        int left = 0;
        // Phase A: Match prefix segments moving forward
        while (left < n1 && left < n2 && words1[static_cast<size_t>(left)] == words2[static_cast<size_t>(left)]) {
            left++;
        }

        int right1 = n1 - 1;
        int right2 = n2 - 1;
        // Phase B: Match suffix segments moving backward, stopping where the prefix pointer hit
        while (right1 >= left && right2 >= left && words1[static_cast<size_t>(right1)] == words2[static_cast<size_t>(right2)]) {
            right1--;
            right2--;
        }

        // Step 3: Check if either word array has been completely exhausted
        // If left exceeds right1, words1 is fully consumed. If left exceeds right2, words2 is fully consumed.
        return (left > right1 || left > right2);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N and M represent the total character lengths of sentence1 
 *                  and sentence2. Tokenizing the strings takes linear time, and the two-pointer 
 *                  index elimination loops check each word at most once.
 *
 * SPACE COMPLEXITY: O(N + M) Auxiliary - Dynamic vector allocations scale linearly with the total number 
 *                   of words parsed from the input sentences.
 */
int main() {
    string phrase1, phrase2;
    cout << "=== Sentence Similarity III Two-Pointer Partition Console ===\n";
    cout << "Enter the first sentence string:\n";
    if (!getline(cin, phrase1)) return 1;
    cout << "Enter the second sentence string:\n";
    if (!getline(cin, phrase2)) return 1;

    Solution solver;
    cout << "\nTokenizing word structures and eliminating matching partitions..." << endl;
    bool matchingSimilarityOutcome = solver.areSentencesSimilar(phrase1, phrase2);

    if (matchingSimilarityOutcome) {
        cout << "Result: The configured sentences are SIMILAR! (true)\n";
    } else {
        cout << "Result: The sentences are NOT SIMILAR due to structural fragmentation. (false)\n";
    }

    return 0;
}