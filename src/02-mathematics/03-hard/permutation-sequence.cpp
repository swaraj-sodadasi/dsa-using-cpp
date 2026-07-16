#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
CORE LOGIC (FACTORIAL NUMBER SYSTEM / CANTOR EXPANSION):
- Finding the k-th permutation of $n$ elements by generating all permutations sequentially would result 
  in a highly inefficient $O(n!)$ runtime.
- Instead, we can determine the digits of the k-th permutation directly using the **Factorial Number System** 
  (also known as Cantor Expansion).
- For a set of $n$ elements, the total number of unique permutations is $n!$. If we fix the first digit, 
  there are exactly $(n - 1)!$ permutations for the remaining positions.
- **Iterative Digit Extraction Steps**:
  1. Convert $k$ from a 1-indexed to a 0-indexed offset by subtracting 1 (`k--`).
  2. For the first position, the block index can be found via: `index = k / (n - 1)!`.
  3. The digit at `numbers[index]` is chosen as the next character of our sequence.
  4. Remove that digit from our available `numbers` pool so it cannot be reused.
  5. Update $k$ to track the position within the sub-block: `k %= (n - 1)!`.
  6. Repeat this process for the remaining slots until all positions are filled.

TIME COMPLEXITY: $O(n^2)$ - We run an outer loop $n$ times. Within each loop, removing an element from a 
                 vector takes linear time proportional to the remaining elements. Given $n \le 9$, this is exceptionally fast.
SPACE COMPLEXITY: $O(n)$ - Storage space is minimized to tracking the numeric character strings and the factorial array layout.
*/
string getPermutation(int n, int k) {
    int factorial = 1;
    vector<int> numbers;
    
    // Precompute (n-1)! and populate the available numbers pool
    for (int i = 1; i < n; ++i) {
        factorial *= i;
    }
    for (int i = 1; i <= n; ++i) {
        numbers.push_back(i);
    }
    
    // Switch to 0-based indexing for accurate mathematical block division
    k--;
    string result = "";
    
    // Loop to build out each character position of the permutation sequence
    for (int i = n - 1; i >= 0; --i) {
        int index = k / factorial;
        
        result += to_string(numbers[static_cast<size_t>(index)]);
        numbers.erase(numbers.begin() + index); // Remove the used digit from the pool
        
        if (i > 0) {
            k %= factorial;
            factorial /= i; // Step down to the next lower factorial size tier (i-1)!
        }
    }
    
    return result;
}

int main() {
    int n, k;
    cout << "Enter the number of elements N (typically 1 to 9): ";
    cin >> n;
    cout << "Enter the target permutation sequence index K: ";
    cin >> k;

    // Check bounds for safe execution
    if (n < 1 || n > 9) {
        cout << "Invalid input. N must be between 1 and 9." << endl;
        return 1;
    }
    
    // Compute total possible permutations to validate K bounds
    int totalPermutations = 1;
    for (int i = 1; i <= n; ++i) totalPermutations *= i;
    
    if (k < 1 || k > totalPermutations) {
        cout << "Invalid input. K must be between 1 and " << totalPermutations << "." << endl;
        return 1;
    }

    string sequence = getPermutation(n, k);
    cout << "\nThe " << k << "-th permutation sequence for N = " << n << " is: \"" << sequence << "\"" << endl;

    return 0;
}