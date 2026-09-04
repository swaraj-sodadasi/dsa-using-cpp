#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
private:
    inline bool isVowel(char c) const {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MAXIMUM NUMBER OF VOWELS IN A SUBSTRING OF GIVEN LENGTH
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum number of vowel letters in any substring 
     * of string `s` with length `k`.
     * 
     * 1. FIXED-SIZE SLIDING WINDOW LAYOUT:
     *    We maintain a fixed sliding window of size `k`.
     * 
     * 2. INITIAL WINDOW EVALUATION:
     *    We compute the vowel count for the first `k` characters (`[0, k - 1]`).
     * 
     * 3. WINDOW SHIFTING & O(1) UPDATES:
     *    For each index `i` from `k` to `s.length() - 1`:
     *      - Subtract 1 from `currentVowels` if the character leaving the window (`s[i - k]`) is a vowel.
     *      - Add 1 to `currentVowels` if the character entering the window (`s[i]`) is a vowel.
     *      - Update `maxVowels` with `max(maxVowels, currentVowels)`.
     */
    int maxVowels(const string& s, int k) {
        int currentVowels = 0;
        int n = static_cast<int>(s.length());

        // Process initial window of length k
        for (int i = 0; i < k && i < n; ++i) {
            if (isVowel(s[static_cast<size_t>(i)])) {
                currentVowels++;
            }
        }

        int maxVowelCount = currentVowels;

        // Slide the fixed window across the rest of the string
        for (int i = k; i < n; ++i) {
            if (isVowel(s[static_cast<size_t>(i - k)])) {
                currentVowels--;
            }
            if (isVowel(s[static_cast<size_t>(i)])) {
                currentVowels++;
            }
            maxVowelCount = max(maxVowelCount, currentVowels);
            
            // Early exit optimization if theoretical maximum reached
            if (maxVowelCount == k) return k;
        }

        return maxVowelCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of string s.
 *                  Single linear pass across the string.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Constant extra space used for counters.
 */
int main() {
    cout << "=== Maximum Vowels in Substring Fixed Window Engine ===\n";
    cout << "Enter input string s: ";
    string s;
    if (!(cin >> s)) return 1;

    cout << "Enter window length k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    int result = solver.maxVowels(s, k);

    cout << "Maximum number of vowels in substring of length " << k << ": [" << result << "]\n";

    return 0;
}