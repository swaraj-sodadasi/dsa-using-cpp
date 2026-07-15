#include <iostream>

using namespace std;

/*
CORE LOGIC (BINARY EXPONENTIATION / EXPONENTIATION BY SQUARING):
- Computing x^n by multiplying x by itself n times takes O(n) time, which is inefficient for large exponents.
- Instead, we use binary exponentiation (exponentiation by squaring), which reduces the time complexity to O(log n).
- The key recurrence relations are:
    - If n is even: x^n = (x * x)^(n / 2)
    - If n is odd:  x^n = x * (x * x)^((n - 1) / 2)
- Handling Negative Exponents:
    - If n < 0, x^n is equivalent to (1 / x)^(-n).
    - To avoid overflow when n is INT_MIN (since negating INT_MIN exceeds the maximum positive value of a signed 32-bit int), we copy the exponent to a `long long` variable.
- We iterate while the exponent is greater than 0:
    - If the current bit of the exponent is odd, we multiply the result by the current base.
    - We then square the base (`x = x * x`) and divide the exponent by 2 (`n = n / 2`).

TIME COMPLEXITY: O(log N) - The exponent is halved in each step of the bit-shifting / division sequence.
SPACE COMPLEXITY: O(1) - The calculation runs entirely in-place using scalar variables.
*/
double myPow(double x, int n) {
    long long N = n; // Use long long to prevent integer overflow if n == INT_MIN
    
    // Handle negative exponent transformation
    if (N < 0) {
        x = 1.0 / x;
        N = -N;
    }

    double result = 1.0;
    double currentProduct = x;

    while (N > 0) {
        // If the lowest bit is set (exponent is odd), multiply the result by the current base value
        if (N % 2 == 1) {
            result *= currentProduct;
        }
        
        // Square the base for the next binary place value level
        currentProduct *= currentProduct;
        N /= 2;
    }

    return result;
}

int main() {
    double x;
    int n;
    
    cout << "Enter the base value (x): ";
    cin >> x;
    cout << "Enter the integer exponent (n): ";
    cin >> n;

    // Edge case handling: 0 raised to a negative power represents division by zero
    if (x == 0.0 && n < 0) {
        cout << "\nError: Division by zero (0 raised to a negative power)." << endl;
        return 1;
    }

    double result = myPow(x, n);
    cout << "\nResult of " << x << "^" << n << " is: " << result << endl;

    return 0;
}