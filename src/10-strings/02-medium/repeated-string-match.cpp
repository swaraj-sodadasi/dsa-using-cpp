#include <iostream>
#include <string>

using namespace std;

class Solution {
private:
    // Large prime for Rabin-Karp hashing to minimize collision probabilities
    static const long long MOD = 1000000007; 
    // Base multiplier chosen for the alphabet character set encoding
    static const long long BASE = 256;       

    /*
     * RABIN-KARP SUBSTRING ROLLING HASH ENGINE
     * Sweeps through a text string to find if a target pattern exists in linear time.
     */
    bool rabinKarpSearch(const string& text, const string& pattern) {
        size_t n = text.length();
        size_t m = pattern.length();
        if (n < m) return false;

        long long patternHash = 0;
        long long currentWindowHash = 0;
        long long highestBasePower = 1; // BASE^(m-1) % MOD

        // Compute the highest base factor for rolling transitions
        for (size_t i = 0; i < m - 1; ++i) {
            highestBasePower = (highestBasePower * BASE) % MOD;
        }

        // Phase A: Compute initial fingerprint hashes for the pattern and first window
        for (size_t i = 0; i < m; ++i) {
            patternHash = (patternHash * BASE + static_cast<unsigned char>(pattern[i])) % MOD;
            currentWindowHash = (currentWindowHash * BASE + static_cast<unsigned char>(text[i])) % MOD;
        }

        // Phase B: Execute rolling updates across the string sequence matrix
        for (size_t i = 0; i <= n - m; ++i) {
            // If hash match occurs, verify characters directly to handle potential collisions
            if (patternHash == currentWindowHash) {
                if (text.compare(i, m, pattern) == 0) {
                    return true;
                }
            }

            // Slide the window right: drop the leading character and add the trailing character
            if (i < n - m) {
                long long leadingCharComponent = (static_cast<unsigned char>(text[i]) * highestBasePower) % MOD;
                currentWindowHash = (currentWindowHash - leadingCharComponent + MOD) % MOD;
                currentWindowHash = (currentWindowHash * BASE + static_cast<unsigned char>(text[i + m])) % MOD;
            }
        }

        return false;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: RABIN-KARP SUBSTRING VALIDATION / UPPER LIMIT EXPANSION
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum number of times string `a` must be repeated 
     * so that string `b` becomes a substring of it.
     * 
     * 1. THE REPETITION UPPER BOUND BOUNDARY PRINCIPLE:
     *    Let length of `a` be N and length of `b` be M. 
     *    - The absolute minimum number of repetitions needed to even match the length of `b` 
     *      is `ceil(M / N)`.
     *    - However, the target sequence might start near the end of the first copy of `a` and 
     *      spill over. Therefore, we may need to append at most **two additional copies** 
     *      of `a` to ensure any offset alignment can be fully caught.
     *    - Any expansion beyond `ceil(M / N) + 2` is guaranteed to be redundant. If a match is 
     *      not found within this threshold, it is physically impossible, and we can abort early.
     * 
     * 2. MEMORY AND SEARCH PIPELINE:
     *    - We initialize an expanding buffer string `repeatedA` and keep track of our repeat count.
     *    - We append copies of `a` until `repeatedA.length() >= b.length()`.
     *    - We perform a Rabin-Karp rolling hash search. If it matches, we return the current count.
     *    - If it fails, we append `a` one more time (count + 1) and search again to catch alignment overflows.
     *    - If it fails again, we check one final copy (count + 2) before declaring failure and returning -1.
     */
    int repeatedStringMatch(string a, string b) {
        string repeatedA = "";
        int repeatCount = 0;

        // Phase 1: Expand 'a' until its length is at least equal to 'b'
        while (repeatedA.length() < b.length()) {
            repeatedA.append(a);
            repeatCount++;
        }

        // Test baseline expansion threshold
        if (rabinKarpSearch(repeatedA, b)) {
            return repeatCount;
        }

        // Phase 2: Append one more copy to handle offset boundary wraps
        repeatedA.append(a);
        repeatCount++;
        if (rabinKarpSearch(repeatedA, b)) {
            return repeatCount;
        }

        // Phase 3: Final fallback alignment boundary overflow check
        repeatedA.append(a);
        repeatCount++;
        if (rabinKarpSearch(repeatedA, b)) {
            return repeatCount;
        }

        return -1; // Proved impossible across all structural limits
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N represents the length of string a, and M represents the length 
 *                  of string b. Building the repeated buffer scales linearly with the upper limit length, 
 *                  and the Rabin-Karp rolling hash checks the combined text space in linear time.
 *
 * SPACE COMPLEXITY: O(N + M) Auxiliary - The system allocates a string buffer to hold the expanded 
 *                   repetitions of `a`, up to a maximum size cap of approximately M + 2N.
 */
int main() {
    string sourceA, targetB;
    cout << "=== Rabin-Karp Repeated String Matcher Console ===\n";
    cout << "Enter the source base string (a): ";
    if (!(cin >> sourceA)) return 1;
    cout << "Enter the target substring pattern to find (b): ";
    if (!(cin >> targetB)) return 1;

    Solution solver;
    cout << "\nCalculating expansion thresholds and running rolling hash scans..." << endl;
    int minimumRepetitions = solver.repeatedStringMatch(sourceA, targetB);

    if (minimumRepetitions != -1) {
        cout << "\nMatch found! String 'a' must be repeated a minimum of: " << minimumRepetitions << " times.\n";
    } else {
        cout << "\nOutcome: Substring matching is physically impossible. (Result = -1)\n";
    }

    return 0;
}