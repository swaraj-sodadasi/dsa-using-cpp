#include <iostream>

using namespace std;

/*
CORE LOGIC (HALF-ADDER LOGIC USING XOR AND BITWISE SHIFTS):
- The problem asks us to compute the sum of two integers, `a` and `b`, without using the standard arithmetic operators `+` or `-`.
- **Digital Logic Emulation (Half-Adder)**:
  - We can break down addition into two distinct, parallel operations using bitwise logic:
    1. **Sum without carry**: The bitwise XOR ($\oplus$) operation acts exactly like a binary addition where carries are ignored ($0 \oplus 0 = 0$, $0 \oplus 1 = 1$, $1 \oplus 0 = 1$, and $1 \oplus 1 = 0$). We compute this temporary sum using `a ^ b`.
    2. **Carry generation**: A carry bit is generated at a specific position if and only if both bits being added are 1 ($1 \text{ AND } 1 = 1$). We isolate these positions using the bitwise expression `a & b`.
  - **Propagating the Carry**:
    - The generated carries must be added to the next column to the left. We achieve this by shifting the carry bits left by 1 position: `(a & b) << 1`.
  - **Iterative Reduction**:
    - We repeat this process, setting `a` to the partial sum (`a ^ b`) and `b` to the shifted carries (`(a & b) << 1`).
    - The loop runs until the carry variable `b` reduces to 0, meaning all carries have been completely absorbed into the sum.
- **Data Type Safety**: We use `unsigned int` for the shift tracking logic to safely avoid any undefined behavior associated with shifting signed negative numbers leftward.

TIME COMPLEXITY: O(1) - The loop runs at most 32 times for standard 32-bit integer layouts, as the carry bits shift left by 1 position each iteration and will eventually clear to 0.
SPACE COMPLEXITY: O(1) auxiliary - Operates entirely in-place within scalar tracking registers.
*/
int getSum(int a, int b) {
    // Keep propagating carries until there are no remaining carries left to add
    while (b != 0) {
        // Calculate the temporary sum by adding bits without handling any carries
        int temporarySum = a ^ b;
        
        // Calculate the carry bits, casting to unsigned int to handle negative bit shifting safely
        int carry = static_cast<int>((static_cast<unsigned int>(a & b)) << 1);
        
        // Update variables for the next iteration step
        a = temporarySum;
        b = carry;
    }
    
    return a;
}

int main() {
    int a, b;
    cout << "Enter the first integer (a): ";
    if (!(cin >> a)) {
        cout << "Invalid integer numeric input format entry." << endl;
        return 1;
    }
    cout << "Enter the second integer (b): ";
    if (!(cin >> b)) {
        cout << "Invalid integer numeric input format entry." << endl;
        return 1;
    }

    int result = getSum(a, b);
    cout << "\nThe calculated arithmetic sum of " << a << " and " << b << " is: " << result << endl;

    return 0;
}