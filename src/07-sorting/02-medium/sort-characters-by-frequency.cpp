#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * MAX-HEAP BUCKET STRUCTURE FREQUENCY REARRANGEMENT ENGINE
     * --------------------------------------------------------
     * The objective is to sort a given string in decreasing order based on the 
     * frequency of its characters.
     *
     * Algorithmic Strategy (Bucket Map Architecture):
     * - Instead of using a standard heap (priority queue) which costs O(N log K) time, 
     *   we use a bucket structure framework to achieve a strict linear O(N) runtime.
     * - The absolute maximum frequency any single character can obtain is bounded by 
     *   the length of the string `N`.
     *
     * Processing Sequence Protocol:
     * - Step 1: Count character frequencies using a fast hash map (`unordered_map<char, int>`).
     * - Step 2: Establish a bucket map system (`vector<string> buckets`) of size `N + 1`, 
     *   where the index directly represents the character frequency count.
     * - Step 3: Iterate through the frequency records and append each character into its 
     *   respective bucket (`buckets[frequency]`).
     * - Step 4: Construct the resulting string by traversing the buckets array in reverse 
     *   (from index `N` down to 1). For each character found at a given frequency slot, 
     *   we append it to our output string exactly `frequency` times.
     */
    string frequencySort(string s) {
        size_t n = s.length();
        if (n == 0) return s;

        // Step 1: Record character distributions via hash mapping
        unordered_map<char, int> frequencyMap;
        for (char c : s) {
            frequencyMap[c]++;
        }

        // Step 2: Initialize frequency bucket array where index tracks occurrence count
        vector<string> buckets(n + 1, "");

        // Step 3: Distribute characters across their target frequency rows
        for (auto& pair : frequencyMap) {
            char ch = pair.first;
            int freq = pair.second;
            buckets[static_cast<size_t>(freq)] += ch;
        }

        // Step 4: Scan backwards from maximum frequency to rebuild the result string
        string sortedResult = "";
        sortedResult.reserve(n);

        for (int i = static_cast<int>(n); i > 0; --i) {
            if (!buckets[static_cast<size_t>(i)].empty()) {
                for (char c : buckets[static_cast<size_t>(i)]) {
                    // Append the character matching the active bucket index frequency
                    sortedResult.append(static_cast<size_t>(i), c);
                }
            }
        }

        return sortedResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string s. 
 *                  Building the map takes O(N) time. Distributing characters into buckets 
 *                  takes linear time bounded by the unique alphabet size. Rebuilding the 
 *                  string inspects each bucket slot and copies N total characters, yielding O(N).
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The bucket system and internal frequency maps scale 
 *                   proportionally with the size footprint and unique character set of the input string.
 */
int main() {
    string s;
    cout << "Enter the input text string to sort by character frequency: ";
    if (!(cin >> s)) return 1;

    Solution solver;
    cout << "\nExecuting bucket distribution sorting matrix pass..." << endl;
    string resultStr = solver.frequencySort(s);

    cout << "Frequency rearranged string output results: " << resultStr << endl;

    return 0;
}