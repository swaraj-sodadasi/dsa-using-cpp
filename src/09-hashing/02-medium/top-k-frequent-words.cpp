#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: HASH MAP COMBINED WITH MIN-HEAP CUSTOM COMPARISON
     * -------------------------------------------------------------------------------------
     * The objective is to extract the top 'k' most frequent words from a list of strings,
     * sorted by frequency from highest to lowest. Words with identical frequencies must 
     * be sorted in lexicographical (alphabetical) ascending order.
     * 
     * 1. STAGE 1: ACCUMULATING GLOBAL OCCURRENCES
     *    We parse the list of strings and aggregate occurrence counts into an 
     *    `unordered_map<string, int>` called `wordFreqMap` in linear time.
     * 
     * 2. STAGE 2: MAINTAINING A SIZE-BOUNDED MIN-HEAP
     *    To satisfy the optimal O(N log k) runtime constraint, we avoid sorting the whole 
     *    map or using a large Max-Heap. Instead, we maintain a Min-Heap (`priority_queue`) 
     *    capped at size `k`. 
     *    
     *    CRITICAL COMPONENT: CUSTOM COMPARISON STRUCT
     *    Because a Min-Heap naturally drops its *smallest* elements, we must carefully structure 
     *    our comparison logic to handle frequency ties correctly for the final top-k extraction:
     *      - Priority A: Lower frequency elements are pushed to the top of the heap for early removal.
     *      - Priority B (Frequency Tie): The word that is *lexicographically larger* ("zoo" vs "apple") 
     *        is pushed to the top of the heap, ensuring it gets popped first. This leaves the 
     *        lexicographically smaller words safely inside our final top-k set.
     * 
     * 3. STAGE 3: EXTRACTING AND REVERSING THE SOLUTION
     *    Once the heap processes all entries, it contains the top 'k' frequent words. 
     *    Since it's a Min-Heap, popping elements unloads them from least frequent to most frequent. 
     *    We collect them into a vector and reverse it to present the final sorted output.
     */
    struct HeapComparator {
        bool operator()(const pair<string, int>& a, const pair<string, int>& b) const {
            // Condition 1: Primary evaluation based on frequency matching
            if (a.second != b.second) {
                return a.second > b.second; // Smaller frequency rises to the top
            }
            // Condition 2: Secondary evaluation based on lexicographical order during ties
            return a.first < b.first; // Alphabetically larger word rises to the top
        }
    };

    vector<string> topKFrequent(vector<string>& words, int k) {
        // Stage 1: Map all unique strings to their corresponding global occurrence count
        unordered_map<string, int> wordFreqMap;
        for (const string& word : words) {
            wordFreqMap[word]++;
        }

        // Stage 2: Push elements into the size-bounded custom Min-Heap framework
        priority_queue<pair<string, int>, vector<pair<string, int>>, HeapComparator> minHeap;

        for (const auto& entryPair : wordFreqMap) {
            minHeap.push(entryPair);
            // Evict the top element if the heap size exceeds k to keep memory overhead at O(k)
            if (static_cast<int>(minHeap.size()) > k) {
                minHeap.pop();
            }
        }

        // Stage 3: Extract the isolated top-k values out of the final heap structure
        vector<string> topKWordsResult(static_cast<size_t>(k));
        for (int i = k - 1; i >= 0; --i) {
            topKWordsResult[static_cast<size_t>(i)] = minHeap.top().first;
            minHeap.pop();
        }

        return topKWordsResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log k) - Where N represents the total count of words in the input vector. 
 *                  Building the initial hash map takes O(N) time. Iterating through the unique map 
 *                  entries and managing the size-bounded heap costs at most O(log k) per insertion, 
 *                  leading to an efficient total runtime signature.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The primary `wordFreqMap` requires linear space to hold up to 
 *                   N unique string entries, while the custom min-heap requires a small footprint of O(k).
 */
int main() {
    int wordCount, k;
    cout << "Enter the total number of words in the string array: ";
    if (!(cin >> wordCount) || wordCount <= 0) {
        cout << "Invalid word count configuration value." << endl;
        return 1;
    }

    vector<string> words(static_cast<size_t>(wordCount));
    cout << "Enter the words sequentially (Lowercase strings only):\n";
    for (int i = 0; i < wordCount; ++i) {
        cin >> words[static_cast<size_t>(i)];
    }

    cout << "Enter the target k threshold value: ";
    if (!(cin >> k) || k <= 0 || k > wordCount) {
        cout << "Invalid k parameter. Must be greater than 0 and less than or equal to the array size." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting occurrence counting and bounded custom heap extraction sweeps..." << endl;
    vector<string> outputResult = solver.topKFrequent(words, k);

    cout << "\nComputed Top " << k << " Most Frequent Words:\n[ ";
    for (size_t i = 0; i < outputResult.size(); ++i) {
        cout << "\"" << outputResult[i] << "\"" << (i + 1 < outputResult.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}