#include <iostream>

using namespace std;

/*
CORE LOGIC (HAMMING DISTANCE VIA XOR POPULATION COUNT):
- The problem asks us to calculate the Hamming Distance between two integers, which is the number of positions at which the corresponding bits are different.
- **Step 1: Isolate the Mismatched Bits using Bitwise XOR**:
  - The bitwise XOR ($\oplus$) operation outputs a 1 if and only if the two input bits are different ($0 \oplus 1 = 1$, $1 \oplus 0 = 1$), and a 0 if they are identical ($0 \oplus 0 = 0$, $1 \oplus 1 = 0$).
  - By computing `xorResult = x ^ y`, we generate a new integer where each set bit (1) represents a physical position where `x` and `y` differ.
- **Step 2: Count the Set Bits (Population Count)**:
  - The Hamming Distance is equal to the total number of 1 bits inside `xorResult`.
  - We clear the lowest set bit in each iteration using Brian Kernighan's trick: `xorResult = xorResult & (xorResult - 1)`. 
  - Each application flips the rightmost 1 bit to 0. The number of times we can perform this operation before the value becomes 0 is our answer.

TIME COMPLEXITY: O(1) overall / O(K) explicitly - Where K is the total number of differing bits between the two values. The execution takes at most 32 operations for standard integers.
SPACE COMPLEXITY: O(1) auxiliary - Computed completely in-place using localized registers.
*/
int hammingDistance(int x, int y) {
    // Isolate bits that differ between x and y
    int xorResult = x ^ y;
    int distance = 0;
    
    // Count the number of set bits using Brian Kernighan's algorithm
    while (xorResult > 0) {
        xorResult &= (xorResult - 1); // Clears the lowest set bit
        distance++;
    }
    
    return distance;
}

int main() {
    int x, y;
    cout << "Enter the first integer (x): ";
    if (!(cin >> x)) {
        cout << "Invalid integer format processing entry." << endl;
        return 1;
    }
    cout << "Enter the second integer (y): ";
    if (!(cin >> y)) {
        cout << "Invalid integer format processing entry." << endl;
        return 1;
    }

    int result = hammingDistance(x, y);
    cout << "\nThe Hamming Distance between " << x << " and " << y << " is: " << result << endl;

    return 0;
}