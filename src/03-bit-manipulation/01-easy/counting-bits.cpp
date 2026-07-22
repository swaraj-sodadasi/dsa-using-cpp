#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (PATTERN-BASED DYNAMIC PROGRAMMING USING BIT-SHIFTING):
- The objective is to calculate the number of 1 bits for every integer from 0 up to a given number `n`, returning the results as an array.
- A naive solution running Brian Kernighan's or shifting techniques for each number takes O(n * log(n)) time. We can optimize this to O(n) using Dynamic Programming.
- **Dynamic Programming Relation**:
  - For any integer `i`, shifting its bits right by 1 position (`i >> 1`) is equivalent to evaluating the value `i / 2`. 
  - The number of set bits in `i` is identical to the number of set bits in `i >> 1`, plus the value of the rightmost bit itself.
  - The rightmost bit can be cleanly isolated using the bitwise check `i & 1`.
  - Therefore, we establish the transition formula:
    $$ ans[i] = ans[i \gg 1] + (i \text{ \& } 1) $$
- **Example walkthrough**:
  - `i = 5` (binary `101`).
  - `i >> 1 = 2` (binary `010`).
  - `ans[5] = ans[2] + (5 & 1)`. Since `ans[2]` is 1, and `5 & 1` is 1, `ans[5] = 1 + 1 = 2`.

TIME COMPLEXITY: O(n) - We iterate through all elements from 1 to n exactly once, executing constant-time bitwise additions.
SPACE COMPLEXITY: O(1) auxiliary - Beyond allocating the required vector mapping results, no extra storage footprints are generated.
*/
vector<int> countBits(int n) {
    // Initialize a results matrix initialized with zeros up to index bounds
    vector<int> ans(static_cast<size_t>(n) + 1, 0);
    
    // Iteratively build out state subproblems based on prior computed patterns
    for (int i = 1; i <= n; ++i) {
        ans[static_cast<size_t>(i)] = ans[static_cast<size_t>(i >> 1)] + (i & 1);
    }
    
    return ans;
}

int main() {
    int n;
    cout << "Enter the upper bound integer threshold (N): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 1;
    }

    vector<int> result = countBits(n);
    
    cout << "\nThe calculated set bit populations from 0 to " << n << " are:\n";
    for (int i = 0; i <= n; ++i) {
        cout << i << ": " << result[static_cast<size_t>(i)] << "\n";
    }
    
    return 0;
}