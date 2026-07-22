#include <iostream>

using namespace std;

/*
CORE LOGIC (BITWISE STRUCTURAL ISOLATION):
- The problem asks us to determine whether a positive integer has alternating bits (i.e., adjacent bits have different values, such as 10101).
- **Alternative Solution via Structural Bit-Isolation**:
  - Instead of looking at the entire layout at once using a global shift-and-XOR approach, we can isolate and verify individual bit pairings sequentially from right to left.
  - In a perfectly alternating binary number, the last bit (`n & 1`) and the second-to-last bit (`(n >> 1) & 1`) must never be equal.
  - We isolate the lower two bits in each cycle. If their values match (`(n & 1) == ((n >> 1) & 1)`), the alternating structure is broken, and we return `false` early.
  - If they do not match, we discard the lowest bit by shifting right (`n >>= 1`) and loop until the remaining bit structure reduces to 0.
- **Trade-off Analysis**:
  - While this method requires a loop instead of completing in a single step like the shift-and-XOR version, it performs explicit structural checking. This avoids any intermediate numeric overflows and provides early-exit capabilities the moment an invalid adjacent pattern is discovered.

TIME COMPLEXITY: O(log N) - The loop runs at most proportional to the number of bits in the integer (up to 32 steps for a standard 32-bit integer).
SPACE COMPLEXITY: O(1) auxiliary - The structural status is tracked completely in-place within scalar processing registers.
*/
bool hasAlternatingBits(int n) {
    // Keep shifting and evaluating until we isolate the final remaining structural bits
    while (n > 0) {
        int currentBit = n & 1;         // Isolate the rightmost bit
        int nextBit = (n >> 1) & 1;     // Isolate the adjacent bit immediately to its left
        
        // If two adjacent bits share the identical state value, the alternating pattern is broken
        if (currentBit == nextBit) {
            return false;
        }
        
        n >>= 1; // Drop the validated lower bit to inspect the next overlapping pair
    }
    
    return true;
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