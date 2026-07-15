#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

/*
CORE LOGIC (LEAST COMMON MULTIPLE + BINARY SEARCH ON RANGE):
- A positive integer is considered "magical" if it is evenly divisible by either `a` or `b`.
- Given a number `mid`, the count of magical numbers less than or equal to `mid` can be found using the 
  Inclusion-Exclusion Principle:
  Total Magical Numbers = (mid / a) + (mid / b) - (mid / LCM(a, b))
  Where LCM is the Least Common Multiple of `a` and `b`, calculated as `LCM(a, b) = (a * b) / GCD(a, b)`.
- Because the count function is monotonically increasing with respect to `mid`, we can binary search 
  for the exact `mid` value that yields exactly `n` magical numbers.
- **Search Boundaries**:
  - `low`: The smallest possible magical number, which is `min(a, b)`.
  - `high`: An upper bound, which can be safely estimated as `static_cast<long long>(n) * min(a, b)`.
- When the count of magical numbers matches or exceeds `n`, `mid` could be our target answer, but we must continue 
  shrinking our search space to the left (`high = mid - 1`) to find the smallest value that maintains this condition.
- Finally, because the answer can be extremely large, the result must be wrapped modulo 10^9 + 7.

TIME COMPLEXITY: O(log(N * min(A, B))) - The binary search space halves in each step, and the GCD calculation takes O(log(min(A, B))) time.
SPACE COMPLEXITY: O(1) - The range search executes completely in-place using scalar integer variables.
*/
int nthMagicalNumber(int n, int a, int b) {
    long long MOD = 1'000'000'007;
    
    // Calculate the Least Common Multiple using std::gcd
    long long lcm = (static_cast<long long>(a) * b) / std::gcd(a, b);
    
    // Establish the binary search space bounds
    long long low = min(a, b);
    long long high = static_cast<long long>(n) * min(a, b);
    long long answer = low;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        
        // Inclusion-Exclusion Principle calculation
        long long magicalCount = (mid / a) + (mid / b) - (mid / lcm);
        
        if (magicalCount >= n) {
            answer = mid;       // Potential candidate found
            high = mid - 1;     // Try to find a smaller valid value on the left
        } else {
            low = mid + 1;      // Move search window right
        }
    }

    return static_cast<int>(answer % MOD);
}

int main() {
    int n, a, b;
    cout << "Enter the target index N: ";
    cin >> n;
    cout << "Enter divisor A: ";
    cin >> a;
    cout << "Enter divisor B: ";
    cin >> b;

    if (n <= 0 || a <= 0 || b <= 0) {
        cout << "Error: N, A, and B must be positive integers." << endl;
        return 1;
    }

    int result = nthMagicalNumber(n, a, b);
    cout << "\nThe " << n << "-th magical number is: " << result << endl;

    return 0;
}