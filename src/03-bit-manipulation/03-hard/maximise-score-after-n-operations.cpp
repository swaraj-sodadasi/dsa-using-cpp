#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BITMASK DP WITH GREATEST COMMON DIVISOR MATH):
- The problem asks us to maximize the score after performing N operations on an array of size 2*N. 
  In the i-th operation (1-indexed), we choose two elements, remove them, and receive a score equal to:
  $$ i \times \text{gcd}(x, y) $$
- **State Representation via Bitmasks**:
  - The array size is at most 14 (since N <= 7). This allows us to represent the state of available and 
    removed elements using a 14-bit integer `mask`.
  - A bit value of 1 at the $j$-th position indicates that `nums[j]` has been used, while 0 means it remains available.
- **Dynamic Programming Array Mapping**:
  - We use a 1D `dp` array of size $2^{2N}$. `dp[mask]` stores the maximum score achievable given that the elements 
    indicated by the set bits in `mask` have already been cleared from the array.
- **State Transition Strategy**:
  - The number of set bits in `mask` tells us exactly which operation we are currently on. 
    Specifically, `currentOperation = (number of set bits / 2) + 1`.
  - For each `mask` from 0 up to $2^{2N} - 1$, we pick any two available elements `j` and `k` (where the bits at 
    positions `j` and `k` are 0).
  - We simulate picking these two elements by transitioning to `nextMask = mask | (1 << j) | (1 << k)`.
  - The new score is calculated as `dp[mask] + currentOperation * gcd(nums[j], nums[k])`. We update `dp[nextMask]` 
    if this path yields a higher score than previously recorded.

TIME COMPLEXITY: O(4^N * N^2) - There are $2^{2N}$ total states (which is $4^N$). At each state, we find pairs of 
                 available elements using a nested loop running up to $2N$ times. Calculating GCD takes logarithmic time.
SPACE COMPLEXITY: O(2^2N) = O(4^N) auxiliary - The size of the DP array needed to store the optimal scoring states 
                  for each unique subset combination.
*/

// Helper function to calculate the greatest common divisor using Euclidean algorithm
int calculateGcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int maxScore(const vector<int>& nums) {
    size_t n = nums.size();
    size_t totalStates = 1ULL << n;
    
    // dp[mask] stores the maximum score achieved for the subset of elements already picked
    vector<int> dp(totalStates, 0);
    
    // Precompute GCD values for all unique pairs to avoid redundant arithmetic calculations inside loops
    vector<vector<int>> gcdValues(n, vector<int>(n, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            int val = calculateGcd(nums[i], nums[j]);
            gcdValues[i][j] = val;
            gcdValues[j][i] = val;
        }
    }

    // Iterate through every subset configuration mask state
    for (size_t mask = 0; mask < totalStates; ++mask) {
        // Count the number of bits set to 1 in the current state mask
        int bitsSet = 0;
        for (size_t i = 0; i < n; ++i) {
            if ((mask & (1ULL << i)) != 0) {
                bitsSet++;
            }
        }

        // Elements are always picked in pairs, so structural transitions happen on even counts
        if (bitsSet % 2 != 0) continue;

        // Determine the index of the operation we are about to execute (1-indexed)
        int currentOperation = (bitsSet / 2) + 1;

        // Try picking any pair of elements that are still available (bits are 0)
        for (size_t j = 0; j < n; ++j) {
            if ((mask & (1ULL << j)) == 0) {
                for (size_t k = j + 1; k < n; ++k) {
                    if ((mask & (1ULL << k)) == 0) {
                        size_t nextMask = mask | (1ULL << j) | (1ULL << k);
                        
                        int incrementalScore = currentOperation * gcdValues[j][k];
                        int totalNewScore = dp[mask] + incrementalScore;
                        
                        if (totalNewScore > dp[nextMask]) {
                            dp[nextMask] = totalNewScore;
                        }
                    }
                }
            }
        }
    }

    // The final answer is stored at the state where all elements have been selected (all bits are 1)
    return dp[totalStates - 1];
}

int main() {
    int totalElements;
    cout << "Enter the total number of elements (must be an even integer, maximum 14): ";
    if (!(cin >> totalElements) || totalElements <= 0 || totalElements % 2 != 0 || totalElements > 14) {
        cout << "Invalid input sizing configuration. Element count must be an even number between 2 and 14." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(totalElements));
    cout << "Enter the integer elements separated by spaces:\n";
    for (int i = 0; i < totalElements; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int result = maxScore(nums);
    cout << "\nThe maximum total score attainable after completing all operations is: " << result << endl;

    return 0;
}