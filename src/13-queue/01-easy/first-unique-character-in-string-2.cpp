#include <iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: QUEUE-BASED ONLINE STREAM PROCESSING
     * -------------------------------------------------------------------------------------
     * The objective is to find the index of the first non-repeating character in a string.
     * 
     * 1. STREAM-READY DESIGN INTUITION:
     *    In an online streaming model, characters arrive one-by-one. Instead of scanning 
     *    the entire string repeatedly, we use a queue to preserve the sequential order of 
     *    arrival and a frequency array to track occurrence counts.
     * 
     * 2. STATE TRANSITION & LAZY EVICTION:
     *    We sweep the string from left to right:
     *      - Increment the frequency counter for the incoming character.
     *      - Push a pair containing the character and its original index onto the queue.
     *      - "Lazy Eviction": We inspect the front of the queue. If the character at the front 
     *        now has a frequency count greater than 1, it is no longer a unique candidate. We 
     *        pop it from the queue and repeat this check until the front element is unique.
     * 
     * 3. TERMINAL EVALUATION:
     *    After parsing, the element sitting at the front of the queue is guaranteed to be 
     *    the first unique character in the stream. If the queue is completely empty, no 
     *    unique character exists.
     */
    int firstUniqChar(string s) {
        // Frequency array tracking occurrences of lowercase English alphabets
        vector<int> charFrequencies(26, 0);
        // Queue storing pairs of {character, index} to maintain stream arrival order
        queue<pair<char, int>> streamQueue;

        int n = static_cast<int>(s.length());
        for (int i = 0; i < n; ++i) {
            char ch = s[static_cast<size_t>(i)];
            int charIndex = ch - 'a';

            // Update state registers
            charFrequencies[static_cast<size_t>(charIndex)]++;
            streamQueue.push({ch, i});

            // Lazy eviction: remove non-unique characters from the front of the queue
            while (!streamQueue.empty()) {
                char frontChar = streamQueue.front().first;
                int frontCharIdx = frontChar - 'a';

                if (charFrequencies[static_cast<size_t>(frontCharIdx)] > 1) {
                    streamQueue.pop();
                } else {
                    break; // Unique candidate found at the front of the queue
                }
            }
        }

        return streamQueue.empty() ? -1 : streamQueue.front().second;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string.
 *                  Each character is pushed onto the queue once and popped at most once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The frequency list has a fixed allocation of 26 
 *                   integers, and the queue stores at most 26 unique character pairs.
 */
int main() {
    cout << "=== Queue-Based First Unique Character Stream Processor ===\n";
    cout << "Enter the lowercase alphabet string: ";
    
    string inputStr;
    if (!(cin >> inputStr)) return 1;

    Solution solver;
    cout << "\nInitializing stream registers and running lazy eviction sweeps..." << endl;
    int uniqueIndex = solver.firstUniqChar(inputStr);

    if (uniqueIndex != -1) {
        cout << "First Unique Character found at index: [" << uniqueIndex << "] -> '" 
             << inputStr[static_cast<size_t>(uniqueIndex)] << "'\n";
    } else {
        cout << "No unique characters present in the string sequence. Outcome: [-1]\n";
    }

    return 0;
}