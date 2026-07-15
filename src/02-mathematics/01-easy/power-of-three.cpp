#include <iostream>

using namespace std;

/*
CORE LOGIC (MAX-INT DIVISOR TRICK / CONSTANT TIME CHECK):
- A number N is a power of three if there exists an integer 'X' such that N = 3^X.
- Any integer less than or equal to 0 cannot be a power of three.
- In standard C++ for a 32-bit signed integer (`int`), the maximum possible value is 2,147,483,647.
- The largest integer power of three that fits within this 32-bit signed integer limit is 3^19 = 1,162,261,467.
- Because 3 is a prime number, the only prime factors of 3^19 are 3. Therefore, any integer N that is a power of three *must* be a clean divisor of 3^19 with no remainder.
- This allows us to determine if N is a power of three in O(1) time without looping or using floating-point log calculations (which can suffer from precision errors). We simply check: `1162261467 % n == 0`.

TIME COMPLEXITY: O(1) - Evaluates a single arithmetic modulo operation in constant time.
SPACE COMPLEXITY: O(1) - Runs completely in-place using zero auxiliary storage states.
*/
bool isPowerOfThree(int n) {
    // 1162261467 is exactly 3^19, the maximum power of 3 within the 32-bit signed integer boundary
    return n > 0 && 1162261467 % n == 0;
}

int main() {
    int n;
    cout << "Enter an integer: ";
    cin >> n;

    if (isPowerOfThree(n)) {
        cout << n << " is a power of three." << endl;
    } else {
        cout << n << " is NOT a power of three." << endl;
    }

    return 0;
}