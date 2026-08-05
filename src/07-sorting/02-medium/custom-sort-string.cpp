#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * CUSTOM ORDERING KEY PRE-COMPUTATION SORTING ENGINE
     * --------------------------------------------------
     * The objective is to permute the characters of string `s` such that they match 
     * the custom order defined by string `order`. Characters that do not appear in 
     * `order` can be placed anywhere at the end of the rearranged string.
     *
     * Algorithmic Choices:
     * - Option A (Custom Comparator Sorting): Use an O(26) mapping lookup array to sort `s` 
     *   via `std::sort`. This runs in O(N log N) time, where N is the length of `s`.
     * - Option B (Frequency Distribution Bucket Mapping): Count character frequencies in `s` 
     *   and reconstruct the string sequentially according to the sequence specified in `order`. 
     *   This runs in optimal O(N + M) time, where M is the length of `order`.
     *
     * Frequency Distribution Bucket Reconstruction Protocol:
     * - Step 1: Initialize a static character frequency tracking bucket array of 26 slots to 
     *   count the occurrences of each lowercase letter in `s`.
     * - Step 2: Iterate through the custom sequence definition string `order`. For each character, 
     *   if it exists in our frequency array, append it to our result string as many times as it 
     *   appeared in `s`, then clear its count to 0.
     * - Step 3: Iterate through our frequency array from 'a' to 'z' to gather any remaining 
     *   characters that were not explicitly specified in `order`, appending them to the tail.
     */
    string customSortString(string order, string s) {
        // Step 1: Record character frequency profiles inside stable bucket constraints
        vector<int> charCounts(26, 0);
        for (char c : s) {
            charCounts[static_cast<size_t>(c - 'a')]++;
        }

        string sortedResult = "";
        sortedResult.reserve(s.length());

        // Step 2: Reconstruct string sequentially based on the order sequence layout
        for (char c : order) {
            size_t idx = static_cast<size_t>(c - 'a');
            if (charCounts[idx] > 0) {
                sortedResult.append(static_cast<size_t>(charCounts[idx]), c);
                charCounts[idx] = 0; // Clear record block to acknowledge completion
            }
        }

        // Step 3: Append all residual elements left out of the sequence constraints
        for (int i = 0; i < 26; ++i) {
            if (charCounts[static_cast<size_t>(i)] > 0) {
                char ch = static_cast<char>('a' + i);
                sortedResult.append(static_cast<size_t>(charCounts[static_cast<size_t>(i)]), ch);
            }
        }

        return sortedResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N represents the total length of the target string `s` 
 *                  and M represents the size of the sequence layout string `order`. The algorithm 
 *                  performs linear counting passes over the inputs, bypassing heavy sorting cycles.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The custom optimization tracks state using a fixed-size 
 *                   frequency mapping array of 26 slots, requiring zero structural allocations.
 */
int main() {
    string order, s;
    cout << "Enter the custom character sequence ordering layout rule (order): ";
    if (!(cin >> order)) return 1;

    cout << "Enter the target string to be sorted (s): ";
    if (!(cin >> s)) return 1;

    Solution solver;
    cout << "\nExecuting custom key distribution mapping and reconstruction loops..." << endl;
    string customResult = solver.customSortString(order, s);

    cout << "Custom ordered string reconstruction output: " << customResult << endl;

    return 0;
}