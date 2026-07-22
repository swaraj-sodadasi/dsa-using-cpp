#include <iostream>

using namespace std;

/*
CORE LOGIC (BITWISE FLIP USING BIT-MASKING):
- The problem asks us to find the complement of a positive integer. The complement is formed by flipping all the bits 
  in its binary representation, but *only* up to its highest significant bit (we must ignore leading zeros).
- **The Core Trick (XOR Bit-Masking)**:
  - The bitwise XOR ($\oplus$) operation with a 1 bit flips the target bit ($0 \oplus 1 = 1$ and $1 \oplus 1 = 0$).
  - Therefore, if we can construct a bitmask that is a solid block of 1s of the exact same bit-length as `num`, 
    we can compute the complement instantly using `num ^ mask`.
  - For example: 
    If num  = 5 (binary: 101)
    We want a mask = 7 (binary: 111)
    Complement = 101 ^ 111 = 010 (which is 2)
- **Constructing the Mask Dynamically**:
  - We initialize an unsigned tracking variable `mask` to 1.
  - While `mask` is less than `num`, we shift it left and set its lowest bit to 1: `mask = (mask << 1) | 1`.
  - This expands the block of 1s until it completely covers the highest bit of `num`.
- **Boundary Optimization**: We use `unsigned int` for the mask tracking logic to safely prevent any signed 
  integer overflows when dealing with numbers close to maximum bit capacities.

TIME COMPLEXITY: O(log N) - The mask generation loop scales directly with the number of bits in the integer, 
                 running at most 31 times for standard positive 32-bit signed integers.
SPACE COMPLEXITY: O(1) auxiliary - Executed completely in-place within localized register states.
*/
int findComplement(int num) {
    if (num == 0) return 1;

    unsigned int mask = 1;
    // Expand the bitmask until it covers the highest significant bit of num
    while (mask < static_cast<unsigned int>(num)) {
        mask = (mask << 1) | 1;
    }

    // Flip the bits within the valid mask range using a bitwise XOR operation
    return num ^ static_cast<int>(mask);
}

int main() {
    int num;
    cout << "Enter a positive integer: ";
    if (!(cin >> num) || num < 0) {
        cout << "Invalid numeric input configuration entry." << endl;
        return 1;
    }

    int result = findComplement(num);
    cout << "\nThe complement value (with leading zeros ignored) is: " << result << endl;

    return 0;
}