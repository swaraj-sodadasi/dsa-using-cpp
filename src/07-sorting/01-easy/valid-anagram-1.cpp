#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * FREQUENCY SORTING BUCKET VERIFICATION ENGINE
     * --------------------------------------------
     * The objective is to determine if string `t` is an anagram of string `s`.
     * An anagram is a word or phrase formed by rearranging the letters of a 
     * different word or phrase, using all the original letters exactly once.
     *
     * Algorithmic Strategy:
     * - Instead of performing an O(N log N) comparison by sorting the strings, we optimize 
     *   the verification to O(N) using a frequency sorting bucket mapping technique.
     * - Since the inputs are constrained to lowercase English letters ('a' through 'z'), 
     *   we instantiate a fixed-size bucket array of 26 slots to record character counts.
     *
     * Verification Protocol:
     * - Step 1: Check if strings `s` and `t` have identical lengths. If their lengths differ, 
     *   it is mathematically impossible for them to be anagrams. Return `false` immediately.
     * - Step 2: Iterate through both strings simultaneously. For every character processed in `s`, 
     *   increment its respective bucket balance (`bucket[s[i] - 'a']++`). For every character 
     *   processed in `t`, decrement its balance (`bucket[t[i] - 'a']--`).
     * - Step 3: Scan the frequency buckets. If all character counts have returned to exactly 0, 
     *   the character distribution matches perfectly. If any bucket contains a non-zero value, 
     *   return `false`.
     */
    bool isAnagram(string s, string t) {
        // Step 1: Core length validation check
        if (s.length() != t.length()) {
            return false;
        }

        // Step 2: Initialize a static character distribution tracking bucket array
        vector<int> frequencyBuckets(26, 0);
        size_t length = s.length();

        // Populate and deplete frequency records simultaneously
        for (size_t i = 0; i < length; ++i) {
            frequencyBuckets[static_cast<size_t>(s[i] - 'a')]++;
            frequencyBuckets[static_cast<size_t>(t[i] - 'a')]--;
        }

        // Step 3: Verify bucket structural balance integrity
        for (int count : frequencyBuckets) {
            if (count != 0) {
                return false; // Character distribution mismatch discovered
            }
        }

        return true; // Character profiles are perfectly identical
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the length of the input strings. The algorithm 
 *                  performs a single continuous linear pass to count character frequencies, 
 *                  followed by a constant-time O(26) validation loop.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization uses a fixed-size frequency array of 
 *                   26 elements, which consumes the same minimal stack space regardless of 
 *                   the input string size.
 */
int main() {
    string s, t;
    cout << "Enter the initial reference baseline string (s): ";
    if (!(cin >> s)) return 1;

    cout << "Enter the candidate validation target string (t): ";
    if (!(cin >> t)) return 1;

    Solution solver;
    cout << "\nExecuting frequency sorting bucket validation scan..." << endl;
    
    cout << boolalpha;
    bool result = solver.isAnagram(s, t);

    cout << "Anagram validation determination result: " << result << endl;

    return 0;
}