#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DUAL-MAPPING CHARACTER-TO-CHARACTER VALIDATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine if two strings `s` and `t` are isomorphic.
     * Two strings are isomorphic if the characters in `s` can be replaced to get `t` while 
     * preserving character order. No two characters may map to the same target character, 
     * but a character may map to itself.
     * 
     * 1. THE BIJECTION PRINCIPLE (DUAL-MAPPING CONSTRAINT):
     *    A single map tracking `s[i] -> t[i]` is vulnerable to many-to-one mapping errors (where 
     *    multiple distinct characters in `s` target the same character in `t`). To enforce a strict 
     *    one-to-one bijection, we maintain two separate tracking maps simultaneously:
     *      - `mapS2T`: Stores mappings from `s` characters to `t` characters.
     *      - `mapT2S`: Stores mappings from `t` characters to `s` characters.
     * 
     * 2. FIXED-SIZE STRUCTURE INLINE OPTIMIZATION:
     *    Instead of heavy overhead dynamic map structures, we allocate fixed-size arrays of size 256 
     *    (covering the extended ASCII character spectrum). We initialize them with 0 to indicate an 
     *    unmapped state.
     * 
     * 3. SINGLE-PASS CONGRUENCE INSPECTION:
     *    We loop through both strings simultaneously. For each position `i`:
     *      - If an existing entry is found in either array, we check if it matches the current characters.
     *        Any contradiction (`mapS2T[s[i]] != t[i]` or `mapT2S[t[i]] != s[i]`) immediately returns false.
     *      - If no entry exists, we register the new bidirectional link and proceed.
     */
    bool isIsomorphic(string s, string t) {
        if (s.length() != t.length()) {
            return false;
        }

        // Fixed-size validation lookup arrays tracking the ASCII space
        // Initialized to 0 (representing an unmapped state placeholder)
        vector<int> mapS2T(256, 0);
        vector<int> mapT2S(256, 0);

        for (size_t i = 0; i < s.length(); ++i) {
            unsigned char charS = static_cast<unsigned char>(s[i]);
            unsigned char charT = static_cast<unsigned char>(t[i]);

            // Step 3: Validate cross-congruence flags against existing entries
            if (mapS2T[charS] != 0 && mapS2T[charS] != charT) {
                return false;
            }
            if (mapT2S[charT] != 0 && mapT2S[charT] != charS) {
                return false;
            }

            // Register the bidirectional relation maps if this is a new character encounter
            mapS2T[charS] = charT;
            mapT2S[charT] = charS;
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the character length of the strings. The algorithm 
 *                  sweeps the strings in a single pass, performing index reads and writes in 
 *                  true O(1) constant execution time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The internal validation vectors stay locked at a fixed size 
 *                   of 256 slots each, completely independent of string length scaling.
 */
int main() {
    string s, t;
    cout << "Enter the first string (s): ";
    if (!getline(cin, s)) return 1;
    cout << "Enter the second string (t): ";
    if (!getline(cin, t)) return 1;

    Solution solver;
    cout << "\nExecuting dual-mapping character congruence checks..." << endl;
    bool result = solver.isIsomorphic(s, t);

    if (result) {
        cout << "Outcome: The strings are structurally isomorphic. (Result = true)" << endl;
    } else {
        cout << "Outcome: The strings are NOT isomorphic. (Result = false)" << endl;
    }

    return 0;
}