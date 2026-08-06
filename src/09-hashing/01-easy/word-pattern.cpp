#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: STRING-TO-STRING BIJECTIVE DICTIONARY MAPPING
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a character `pattern` matches a space-separated string `s`.
     * A match means a strict bijection exists between each pattern letter and a non-empty word in `s`.
     * 
     * 1. PARSING AND PRE-CHECK SIZING PRUNING:
     *    We split string `s` into separate tokens using an `stringstream` buffer and store them in a 
     *    vector `words`. If the number of pattern characters does not match the total number of words, 
     *    a valid bijection is impossible, so we return false early.
     * 
     * 2. BIJECTIVE MAP CONFIGURATION:
     *    A single mapping structure can cause many-to-one bugs (where different keys map to the 
     *    same target value). To ensure a strict one-to-one relationship, we use two separate hash maps:
     *      - `charToWord`: Maps pattern characters to strings (`char -> string`).
     *      - `wordToChar`: Maps strings back to pattern characters (`string -> char`).
     * 
     * 3. SINGLE-PASS BIJECTIVE COMPARISON:
     *    We loop through the elements from left to right. For each position `i`:
     *      - If either map contains a history tracking record, we verify cross-congruence flags. 
     *        Any contradiction (`charToWord[c] != word` or `wordToChar[word] != c`) immediately returns false.
     *      - If no record exists, we log the new bidirectional dictionary entries and proceed.
     */
    bool wordPattern(string pattern, string s) {
        stringstream ss(s);
        string word;
        vector<string> words;

        // Step 1: Parse the string token stream into distinct words
        while (ss >> word) {
            words.push_back(word);
        }

        if (pattern.length() != words.size()) {
            return false;
        }

        // Step 2: Establish dual bijective mapping lookup registries
        unordered_map<char, string> charToWord;
        unordered_map<string, char> wordToChar;

        // Step 3: Verify character-to-word bidirectional alignment
        for (size_t i = 0; i < pattern.length(); ++i) {
            char c = pattern[i];
            const string& currWord = words[i];

            // Verify mapping consistency from character to word
            if (charToWord.count(c)) {
                if (charToWord[c] != currWord) {
                    return false;
                }
            } else {
                charToWord[c] = currWord;
            }

            // Verify mapping consistency from word back to character
            if (wordToChar.count(currWord)) {
                if (wordToChar[currWord] != c) {
                    return false;
                }
            } else {
                wordToChar[currWord] = c;
            }
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(P + W) - Where P represents the length of the pattern string and W represents 
 *                  the length of string s. Splitting string s takes linear time proportional to its size, 
 *                  and the subsequent bijection verification pass runs in linear O(P) time using amortized 
 *                  O(1) hash map lookups.
 *
 * SPACE COMPLEXITY: O(W) Auxiliary - Allocation space scales linearly to accommodate the parsed tokens vector 
 *                   and the hash dictionary structures storing unique string components from string s.
 */
int main() {
    string pattern, s;
    cout << "Enter the char pattern token (e.g., \"abba\"): ";
    if (!(cin >> pattern)) return 1;
    
    cin.ignore(); // Clear the newline indicator from the stream buffer before using getline
    cout << "Enter the space-separated text string string (s): ";
    if (!getline(cin, s)) return 1;

    Solution solver;
    cout << "\nExecuting bijective dictionary string mapping verifications..." << endl;
    bool result = solver.wordPattern(pattern, s);

    if (result) {
        cout << "Outcome: The input layout matches the specified structural word pattern. (Result = true)" << endl;
    } else {
        cout << "Outcome: Misaligned mapping detected. The layout does NOT match the pattern. (Result = false)" << endl;
    }

    return 0;
}