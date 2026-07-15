#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (TRIAL DIVISION OPTIMIZATION UP TO sqrt(N)):
- The problem asks us to find the k-th factor of a positive integer `n`, sorted in ascending order. If there are fewer than `k` factors, we return -1.
- A naive linear scan from 1 up to `n` takes O(n) time, which is inefficient for large numbers.
- Instead, we optimize using the property that factors always appear in pairs: if `i` is a factor of `n`, then `n / i` is also a factor.
- One of the factors in the pair must be less than or equal to `sqrt(n)`, while the matching factor must be greater than or equal to `sqrt(n)`.
- We can iterate from `i = 1` up to `i * i <= n`:
  - If `i` divides `n` evenly (`n % i == 0`), `i` is a valid factor. We store it in a container.
  - To prevent duplicates when `n` is a perfect square (where `i == n / i`), we check if `i != n / i` before storing the paired factor.
- After extracting all factors via trial division, the first half (smaller factors) will naturally be in ascending order, but the second half (larger paired factors) will be in descending order. 
- We sort the accumulated factors vector to ensure they are fully sorted in ascending order.
- Finally, if `k` is within the valid bounds of the sorted vector, we return the factor at index `k - 1`. Otherwise, we return -1.

TIME COMPLEXITY: O(sqrt(N) + F log F) - Where F is the total number of factors of N. The trial division loop takes O(sqrt(N)) operations, and sorting the factors takes O(F log F) time. For standard 32-bit integers, F is very small (maximum 1344 factors).
SPACE COMPLEXITY: O(F) - Storage space in our auxiliary vector is proportional to the total number of factors found.
*/
int getKthFactor(int n, int k) {
    vector<int> factors;

    // Trial division up to the square root of n
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            factors.push_back(i); // Store the smaller factor
            
            // Store the paired larger factor if it's unique
            if (i != n / i) {
                factors.push_back(n / i);
            }
        }
    }

    // Sort factors in ascending order
    sort(factors.begin(), factors.end());

    // Check if the k-th factor exists (1-indexed mapping to 0-indexed vector)
    if (k > 0 && k <= static_cast<int>(factors.size())) {
        return factors[static_cast<size_t>(k - 1)];
    }

    return -1;
}

int main() {
    int n, k;
    cout << "Enter the integer N: ";
    cin >> n;
    cout << "Enter the target factor index K: ";
    cin >> k;

    if (n <= 0 || k <= 0) {
        cout << "Error: N and K must be positive integers." << endl;
        return 1;
    }

    int result = getKthFactor(n, k);

    if (result != -1) {
        cout << "\nThe " << k << "-th factor of " << n << " is: " << result << endl;
    } else {
        cout << "\n" << n << " has fewer than " << k << " factors. Result: -1" << endl;
    }

    return 0;
}