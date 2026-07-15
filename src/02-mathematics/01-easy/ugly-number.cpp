#include <iostream>

using namespace std;

/*
CORE LOGIC (PRIME FACTOR DECOMPOSITION):
- An ugly number is a positive integer whose prime factors are strictly limited to 2, 3, and 5.
- By definition, non-positive integers (less than or equal to 0) cannot be ugly numbers.
- To check if a number is ugly, we systematically divide it by the allowed prime factors (2, 3, and 5) 
  for as long as the number remains evenly divisible by them.
- If the number is successfully reduced down to 1 after stripping away all factors of 2, 3, and 5, 
  it means it had no other prime factors, and it is an ugly number.
- If the final value is greater than 1, it means the number contains other prime factors (e.g., 7, 11, 13), 
  so it is not an ugly number.

TIME COMPLEXITY: O(log N) - The number of divisions is proportional to the number of times 2, 3, and 5 divide N.
SPACE COMPLEXITY: O(1) - The decomposition runs in-place using the input storage layer with zero heap footprint.
*/
bool isUgly(int n) {
    // Edge case: numbers less than or equal to 0 are not ugly numbers
    if (n <= 0) {
        return false;
    }

    // Repeatedly divide by 2, 3, and 5 while divisible
    const int allowedFactors[] = {2, 3, 5};
    for (int factor : allowedFactors) {
        while (n % factor == 0) {
            n /= factor;
        }
    }

    // If reduced to 1, all prime factors were within {2, 3, 5}
    return n == 1;
}

int main() {
    int n;
    cout << "Enter an integer: ";
    cin >> n;

    if (isUgly(n)) {
        cout << n << " is an ugly number." << endl;
    } else {
        cout << n << " is NOT an ugly number." << endl;
    }

    return 0;
}