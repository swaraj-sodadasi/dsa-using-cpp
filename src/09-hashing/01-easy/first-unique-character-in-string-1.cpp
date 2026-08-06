#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: TWO-PASS FREQUENCY ARRAY LOOKUP
     * -------------------------------------------------------------------------------------
     * The objective is to find the index of the first non-repeating character in a string.
     * If no unique character exists, return -1.
     * 
     * 1. FIXED-SIZE FREQUENCY MAP OPTIMIZATION:
     *    Instead of using a dynamic hash table which adds extra runtime overhead, we allocate
     *    a fixed-size array of size 26. Since the string is constrained to lowercase English 
     *    letters ('a'-'z'), we map characters directly to index positions: `index = s[i] - 'a'`.
     * 
     * 2. THE TWO-PASS ARCHITECTURE:
     *    - Pass 1 (Frequency Collection): We parse the string from left to right, incrementing 
     *      counts in the frequency array: `frequency[s[i] - 'a']++`.
     *    - Pass 2 (Linear Order Validation): We parse the string a second time from left to right. 
     *      For each character, we query its stored frequency. The first character encountered 
     *      with a frequency count exactly equal to 1 is guaranteed to be the first unique character.
     * 
     * 3. CONSTANT OVERHEAD GUARANTEE:
     *    The frequency tracking map relies strictly on O(1) space, making the execution speed 
     *    completely dependent on string length.
     */
    int firstUniqChar(string s) {
        // Frequency array tracking buckets for lowercase letters 'a' through 'z'
        vector<int> charFrequency(26, 0);

        // Pass 1: Aggregate global occurrence frequency counts in linear time
        for (char c : s) {
            charFrequency[static_cast<size_t>(c - 'a')]++;
        }

        // Pass 2: Sweep the string to identify the first character with a unique signature
        for (int i = 0; i < static_cast<int>(s.length()); ++i) {
            if (charFrequency[static_cast<size_t>(s[static_cast<size_t>(i)] - 'a')] == 1) {
                return i;
            }
        }

        return -1; // Fallback value return indicating no unique characters exist
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of the input string s.
 *                  The algorithm runs exactly two linear execution passes across the string structure, 
 *                  performing array index read/write lookups in pure O(1) constant time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The frequency array workspace remains strictly locked 
 *                   at a constant capacity of 26 integer slots, independent of string size scaling.
 */
int main() {
    string targetString;
    cout << "Enter the target string (Lowercase alphabetic characters only): ";
    if (!(cin >> targetString)) {
        cout << "Invalid string parameter input stream." << endl;
        return 1;
    }

    // Input verification loop matching baseline optimization constraints
    for (char c : targetString) {
        if (c < 'a' || c > 'z') {
            cout << "Constraint Error: Input string must contain lowercase letters exclusively." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nLaunching two-pass character frequency scans..." << endl;
    int resultingIndex = solver.firstUniqChar(targetString);

    if (resultingIndex != -1) {
        cout << "First unique character discovered: '" << targetString[static_cast<size_t>(resultingIndex)] 
             << "' at index location: " << resultingIndex << endl;
    } else {
        cout << "Outcome: No unique non-repeating characters identified in the string. Result = -1" << endl;
    }

    return 0;
}