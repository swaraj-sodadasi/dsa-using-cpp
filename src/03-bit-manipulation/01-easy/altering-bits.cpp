#include <iostream>

using namespace std;

/*
CORE LOGIC (BITWISE SHIFT XOR VALIDATION):
- The problem asks us to determine whether a positive integer has alternating bits (i.e., whether any two adjacent bits are always different, like 101010).
- **Step 1: Shift and Align Mismatches**:
  - Let the original number be `n` (e.g., n = 5, which is binary 101).
  - If we shift `n` right by 1 position (`n >> 1`), the binary representation becomes 010.
  - If the bits in `n` alternate perfectly, then every bit in `n` will be the exact opposite of the bit sitting directly adjacent to it. Shifting aligns these opposite pairs.
- **Step 2: Cancel Alternating States using Bitwise XOR**:
  - We execute `xorResult = n ^ (n >> 1)`.
  - Because every adjacent bit was an opposite pair, the XOR operation (which yields 1 for mismatched inputs) converts the alternating sequence into a solid block of 1s (e.g., 101 ^ 010 = 111).
- **Step 3: Validate the Full Block of 1s (All-Ones Predecessor Check)**:
  - If `xorResult` consists entirely of 1s (like 111), adding 1 to it will trigger a carry chain that turns it into a power of two with a single leading 1 followed by zeros (e.g., 111 + 1 = 1000).
  - We can check if `xorResult` is a solid block of 1s by evaluating `(xorResult & (xorResult + 1)) == 0`. If the result is 0, then the bits alternated perfectly.

TIME COMPLEXITY: O(1) - Evaluates instantly using low-level hardware bit instructions without any looping or iterative bit-shifting passes.
SPACE COMPLEXITY: O(1) auxiliary - Operates entirely within scalar tracking registers.
*/
bool hasAlternatingBits(int n) {
    // Shift right by 1 and XOR to transform alternating bits into an all-ones bitmask
    long long xorResult = n ^ (n >> 1);
    
    // Check if the resulting mask consists entirely of 1 bits using a predecessor boundary check
    return (xorResult & (xorResult + 1)) == 0;
}

int main() {
    int n;
    cout << "Enter a positive integer: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid entry. Please enter a positive signed integer." << endl;
        return 1;
    }

    if (hasAlternatingBits(n)) {
        cout << "\n" << n << " has alternating bits." << endl;
    } else {
        cout << "\n" << n << " does NOT have alternating bits." << endl;
    }

    return 0;
}