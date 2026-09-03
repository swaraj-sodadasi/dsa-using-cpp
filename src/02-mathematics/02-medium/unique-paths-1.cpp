#include <iostream>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (COMBINATORICS / BINOMIAL COEFFICIENTS nCr):
- A robot starts at the top-left corner of an `m x n` grid and wants to reach the bottom-right corner. 
  The robot can only move either down or right at any point in time.
- To reach the destination, the robot must make exactly `m - 1` downward moves and `n - 1` rightward moves, 
  regardless of the path taken. The total number of moves is always `(m - 1) + (n - 1) = m + n - 2`.
- The problem then reduces to choosing which of these total moves will be down moves (or right moves). 
  Mathematically, this is the number of combinations of choosing `k` items out of `N` total items:
  $$ \binom{N}{k} = \frac{N!}{k!(N-k)!} $$
  Where:
  - $N = m + n - 2$
  - $k = \min(m - 1, n - 1)$ (choosing the minimum optimizes the loop iteration bounds).
- We compute the binomial coefficient $\binom{N}{k}$ iteratively to avoid integer overflow, multiplying and dividing 
  step-by-step: `result = result * (N - i) / (i + 1)`.

TIME COMPLEXITY: O(min(M, N)) - The calculation loops exactly `k` times, matching the smaller direction bound.
SPACE COMPLEXITY: O(1) - The calculation runs completely in-place using a single accumulation variable.
*/
int uniquePaths(int m, int n) {
    // Total moves needed
    int N = m + n - 2;
    // Choose the smaller value between down moves and right moves to minimize iterations
    int k = min(m - 1, n - 1);
    
    long long result = 1; // Use long long to prevent intermediate value overflow
    
    for (int i = 0; i < k; ++i) {
        result = result * (N - i) / (i + 1);
    }
    
    return static_cast<int>(result);
}

int main() {
    int m, n;
    cout << "Enter the grid dimensions (m rows and n columns): ";
    cin >> m >> n;

    if (m <= 0 || n <= 0) {
        cout << "Invalid grid boundaries. Dimensions must be positive integers." << endl;
        return 1;
    }

    int paths = uniquePaths(m, n);
    cout << "\nThe total number of unique paths from top-left to bottom-right is: " << paths << endl;

    return 0;
}