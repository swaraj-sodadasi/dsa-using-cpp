#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BITMASK STATE TRACKING FOR PALINDROMIC PARITY):
- The problem asks us to find the maximum length of a substring that can be rearranged to form a palindrome.
- **Palindromic Requirement**:
  - A string of characters can be rearranged into a palindrome if and only if **at most one** character type has an odd frequency count.
  - Since the input string `s` consists solely of numeric digits ('0' - '9'), there are exactly 10 possible character types.
- **Parity Representation via Bitmasks**:
  - We can track the frequency parity (even or odd) of each digit using a 10-bit integer mask.
  - The $i$-th bit of our mask represents the parity of digit $i$ (0 for an even frequency, 1 for an odd frequency).
  - When we encounter a digit `d` (where $d = \text{c} - '0'$), we toggle its parity bit using a bitwise XOR operation: `mask ^= (1 << d)`.
- **Finding Valid Substrings**:
  - Suppose the prefix parity mask up to index `j` is `mask`. A substring ending at `j` starting after index `i` is valid if the character frequencies in that substring meet our palindromic requirement.
  - **Case 1: All characters have even frequencies inside the substring**
    - This happens if the prefix mask at index `i` is identical to `mask`. The substring length is `j - firstOccurrence[mask]`.
  - **Case 2: Exactly one character has an odd frequency inside the substring**
    - This happens if the prefix mask at index `i` differs from `mask` at exactly one bit position. 
    - We check all 10 possible single-bit variations by calculating `targetMask = mask ^ (1 << oddDigit)`. If `targetMask` has been seen before, the valid substring length is `j - firstOccurrence[targetMask]`.
- **Optimization Strategy**:
  - We use an array/vector `firstOccurrence` of size 1024 ($2^{10}$) initialized to -2 to store the earliest index where each unique mask was seen. The base state mask 0 is initialized to index -1.

TIME COMPLEXITY: O(10 * N) = O(N) - We perform a single linear pass over the string of length N. For each character, we inspect 10 single-bit variations in constant time.
SPACE COMPLEXITY: O(2^10) = O(1) auxiliary - The lookup table size is strictly bounded by 1024 entries regardless of how large the input string grows.
*/
int longestAwesome(const string& s) {
    // 2^10 = 1024 unique bitmask combinations possible for digits 0-9
    vector<int> firstOccurrence(1024, -2);
    
    int mask = 0;
    firstOccurrence[mask] = -1; // Base case: an empty prefix has a mask of 0 at index -1
    int maxLength = 0;

    for (int j = 0; j < static_cast<int>(s.length()); ++j) {
        int digit = s[static_cast<size_t>(j)] - '0';
        mask ^= (1 << digit); // Toggle the parity bit for the current digit

        // Case 1: Substring where all character frequencies are even (matching mask)
        if (firstOccurrence[mask] != -2) {
            maxLength = max(maxLength, j - firstOccurrence[mask]);
        }

        // Case 2: Substring where exactly one character frequency is odd
        for (int oddDigit = 0; oddDigit < 10; ++oddDigit) {
            int targetMask = mask ^ (1 << oddDigit);
            if (firstOccurrence[targetMask] != -2) {
                maxLength = max(maxLength, j - firstOccurrence[targetMask]);
            }
        }

        // Record the current mask if it is the first time we have encountered this configuration
        if (firstOccurrence[mask] == -2) {
            firstOccurrence[mask] = j;
        }
    }

    return maxLength;
}

int main() {
    string s;
    cout << "Enter a string consisting entirely of numeric digits: ";
    if (!(cin >> s)) {
        cout << "Invalid string format configuration entry." << endl;
        return 1;
    }

    // Input verification to ensure content constraints are respected
    for (char c : s) {
        if (c < '0' || c > '9') {
            cout << "Constraint Error: String contains non-digit character variations." << endl;
            return 1;
        }
    }

    int result = longestAwesome(s);
    cout << "\nThe length of the longest awesome palindromic substring is: " << result << endl;

    return 0;
}