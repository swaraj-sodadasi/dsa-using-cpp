#include <iostream>
#include <cstdint> // Required for fixed-width integer types like uint32_t

using namespace std;

/*
CORE LOGIC (BRIAN KERNIGHAN’S ALGORITHM):
- The problem asks us to calculate the Hamming Weight (number of 1 bits) of an unsigned integer.
- The key observation is that executing the operation `n & (n - 1)` clears the lowest set bit (rightmost 1) of `n` to 0.
- For example:
  If n     = 12 (binary: 1100)
  Then n-1 = 11 (binary: 1011)
  n & (n-1)= 1100 & 1011 = 1000 (The rightmost 1 has been cleared!)
- Instead of checking all 32 bits sequentially using generic right-shifts, this algorithm jumps directly from one set bit to the next.
- We loop, clearing one bit at a time, and increment a counter until `n` reduces to 0.

TIME COMPLEXITY: O(1) overall / O(K) explicitly - Where K is the exact number of set bits (1s) inside the integer. 
                 In the absolute worst case (all bits set), it takes at most 32 operations for a standard 32-bit unsigned integer.
SPACE COMPLEXITY: O(1) auxiliary - Runs completely in-place using only scalar state elements.
*/
int hammingWeight(uint32_t n) {
    int count = 0;
    
    while (n > 0) {
        n = n & (n - 1); // Clears the lowest set bit
        count++;
    }
    
    return count;
}

int main() {
    uint32_t n;
    cout << "Enter an unsigned integer: ";
    if (!(cin >> n)) {
        cout << "Invalid numeric parsing bounds entry." << endl;
        return 1;
    }

    int result = hammingWeight(n);
    cout << "\nThe Hamming Weight (Number of 1 bits) is: " << result << endl;

    return 0;
}