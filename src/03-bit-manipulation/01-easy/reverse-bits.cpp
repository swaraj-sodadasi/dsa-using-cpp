#include <iostream>
#include <cstdint> // Required for fixed-width integer types like uint32_t

using namespace std;

/*
CORE LOGIC (UNSIGNED INTEGER BITWISE REVERSAL):
- The problem asks us to reverse the bits of a given 32-bit unsigned integer.
- **Bitwise Shifting Strategy**:
  1. We initialize a `result` variable to 0, which will store our reversed bits.
  2. We loop exactly 32 times because a standard 32-bit integer has 32 distinct positions.
  3. In each iteration, we prepare our `result` to receive a new bit by shifting it to the left: `result <<= 1`.
  4. We extract the lowest bit (rightmost bit) of our input `n` using the bitwise expression `n & 1`.
  5. We append this extracted bit to the empty position created in our `result` using a bitwise OR: `result |= (n & 1)`.
  6. We then shift the input `n` to the right by 1 position (`n >>= 1`) to bring the next bit into the rightmost spot for the next iteration.
  7. This processes the input from right-to-left and constructs the output from left-to-right, completely reversing the layout.

TIME COMPLEXITY: O(1) - The loop runs exactly 32 times regardless of the numerical magnitude of the input integer.
SPACE COMPLEXITY: O(1) auxiliary - Executed completely in-place using scalar integer trackers.
*/
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    
    for (int i = 0; i < 32; ++i) {
        result <<= 1;          // Shift the accumulated result to the left to open a new lower slot
        result |= (n & 1);     // Extract the current rightmost bit from n and insert it into the result
        n >>= 1;               // Shift the input integer right to bring the next bit to the 0th index position
    }
    
    return result;
}

int main() {
    uint32_t n;
    cout << "Enter a 32-bit unsigned integer: ";
    if (!(cin >> n)) {
        cout << "Invalid integer entry configuration." << endl;
        return 1;
    }

    uint32_t result = reverseBits(n);
    cout << "\nThe result after reversing all 32 bits is: " << result << endl;

    return 0;
}