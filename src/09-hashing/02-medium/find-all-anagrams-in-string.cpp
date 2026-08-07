#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SLIDING WINDOW CHARACTER FREQUENCY PROFILING
     * -------------------------------------------------------------------------------------
     * The objective is to find all starting indices of string p's anagrams in string s.
     * Both strings contain lowercase English letters exclusively.
     * 
     * 1. DIMENSION PRUNING HEURISTIC:
     *    If the length of target string `s` is shorter than pattern string `p`, it is 
     *    mathematically impossible to find any valid anagram window. Return empty vector.
     * 
     * 2. FIXED-SIZE FREQUENCY ARRAY PROFILES:
     *    Instead of heavy overhead dynamic map structures, we allocate two fixed-size arrays 
     *    of size 26 (`pCount` and `sCount`) mapping characters directly to index coordinates:
     *    `index = char - 'a'`.
     * 
     * 3. THE SLIDING WINDOW CRITERION:
     *    - Initial Window Setup: We tally the occurrences of the first `p.length()` characters 
     *      in both `p` and `s`. If the frequency arrays match exactly, index 0 is an anagram start.
     *    - Linear Sliding Phase: We slide a window of size `p.length()` across `s` from left to right.
     *      At each iteration, we include one new character on the right and eject one character 
     *      from the left, adjusting the `sCount` table in true $O(1)$ constant time.
     *    - At every step, we check for profile equivalence: if `sCount == pCount`, the current 
     *      left window boundary is logged into our results vector.
     */
    vector<int> findAnagrams(string s, string p) {
        int sLen = static_cast<int>(s.length());
        int pLen = static_cast<int>(p.length());

        // Step 1: Early structural constraint boundary check
        if (sLen < pLen) {
            return {};
        }

        // Step 2: Set up static 26-element character frequency profiles
        vector<int> pCount(26, 0);
        vector<int> sCount(26, 0);

        // Populate initial frequency snapshots for pattern and first window block of s
        for (int i = 0; i < pLen; ++i) {
            pCount[static_cast<size_t>(p[static_cast<size_t>(i)] - 'a')]++;
            sCount[static_cast<size_t>(s[static_cast<size_t>(i)] - 'a')]++;
        }

        vector<int> startingIndicesResult;
        
        // Check structural congruence for the initial window at index position 0
        if (sCount == pCount) {
            startingIndicesResult.push_back(0);
        }

        // Step 3: Slide the window across the remaining characters of string s
        for (int i = pLen; i < sLen; ++i) {
            // Append the incoming trailing character on the right
            sCount[static_cast<size_t>(s[static_cast<size_t>(i)] - 'a')]++;
            
            // Discard the outgoing leading character from the left boundary
            int leftBoundaryIndex = i - pLen;
            sCount[static_cast<size_t>(s[static_cast<size_t>(leftBoundaryIndex)] - 'a')]--;

            // Equivalence check against target template pattern profile map
            if (sCount == pCount) {
                startingIndicesResult.push_back(leftBoundaryIndex + 1);
            }
        }

        return startingIndicesResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of string s. The sliding loop
 *                  executes exactly N - pLen iterations. Inside the loop, frequency adjustments
 *                  run in O(1), and array comparisons take a maximum fixed 26 steps, which simplifies 
 *                  to a highly optimized linear-time pass.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sliding frequency maps stay strictly locked at a static 
 *                   capacity of 26 integers each, completely independent of string input sizes.
 */
int main() {
    string s, p;
    cout << "Enter the primary target search string (s): ";
    if (!(cin >> s)) return 1;
    cout << "Enter the target pattern template string (p): ";
    if (!(cin >> p)) return 1;

    // Validate lowercase constraints
    for (char c : s + p) {
        if (c < 'a' || c > 'z') {
            cout << "Constraint Error: Inputs must contain lowercase letters exclusively." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nLaunching sliding window character frequency checks..." << endl;
    vector<int> outputResult = solver.findAnagrams(s, p);

    cout << "Anagram matches discovered at starting indices: [ ";
    for (size_t i = 0; i < outputResult.size(); ++i) {
        cout << outputResult[i] << (i + 1 < outputResult.size() ? ", " : "");
    }
    cout << " ]\nTotal Windows Identified: " << outputResult.size() << endl;

    return 0;
}