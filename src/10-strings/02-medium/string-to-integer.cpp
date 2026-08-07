#include <iostream>
#include <string>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: EDGE CASE WHITE-SPACE, SIGN, AND OVERFLOW HANDLING
     * -------------------------------------------------------------------------------------
     * The objective is to implement a robust string-to-integer parsing engine equivalent 
     * to the standard `myAtoi` specifications, executing under strict safety guarantees.
     * 
     * 1. STAGED FINITE-STATE PARSING PIPELINE:
     *    Instead of complex regular expression backtracking systems, we process the string 
     *    in place sequentially via three structural phases:
     *      - Phase A: Skip all leading whitespace characters (`' '`).
     *      - Phase B: Capture sign markers. Check for a single optional positive `'+'` or 
     *        negative `'-'` character to determine the multiplier value (+1 or -1).
     *      - Phase C: Process numeric digits. Read incoming digit characters until a 
     *        non-digit character is encountered or the end of the string is reached.
     * 
     * 2. PROACTIVE MULTIPLICATION OVERFLOW GUARD CONTRACT:
     *    To prevent undefined behavior from 32-bit signed integer overflow during math accumulation, 
     *    we evaluate values *before* multiplying the running integer by 10 or adding the new digit.
     *    Let the upper bound limit values be `INT_MAX = 2147483647`.
     *    Before calculating `runningResult = runningResult * 10 + currentDigit`, we check:
     *      - If `runningResult > INT_MAX / 10`, then multiplying by 10 will overflow immediately.
     *      - If `runningResult == INT_MAX / 10` and `currentDigit > INT_MAX % 10` (which is 7), 
     *        adding the digit will overflow.
     *    If either condition is met, we trigger an immediate clamp intervention, returning 
     *    `INT_MAX` or `INT_MIN` based on our sign multiplier.
     */
    int myAtoi(string s) {
        int n = static_cast<int>(s.length());
        int i = 0;
        
        // Phase A: Eject leading whitespace anomalies from the stream
        while (i < n && s[static_cast<size_t>(i)] == ' ') {
            i++;
        }

        if (i == n) return 0;

        // Phase B: Evaluate sign presence markers
        int signMultiplier = 1;
        if (s[static_cast<size_t>(i)] == '-') {
            signMultiplier = -1;
            i++;
        } else if (s[static_cast<size_t>(i)] == '+') {
            i++;
        }

        int runningResult = 0;

        // Phase C: Extract contiguous numeric digits and manage numerical overflows
        while (i < n && s[static_cast<size_t>(i)] >= '0' && s[static_cast<size_t>(i)] <= '9') {
            int currentDigit = s[static_cast<size_t>(i)] - '0';

            // Proactive Overflow Guard: Inspect boundaries prior to computing matrix transformations
            if (runningResult > INT_MAX / 10 || (runningResult == INT_MAX / 10 && currentDigit > INT_MAX % 10)) {
                return (signMultiplier == 1) ? INT_MAX : INT_MIN;
            }

            runningResult = runningResult * 10 + currentDigit;
            i++;
        }

        return runningResult * signMultiplier;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of the input string s. 
 *                  The single pass cursor loop processes each index layout at most once, 
 *                  guaranteeing a linear runtime constraint footprint.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The mathematical transformations are verified completely in place 
 *                   on primitive register variables, eliminating dynamic memory allocations.
 */
int main() {
    string inputText;
    cout << "=== String to Integer (atoi) Engine Framework ===\n";
    cout << "Enter a textual string sequence to process (e.g., \"  -42 words\"):\n";
    if (!getline(cin, inputText)) return 1;

    Solution solver;
    cout << "\nInitializing sequential tokenizer states and processing numeric bits..." << endl;
    int parsedOutputResult = solver.myAtoi(inputText);

    cout << "\nParsed Integral Target Outcome: " << parsedOutputResult << "\n";

    return 0;
}