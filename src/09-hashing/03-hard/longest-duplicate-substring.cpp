#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <string_view>

using namespace std;

class Solution {
private:
    /*
     * -------------------------------------------------------------------------------------
     * MATHEMATICAL COLLISION REDUCTION WITH DOUBLE ROLLING HASH
     * -------------------------------------------------------------------------------------
     * String hashing over large windows is prone to collisions. To practically eliminate 
     * collisions without the O(L) overhead of a full string comparison, we generate a 
     * composite double-hash pair using two large independent prime moduli:
     *   Mod 1 = 1,000,000,007, Mod 2 = 1,000,000,009.
     * We use a common base value (base = 31), which is sufficient for lowercase alphabets.
     */
    const long long BASE = 31;
    const long long MOD1 = 1000000007;
    const long long MOD2 = 1000000009;

    // Custom pair hash tracking structure to store composite hashes in unordered_map containers
    struct PairHash {
        size_t operator()(const pair<long long, long long>& p) const {
            return (static_cast<size_t>(p.first) << 32) ^ static_cast<size_t>(p.second);
        }
    };

    /*
     * RABIN-KARP ROLLING HASH SEARCH IMPLEMENTATION
     * Checks if there is any duplicate substring of length 'len' inside string 's'.
     * Returns the starting index if found, or -1 if no duplicates exist.
     */
    int checkDuplicateLength(const string& s, int len) {
        if (len == 0) return 0;

        // Precompute base^len % MOD factors for linear window evictions
        long long basePower1 = 1;
        long long basePower2 = 1;
        for (int i = 0; i < len; ++i) {
            basePower1 = (basePower1 * BASE) % MOD1;
            basePower2 = (basePower2 * BASE) % MOD2;
        }

        // Compute the initial polynomial hash value for the first window block [0 ... len-1]
        long long currentHash1 = 0;
        long long currentHash2 = 0;
        for (int i = 0; i < len; ++i) {
            currentHash1 = (currentHash1 * BASE + (s[static_cast<size_t>(i)] - 'a' + 1)) % MOD1;
            currentHash2 = (currentHash2 * BASE + (s[static_cast<size_t>(i)] - 'a' + 1)) % MOD2;
        }

        // Map stores: {Composite_Double_Hash_Pair -> List_Of_Starting_Indices}
        unordered_map<pair<long long, long long>, vector<int>, PairHash> hashRegistry;
        hashRegistry[{currentHash1, currentHash2}].push_back(0);

        int n = static_cast<int>(s.length());
        
        // Slide the fixed-size window across the remaining characters of the string
        for (int i = len; i < n; ++i) {
            // Roll Hash 1: Add trailing character and eject leading character
            currentHash1 = (currentHash1 * BASE + (s[static_cast<size_t>(i)] - 'a' + 1)) % MOD1;
            long long outValue1 = (basePower1 * (s[static_cast<size_t>(i - len)] - 'a' + 1)) % MOD1;
            currentHash1 = (currentHash1 - outValue1 + MOD1) % MOD1;

            // Roll Hash 2: Add trailing character and eject leading character
            currentHash2 = (currentHash2 * BASE + (s[static_cast<size_t>(i)] - 'a' + 1)) % MOD2;
            long long outValue2 = (basePower2 * (s[static_cast<size_t>(i - len)] - 'a' + 1)) % MOD2;
            currentHash2 = (currentHash2 - outValue2 + MOD2) % MOD2;

            pair<long long, long long> compositeHashKey = {currentHash1, currentHash2};

            // If the hash key exists, perform a string_view verification check to ensure absolute accuracy
            if (hashRegistry.find(compositeHashKey) != hashRegistry.end()) {
                string_view currentWindowStr(s.data() + i - len + 1, static_cast<size_t>(len));
                for (int pastStartIndex : hashRegistry[compositeHashKey]) {
                    string_view candidateStr(s.data() + pastStartIndex, static_cast<size_t>(len));
                    if (currentWindowStr == candidateStr) {
                        return i - len + 1; // Double hash confirmed: duplicate match isolated
                    }
                }
            }
            
            hashRegistry[compositeHashKey].push_back(i - len + 1);
        }

        return -1;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: LONGEST DUPLICATE SUBSTRING
     * -------------------------------------------------------------------------------------
     * The objective is to identify the longest duplicated substring within a string 's'.
     * 
     * 1. THE MONOTONIC BINARY SEARCH CONTEXT:
     *    If a duplicate substring of length L exists, it implies a duplicate substring of 
     *    length L - 1 must also exist. Conversely, if no duplicate substring of length L 
     *    can be found, it is impossible for any duplicate substring of length L + 1 to exist. 
     *    This monotonic relationship allows us to binary search over the range of possible lengths [0, N].
     * 
     * 2. THE COMBINATORIAL OPTIMIZATION SWEEP:
     *    - Binary Search selects a candidate length `mid`.
     *    - The Rabin-Karp rolling hash searches for a duplicate of length `mid` in linear time.
     *    - If found, we save the result and look for a longer match by moving the left bound (`low = mid + 1`).
     *    - If not found, we look for a shorter match by moving the right bound (`high = mid - 1`).
     */
    string longestDupSubstring(string s) {
        int n = static_cast<int>(s.length());
        int low = 1;
        int high = n - 1;
        
        int optimalStartIdx = -1;
        int maxSubstringLength = 0;

        // Step 1: Execute Binary Search across possible length domains
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int startIdx = checkDuplicateLength(s, mid);

            if (startIdx != -1) {
                optimalStartIdx = startIdx;
                maxSubstringLength = mid;
                low = mid + 1; // Try to isolate a longer duplicate window configuration
            } else {
                high = mid - 1; // Restrict search to smaller dimensions
            }
        }

        if (optimalStartIdx == -1) {
            return "";
        }
        return s.substr(static_cast<size_t>(optimalStartIdx), static_cast<size_t>(maxSubstringLength));
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the length of string s. The binary search 
 *                  divides the length domain space in O(log N) operations. For each step, the 
 *                  Rabin-Karp rolling hash searches the string in O(N) linear execution time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal `hashRegistry` hash map holds up to N 
 *                   fingerprint entries during a length confirmation pass.
 */
int main() {
    string s;
    cout << "Enter the primary string to evaluate (lowercase letters only):\n";
    if (!(cin >> s)) return 1;

    // Validate character bounds constraints
    for (char c : s) {
        if (c < 'a' || c > 'z') {
            cout << "Constraint Error: Input must contain lowercase letters exclusively." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nLaunching Rabin-Karp rolling hash checks across binary length bounds..." << endl;
    string duplicateResult = solver.longestDupSubstring(s);

    if (duplicateResult.empty()) {
        cout << "Outcome: No duplicate substring configurations exist inside the input." << endl;
    } else {
        cout << "Longest duplicate substring isolated: \"" << duplicateResult << "\"" << endl;
        cout << "Maximum sequence window length: " << duplicateResult.length() << " characters." << endl;
    }

    return 0;
}