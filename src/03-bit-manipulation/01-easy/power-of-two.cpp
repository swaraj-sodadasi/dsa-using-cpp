#include <iostream>

using namespace std;

/*
CORE LOGIC (BITWISE AND WITH PREDECESSOR CHECK):
- The problem asks us to determine whether a given integer is a power of two.
- An integer is a power of two if it is strictly greater than zero and its binary representation contains exactly one set bit (a single 1 bit).
  - For example: 4 (binary 100), 8 (binary 1000), 16 (binary 10000).
- **The Predecessor Bitwise Property**:
  - When we subtract 1 from a power of two, its single set bit flips to 0, and all the lower bits to its right turn into 1s.
  - For example:
    If n     = 8  (binary: 1000)
    Then n-1 = 7  (binary: 0111)
  - If we perform a bitwise AND between these two values (`n & (n - 1)`), the matching bits cancel out perfectly, resulting in 0:
    1000 & 0111 = 0000
- For any integer that is *not* a power of two, its binary form has more than one set bit, so `n & (n - 1)` will leave those extra bits intact and return a non-zero value.
- **Boundary Constraint**: Non-positive numbers (numbers <= 0) can never be powers of two. We filter them out early using an explicit check (`n > 0`).

TIME COMPLEXITY: O(1) - The check completes instantly via hardware bit operations, without any iterations or recursive shifting loops.
SPACE COMPLEXITY: O(1) auxiliary - No extra memory allocation is generated; it runs entirely within scalar register values.
*/
bool isPowerOfTwo(int n) {
    // A power of two must be strictly greater than 0, and n & (n - 1) must evaluate to exactly 0
    return (n > 0) && ((n & (n - 1)) == 0);
}

int main() {
    int n;
    cout << "Enter an integer to evaluate: ";
    if (!(cin >> n)) {
        cout << "Invalid input. Please enter a valid signed integer." << endl;
        return 1;
    }

    if (isPowerOfTwo(n)) {
        cout << "\n" << n << " is a power of two." << endl;
    } else {
        cout << "\n" << n << " is NOT a power of two." << endl;
    }

    return 0;
}