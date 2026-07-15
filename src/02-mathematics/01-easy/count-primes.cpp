#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (SIEVE OF ERATOSTHENES):
- The objective is to count the number of prime numbers strictly less than a given non-negative integer `n`.
- We initialize a boolean tracking array `isPrime` of size `n` with all true values, assuming every number is prime initially. We mark indices 0 and 1 as false since 0 and 1 are not prime.
- We loop from `i = 2` up to `sqrt(n)` (i.e., `i * i < n`). If `isPrime[i]` is true, it means `i` is a prime number. 
- For every prime `i`, we mark all of its cross-multiples starting from `i * i` up to `n` as false (`isPrime[j] = false`), because they are composite numbers. Starting the inner loop at `i * i` avoids redundant evaluations of smaller factors already handled by smaller primes.
- Finally, we make a linear pass over the boolean array from 2 up to `n - 1` to count the remaining entries that are still marked true.

TIME COMPLEXITY: O(N log(log N)) - The inner loop executes a harmonic-like sequence of prime reciprocal sums, resulting in a near-linear asymptotic complexity layer.
SPACE COMPLEXITY: O(N) - A contiguous bitwise or boolean tracking vector structure of size N is allocated to record the primality state.
*/
int countPrimes(int n) {
    if (n <= 2) return 0;

    vector<bool> isPrime(static_cast<size_t>(n), true);
    isPrime[0] = false;
    isPrime[1] = false;

    for (int i = 2; i * i < n; ++i) {
        if (isPrime[static_cast<size_t>(i)]) {
            // Unmark cross-multiples starting from i * i to prevent duplicate indexing
            for (int j = i * i; j < n; j += i) {
                isPrime[static_cast<size_t>(j)] = false;
            }
        }
    }

    int primeCount = 0;
    for (int i = 2; i < n; ++i) {
        if (isPrime[static_cast<size_t>(i)]) {
            primeCount++;
        }
    }

    return primeCount;
}

int main() {
    int n;
    cout << "Enter a non-negative integer N: ";
    cin >> n;

    int result = countPrimes(n);
    cout << "The total number of prime numbers strictly less than " << n << " is: " << result << endl;

    return 0;
}