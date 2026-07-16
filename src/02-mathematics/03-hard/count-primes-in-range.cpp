#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/*
CORE LOGIC (SEGMENTED SIEVE FOR LARGE RANGES):
- The problem asks us to count the number of prime numbers in a given range [L, R].
- A standard Sieve of Eratosthenes requires O(R) space, which becomes completely impractical or causes Out-Of-Memory errors when R is very large (e.g., up to 10^9 or 10^12).
- **Segmented Sieve Approach**:
  1. We observe that any composite number in the range [L, R] must have at least one prime factor less than or equal to sqrt(R).
  2. We first find all prime numbers up to sqrt(R) using a standard simple Sieve of Eratosthenes.
  3. We create a boolean array `isPrime` of size (R - L + 1) to represent the numbers in the range [L, R], where index `i` maps to the number `L + i`.
  4. For each prime `p` found in the base sieve, we calculate its first multiple that is greater than or equal to `L`. Let this be `startValue`:
     $$ startValue = \max(p \times p, \lfloor \frac{L + p - 1}{p} \rfloor \times p) $$
  5. We then iterate from `startValue` up to `R` in steps of `p`, marking all these multiples as composite (`false`) in our segmented range map.
  6. Finally, we count the remaining `true` values in our segment array. (Special check: if `L == 1`, we explicitly mark 1 as non-prime).

TIME COMPLEXITY: O((R - L + 1) log log R + sqrt(R) log log sqrt(R)) - Evaluating only the relevant multiples within the segment window keeps the execution runtime linear to the window size.
SPACE COMPLEXITY: O(sqrt(R) + (R - L + 1)) - Memory allocation scales strictly with the size of the range segment rather than the absolute value of R, enabling huge range evaluations.
*/

// Helper function to generate all base primes up to a maximum value limit using standard Sieve
vector<int> simpleSieve(int limit) {
    vector<bool> isPrime(static_cast<size_t>(limit) + 1, true);
    vector<int> primes;
    
    if (limit >= 0) isPrime[0] = false;
    if (limit >= 1) isPrime[1] = false;
    
    for (int p = 2; p * p <= limit; ++p) {
        if (isPrime[static_cast<size_t>(p)]) {
            for (int i = p * p; i <= limit; i += p) {
                isPrime[static_cast<size_t>(i)] = false;
            }
        }
    }
    
    for (int p = 2; p <= limit; ++p) {
        if (isPrime[static_cast<size_t>(p)]) {
            primes.push_back(p);
        }
    }
    return primes;
}

long long countPrimesInRange(long long L, long long R) {
    if (L > R || R < 2) return 0;
    if (L < 2) L = 2; // Prime numbers start at 2

    // Step 1: Find all primes up to sqrt(R)
    int limit = static_cast<int>(sqrt(R));
    vector<int> basePrimes = simpleSieve(limit);

    // Step 2: Initialize segmented sieve array mapping to the window range [L, R]
    long long rangeSize = R - L + 1;
    vector<bool> isPrime(static_cast<size_t>(rangeSize), true);

    // Step 3: Use base primes to cross off composite multiples inside the segment window
    for (int p : basePrimes) {
        // Find the smallest multiple of p that is >= L
        long long startValue = (L + p - 1) / p * p;
        
        // A prime cannot cross itself out; start crossing off from its square if it falls below L
        if (startValue < static_cast<long long>(p) * p) {
            startValue = static_cast<long long>(p) * p;
        }

        // Cross off all multiples of p in the range [L, R]
        for (long long j = startValue; j <= R; j += p) {
            isPrime[static_cast<size_t>(j - L)] = false;
        }
    }

    // Step 4: Count remaining primes within the window segment
    long long primeCount = 0;
    for (long long i = 0; i < rangeSize; ++i) {
        if (isPrime[static_cast<size_t>(i)]) {
            primeCount++;
        }
    }

    return primeCount;
}

int main() {
    long long L, R;
    cout << "Enter the lower bound range integer (L): ";
    cin >> L;
    cout << "Enter the upper bound range integer (R): ";
    cin >> R;

    if (L < 1 || R < 1) {
        cout << "Range bounds must be positive non-zero integers." << endl;
        return 1;
    }

    long long totalPrimes = countPrimesInRange(L, R);
    cout << "\nThe total number of prime numbers within the range [" << L << ", " << R << "] is: " << totalPrimes << endl;

    return 0;
}