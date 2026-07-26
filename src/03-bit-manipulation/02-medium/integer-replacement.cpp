#include <iostream>

using namespace std;

/*
CORE LOGIC (BITWISE LEAST SIGNIFICANT BIT OPTIMIZATION TACTICS):
- The objective is to find the minimum number of replacements to reduce a given positive integer `n` to 1.
  - If `n` is even, we must replace `n` with `n / 2`.
  - If `n` is odd, we can replace `n` with either `n + 1` or `n - 1`.
- **Greedy Bit-Level Strategy**:
  - Even numbers offer no choice: we must perform a bitwise right-shift (`n >>= 1`).
  - Odd numbers end in a 1 bit (`...XX1`). We must decide whether adding or subtracting 1 simplifies the remaining binary structure faster.
    - If we subtract 1, the rightmost bit becomes 0 (`...XX0`), leaving the bits to its left unchanged.
    - If we add 1, a carry bit cascades upward, flipping adjacent 1 bits to 0s.
  - To minimize the total operations, our goal for odd numbers is to create as many trailing zeros as possible in the next step.
  - We look at the lowest two bits:
    - If `n` ends in `11` (binary representation of a number congruent to 3 mod 4), adding 1 transforms `11` into `100`, clearing two bits at once. Therefore, incrementing is generally optimal.
    - If `n` ends in `01` (binary representation of a number congruent to 1 mod 4), subtracting 1 transforms `01` into `00`. Therefore, decrementing is optimal.
  - **Edge Case Exception**: The number 3 ends in `11`, but incrementing it to 4 takes 3 steps (3 -> 4 -> 2 -> 1), while decrementing it to 2 takes only 2 steps (3 -> 2 -> 1). We catch this exception explicitly.
- **Data Type Safety**: We use a 64-bit integer (`long long`) to handle the variable state. This safely prevents any signed integer overflow vulnerabilities when `n` is equal to $2^{31} - 1$ and we add 1.

TIME COMPLEXITY: O(log N) - Each division eliminates a bit immediately, and each addition/subtraction creates an even number that will be divided in the very next step. The loop executes at most two operations per bit position.
SPACE COMPLEXITY: O(1) auxiliary - Evaluated completely in-place using scalar integer parameters.
*/
int integerReplacement(int n) {
    long long tempNum = n;
    int operationCount = 0;

    while (tempNum > 1) {
        if ((tempNum & 1) == 0) {
            // Even path: eliminate the lowest bit via an immediate shift
            tempNum >>= 1;
        } else if (tempNum == 3) {
            // Special baseline odd structural boundary fallback
            tempNum--;
        } else if ((tempNum & 3) == 3) {
            // Number ends in binary ...11 (3 mod 4): incrementing clears more bits
            tempNum++;
        } else {
            // Number ends in binary ...01 (1 mod 4): decrementing clears more bits
            tempNum--;
        }
        operationCount++;
    }

    return operationCount;
}

int main() {
    int n;
    cout << "Enter a positive integer value (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input. Please enter a positive signed integer." << endl;
        return 1;
    }

    int result = integerReplacement(n);
    cout << "\nThe minimum number of structural replacements required is: " << result << endl;

    return 0;
}