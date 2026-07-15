#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (MODULAR EXPONENTIATION WITH EULER'S PROPERTY / EXPONENT DECOMPOSITION):
- The problem asks us to compute $a^b \pmod{1337}$, where $b$ is an extremely large positive integer 
  represented as an array of individual digits.
- Because $b$ can be thousands of digits long, we cannot compute or store it directly as a standard integer. 
  Instead, we can break down the exponent using properties of exponents and modular arithmetic:
  $$ a^{[1, 2, 3]} = a^{12 \times 10 + 3} = (a^{12})^{10} \times a^3 $$
- This mathematical structure allows us to process the array $b$ recursively or iteratively. For each digit $d$ in $b$:
  1. We raise our current accumulated result to the power of 10.
  2. We multiply that value by $a^d$.
  3. Every intermediate calculation is wrapped modulo 1337 to keep numbers small and prevent integer overflow.
- **Modular Exponentiation Helper (`modPow`)**: We implement a fast power function using binary exponentiation 
  (exponentiation by squaring) to efficiently calculate $(base^{exp}) \pmod{1337}$ in $O(\log(exp))$ time.
- Note: While Euler's Totient Theorem ($\phi(1337) = 1140$) can be used to reduce the large exponent array beforehand, 
  the iterative digit-by-digit modular expansion approach is highly elegant, bypasses array-wide division, 
  and performs dynamically in linear time proportional to the length of the digit array.

TIME COMPLEXITY: O(M) - Where M is the number of elements (digits) in array `b`. For each digit, we execute a fixed 
                 modular exponentiation to the power of 10, which runs in a small, bounded number of operations.
SPACE COMPEXITY: O(1) auxiliary - The computation runs iteratively using only a few scalar state tracking variables.
*/

// Helper function to calculate (base^exp) % 1337 using binary exponentiation
int modPow(int base, int exp, int mod = 1337) {
    base %= mod;
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int superPow(int a, const vector<int>& b) {
    int result = 1;
    int mod = 1337;

    for (int digit : b) {
        // Step 1: Raise the current result to the power of 10
        // Step 2: Multiply by a raised to the power of the current digit
        result = (modPow(result, 10, mod) * modPow(a, digit, mod)) % mod;
    }

    return result;
}

int main() {
    int a;
    cout << "Enter the base value (a): ";
    cin >> a;

    int size;
    cout << "Enter the number of digits in the large exponent array (b): ";
    cin >> size;

    if (size <= 0) {
        cout << "Invalid exponent size." << endl;
        return 1;
    }

    vector<int> b(size);
    cout << "Enter the digits of exponent b separated by space (e.g., for 4321 enter 4 3 2 1): ";
    for (int i = 0; i < size; ++i) {
        cin >> b[i];
        if (b[i] < 0 || b[i] > 9) {
            cout << "Invalid input. Each array element must be a single digit between 0 and 9." << endl;
            return 1;
        }
    }

    int answer = superPow(a, b);
    cout << "\nThe result of a^b % 1337 is: " << answer << endl;

    return 0;
}