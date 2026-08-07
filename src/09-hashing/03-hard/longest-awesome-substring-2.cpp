#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BITMASK CHARACTER-PARITY STATE PREFIX HASHING
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum length of a contiguous substring that can be 
     * rearranged into a palindrome. The input string contains numeric digit characters ('0'-'9').
     * 
     * 1. THE PALINDROME REARRANGEMENT RULE:
     *    A string can be rearranged into a palindrome if and only if the counts of its unique 
     *    characters satisfy one of two conditions:
     *      - Case A (Even Length): Every character occurs an EVEN number of times (0 odd counts).
     *      - Case B (Odd Length): Exactly ONE character occurs an ODD number of times.
     * 
     * 2. THE BITMASK PARITY STATE PROFILE:
     *    Since we only care about whether counts are even or odd, we can represent the parity 
     *    state using a 10-bit integer mask. Each bit position (0 to 9) corresponds to a digit.
     *      - Bit `i` is 0: Digit `i` has appeared an EVEN number of times so far.
     *      - Bit `i` is 1: Digit `i` has appeared an ODD number of times so far.
     *    As we traverse the string, we update our running prefix mask by toggling the bit of 
     *    the current digit using the XOR operator: `runningMask ^= (1 << digit)`.
     * 
     * 3. THE PREFIX HASH REGISTRY MATCHING:
     *    The parity state of a substring spanning from index `j + 1` to `i` is given by:
     *    `SubstringParityState = runningMask[i] ^ runningMask[j]`
     *    We want this `SubstringParityState` to represent a valid palindrome state.
     * 
     *    - Sub-case A (All Even Parities): We need `runningMask[i] ^ runningMask[j] == 0`, 
     *      which means `runningMask[j] == runningMask[i]`. If this exact mask was seen before, 
     *      we compute the window width: `i - earliest_seen_index`.
     *    - Sub-case B (Exactly One Odd Parity): We need `runningMask[i] ^ runningMask[j] == (1 << oddDigit)`. 
     *      Rearranging gives `runningMask[j] = runningMask[i] ^ (1 << oddDigit)`. We loop through all 
     *      10 possible digits (0-9), flip one bit of `runningMask[i]` at a time, and look up the 
     *      resulting complement mask in our history.
     * 
     * 4. EFFICIENCY OPTIMIZATION:
     *    Since a 10-bit mask can only take values between 0 and 2^10 - 1 (1023), we can replace the 
     *    `unordered_map` with a fixed-size tracking array of size 1024, initialized to -2. 
     *    We seed index 0 with -1 to handle valid palindromic prefixes starting from index 0.
     */
    int longestAwesome(string s) {
        // Fast-lookup registry array storing the earliest index where each mask is encountered.
        // Size is 2^10 = 1024. Initialize with -2 to mark them as unvisited.
        vector<int> earliestMaskIndex(1024, -2);
        
        // Base case: A prefix mask of 0 occurs implicitly at index -1
        earliestMaskIndex[0] = -1;

        int runningMask = 0;
        int longestAwesomeWindow = 0;

        for (int i = 0; i < static_cast<int>(s.length()); ++i) {
            int digit = s[static_cast<size_t>(i)] - '0';
            
            // Toggle the bit corresponding to the current digit character
            runningMask ^= (1 << digit);

            // Sub-case A: Check if the exact same prefix mask has been encountered before
            if (earliestMaskIndex[static_cast<size_t>(runningMask)] != -2) {
                longestAwesomeWindow = max(longestAwesomeWindow, i - earliestMaskIndex[static_cast<size_t>(runningMask)]);
            }

            // Sub-case B: Allow exactly one digit to have an odd frequency parity profile
            for (int oddDigit = 0; oddDigit <= 9; ++oddDigit) {
                int requiredComplementMask = runningMask ^ (1 << oddDigit);
                
                if (earliestMaskIndex[static_cast<size_t>(requiredComplementMask)] != -2) {
                    longestAwesomeWindow = max(longestAwesomeWindow, i - earliestMaskIndex[static_cast<size_t>(requiredComplementMask)]);
                }
            }

            // If this is the first time we've seen this mask, register its index
            if (earliestMaskIndex[static_cast<size_t>(runningMask)] == -2) {
                earliestMaskIndex[static_cast<size_t>(runningMask)] = i;
            }
        }

        return longestAwesomeWindow;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string s. The single-pass 
 *                  loop parses each character exactly once. Inside the loop, testing the 10 odd-parity 
 *                  variations takes a fixed 10 steps, completing in true O(1) constant runtime.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The lookup array allocation is strictly locked at a static 
 *                   capacity of 1024 integers, remaining completely independent of the input string length.
 */
int main() {
    string inputStr;
    cout << "Enter the numeric source string to evaluate (digits 0-9 exclusively):\n";
    if (!(cin >> inputStr)) return 1;

    // Validate string digit parameters constraints
    for (char c : inputStr) {
        if (c < '0' || c > '9') {
            cout << "Constraint Error: Input must contain numeric digits ('0'-'9') exclusively." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting prefix parity bitmask comparisons and window evaluations..." << endl;
    int resultingMaxLength = solver.longestAwesome(inputStr);

    cout << "Length of the longest awesome palindromic substring isolated: " << resultingMaxLength << endl;

    return 0;
}