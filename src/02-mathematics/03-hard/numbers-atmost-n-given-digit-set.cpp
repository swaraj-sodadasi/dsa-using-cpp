#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (DIGIT DP / COMBINATORIAL COUNTING):
- The problem asks us to find the count of positive integers that can be generated using a given set of string digits `D` such that the formed numbers are less than or equal to an integer `n`.
- Let `L` be the length of the string representation of `n`.
- We can divide the total valid combinations into two main structural sets:
  1. **Numbers with strictly fewer digits than L**:
     If a generated number has fewer digits than `n`, it is guaranteed to be smaller than `n`, regardless of what digits are used. For a number of length `i` (where `1 <= i < L`), each position can be independently filled by any digit in `D`. Thus, the combinations for length `i` is $|D|^i$. We sum this value up for all lengths from 1 to `L - 1`.
  2. **Numbers with exactly L digits**:
     We process positions from the most significant digit (left) to the least significant digit (right). For each position `i`:
     - Any digit in `D` that is *strictly smaller* than the corresponding digit in `n` can be placed at position `i`. Once a smaller digit is placed, all remaining `L - 1 - i` positions can be filled arbitrarily by any digit in `D` ($|D|^{L - 1 - i}$ variations).
     - If a digit in `D` matches the current digit of `n` *exactly*, we can place it and proceed to evaluate the next position (position `i + 1`) under structural constraints. If the current digit of `n` does not exist in `D`, this matching branch terminates completely.
     - If we successfully match all `L` digits up to the final position, it means the number `n` itself can be constructed using `D`, contributing exactly $+1$ to our total.

TIME COMPLEXITY: O(L * |D|) - Where L is the number of digits in `n` (at most 10 for a standard 32-bit signed integer) and |D| is the size of the digit set (at most 9). The loops execute in constant time boundaries.
SPACE COMPLEXITY: O(L) - The structural memory footprint is bounded strictly by the string conversions used for range validation.
*/
int atMostNGivenDigitSet(const vector<string>& digits, int n) {
    string s = to_string(n);
    int len = static_cast<int>(s.length());
    int numDigits = static_cast<int>(digits.size());
    int totalCount = 0;

    // Part 1: Count all valid numbers with lengths strictly less than the length of n
    for (int i = 1; i < len; ++i) {
        totalCount += static_cast<int>(pow(numDigits, i));
    }

    // Part 2: Count valid numbers that have the exact same number of digits as n
    for (int i = 0; i < len; ++i) {
        bool matchFound = false;
        
        for (const string& d : digits) {
            // If the element digit is smaller than the current digit in n
            if (d[0] < s[static_cast<size_t>(i)]) {
                totalCount += static_cast<int>(pow(numDigits, len - 1 - i));
            } 
            // If the element digit matches the digit in n exactly
            else if (d[0] == s[static_cast<size_t>(i)]) {
                matchFound = true;
                break; // Break the inner loop to move down to the next position tier
            }
        }

        // If no exact match was found for the current position, we cannot continue matching further prefixes
        if (!matchFound) {
            return totalCount;
        }
    }

    // If the loop finishes completely, it means we constructed the exact value of n using the digit set
    return totalCount + 1;
}

int main() {
    int numDigits;
    cout << "Enter the number of unique digits in the set: ";
    cin >> numDigits;

    if (numDigits <= 0) {
        cout << "Invalid digit set configuration size." << endl;
        return 1;
    }

    vector<string> digits(static_cast<size_t>(numDigits));
    cout << "Enter the unique digits as single-character strings (1-9) separated by spaces:\n";
    for (int i = 0; i < numDigits; ++i) {
        cin >> digits[static_cast<size_t>(i)];
    }

    // Sort digits to guarantee ascending order during prefix matching evaluations
    sort(digits.begin(), digits.end());

    int n;
    cout << "Enter the upper bound integer boundary value (N): ";
    cin >> n;

    if (n <= 0) {
        cout << "N must be a positive non-zero integer sequence threshold." << endl;
        return 1;
    }

    int result = atMostNGivenDigitSet(digits, n);
    cout << "\nThe number of positive integers <= " << n << " that can be written using the digit set is: " << result << endl;

    return 0;
}