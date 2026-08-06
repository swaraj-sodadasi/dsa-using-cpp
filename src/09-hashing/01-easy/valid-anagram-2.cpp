#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SINGLE-ARRAY FIXED BUCKET FREQUENCY COUNTING
     * -------------------------------------------------------------------------------------
     * The objective is to determine if string `t` is an anagram of string `s`.
     * An anagram is a word formed by rearranging the letters of a different word,
     * using all the original letters exactly once.
     * 
     * 1. DIMENSION PRUNING HEURISTIC:
     *    If the lengths of `s` and `t` do not match exactly, they cannot be anagrams. 
     *    We check this immediately to return false early.
     * 
     * 2. SINGLE-ARRAY COUNTER BALANCE ARCHITECTURE:
     *    Instead of allocating two hash tables, we allocate a single fixed-size array of 26 slots, 
     *    mapping characters directly to index positions: `index = char - 'a'`.
     *    We loop through both strings simultaneously:
     *      - Increment the count for characters found in `s`: `charCounts[s[i] - 'a']++`
     *      - Decrement the count for characters found in `t`: `charCounts[t[i] - 'a']--`
     * 
     * 3. ZERO-SUM VERIFICATION:
     *    If `t` is a valid anagram of `s`, every increment will be balanced by a matching decrement, 
     *    leaving all slots in the frequency array at exactly 0. Any non-zero value means a mismatch.
     */
    bool isAnagram(string s, string t) {
        // Step 1: Prune if lengths are mismatched
        if (s.length() != t.length()) {
            return false;
        }

        // Fixed-size frequency counter tracking buckets for letters 'a' through 'z'
        vector<int> charCounts(26, 0);

        // Step 2: Accumulate and balance frequency counts simultaneously
        for (size_t i = 0; i < s.length(); ++i) {
            charCounts[static_cast<size_t>(s[i] - 'a')]++;
            charCounts[static_cast<size_t>(t[i] - 'a')]--;
        }

        // Step 3: Verify the zero-sum balance condition across all buckets
        for (int count : charCounts) {
            if (count != 0) {
                return false;
            }
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the character length of the strings. The algorithm 
 *                  sweeps the strings in a single pass, performing array updates in true O(1) constant time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The count tracker stays locked at a capacity of 26 integer slots, 
 *                   independent of string length scaling.
 */
int main() {
    string s, t;
    cout << "Enter the first string (s): ";
    if (!(cin >> s)) return 1;
    cout << "Enter the second string (t): ";
    if (!(cin >> t)) return 1;

    // Validate lowercase constraints
    for (char c : s + t) {
        if (c < 'a' || c > 'z') {
            cout << "Constraint Error: Inputs must contain lowercase letters exclusively." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting single-array frequency balancing..." << endl;
    bool result = solver.isAnagram(s, t);

    if (result) {
        cout << "Outcome: The strings are valid anagrams of each other. (Result = true)" << endl;
    } else {
        cout << "Outcome: The strings are NOT anagrams. (Result = false)" << endl;
    }

    return 0;
}