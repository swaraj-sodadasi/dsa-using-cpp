#include <iostream>

using namespace std;

/*
CORE LOGIC (COMBINATORIAL PERMUTATIONS):
- The problem asks us to count all numbers with unique digits in the range 0 <= x < 10^n.
- Edge Cases:
  - For n = 0, the range is 0 <= x < 1. Only 0 is valid. The answer is 1.
  - For n = 1, the range is 0 <= x < 10. All single-digit numbers (0-9) have unique digits. The answer is 10.
  - For n > 10, by the Pigeonhole Principle, any number with 11 or more digits must repeat at least one digit. Since we only have 10 unique digits (0-9), we can cap our maximum processing at n = 10.
- Combinatorial Strategy for a number with exactly 'd' digits (where d > 1):
  - **Choice for the first digit (most significant)**: It can be any digit from 1 to 9 (cannot be 0 to avoid leading zeroes). This leaves us with **9 choices**.
  - **Choice for the second digit**: It can be any digit from 0 to 9 except the one chosen for the first position. This leaves us with **9 choices**.
  - **Choice for the third digit**: Any digit from 0 to 9 except the two already used. This leaves us with **8 choices**.
  - Generalizing this, for the k-th digit position, we have `10 - (k - 1)` choices available.
- We accumulate these dynamic permutation combinations incrementally for each length from 1 up to n to compute the total global count.

TIME COMPLEXITY: O(1) - The maximum number of loop iterations is strictly bounded by the number of unique base-10 digits (n <= 10).
SPACE COMPLEXITY: O(1) - The matching is evaluated completely in-place using a few scalar variables.
*/
int countNumbersWithUniqueDigits(int n) {
    // If n is greater than 10, it will yield the exact same result as n = 10 due to the Pigeonhole Principle
    if (n > 10) {
        n = 10;
    }

    if (n == 0) return 1;

    // Base case for single-digit numbers (0-9)
    int totalCount = 10; 
    int uniqueDigitsProduct = 9;
    int availableChoices = 9;

    // Greedily append counts for numbers with digit lengths from 2 up to n
    for (int i = 2; i <= n; ++i) {
        uniqueDigitsProduct *= availableChoices;
        totalCount += uniqueDigitsProduct;
        availableChoices--;
    }

    return totalCount;
}

int main() {
    int n;
    cout << "Enter the value of exponent n (where range is 0 <= x < 10^n): ";
    cin >> n;

    if (n < 0) {
        cout << "Invalid input. Exponent n must be a non-negative integer." << endl;
        return 1;
    }

    int result = countNumbersWithUniqueDigits(n);
    cout << "\nTotal numbers with completely unique digits: " << result << endl;

    return 0;
}