#include <iostream>
#include <chrono>

using namespace std;

/*
CORE LOGIC (BASIC LINEAR VS. TREE RECURSION):
- The objective is to compute the n-th Fibonacci number ($F_n$), defined mathematically by the recurrence relation:
  $$ F_n = F_{n-1} + F_{n-2} $$
  with base initialization contexts: $F_0 = 0, F_1 = 1$.
- **Tree Recursion (Exponential Complexity)**:
  - The intuitive execution matches the mathematical framework directly by spawning two sub-calls: `treeRecursion(n - 1) + treeRecursion(n - 2)`.
  - This architecture suffers from severe structural redundancy, recalculating identical sub-problems (e.g., $F_{n-2}$ is re-evaluated independently inside both the $F_{n-1}$ and $F_{n-2}$ paths). 
  - This causes the state space expansion layout to explode exponentially.
- **Linear State Tracking (Iterative/Linear Complexity)**:
  - Instead of looking backward via a branching tree structure, we calculate intermediate states sequentially from the bottom up.
  - We keep track of the two most recent Fibonacci values using two variables (`prev2` and `prev1`). For each step up to $n$, the next value is computed as `curr = prev1 + prev2`. We then update `prev2 = prev1` and `prev1 = curr`.
  - This eliminates duplicate work, reducing the execution time down to a flat linear runtime.

TIME COMPLEXITY:
- Tree Recursion: $O(2^N)$ - The recursive call framework splits in two at every level, creating an exponential state space tree.
- Linear Iteration: $O(N)$ - We traverse a flat, single-loop path from 2 up to N exactly once.
SPACE COMPLEXITY:
- Tree Recursion: $O(N)$ auxiliary - Bounded by the deepest execution path active on the system runtime call stack.
- Linear Iteration: $O(1)$ auxiliary - All allocations scale in-place inside fixed tracking scalar variables.
*/

// Implementation 1: Tree Recursion (Naive Exponential Path)
long long fibonacciTreeRecursion(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacciTreeRecursion(n - 1) + fibonacciTreeRecursion(n - 2);
}

// Implementation 2: Linear State Tracking (Optimal Linear Path)
long long fibonacciLinearIteration(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    long long prev2 = 0; // Represents F(i-2)
    long long prev1 = 1; // Represents F(i-1)
    long long current = 0;

    for (int i = 2; i <= n; ++i) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}

int main() {
    int n;
    cout << "Enter the Fibonacci index sequence identifier (n): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid index tracking configuration parameters." << endl;
        return 1;
    }

    // Cap the tree execution demonstration threshold to prevent runtime hangs
    cout << "\n--- Execution 1: Linear Iteration ---" << endl;
    auto startLinear = chrono::high_resolution_clock::now();
    long long resLinear = fibonacciLinearIteration(n);
    auto endLinear = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsedLinear = endLinear - startLinear;
    
    cout << "Result F(" << n << ") = " << resLinear << endl;
    cout << "Time Elapsed: " << elapsedLinear.count() << " ms" << endl;

    if (n <= 45) {
        cout << "\n--- Execution 2: Tree Recursion ---" << endl;
        cout << "Running naive tree branching recursion (please wait)..." << endl;
        auto startTree = chrono::high_resolution_clock::now();
        long long resTree = fibonacciTreeRecursion(n);
        auto endTree = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsedTree = endTree - startTree;

        cout << "Result F(" << n << ") = " << resTree << endl;
        cout << "Time Elapsed: " << elapsedTree.count() << " ms" << endl;
    } else {
        cout << "\n[Skipping Tree Recursion execution loop because n > 45 would cause an exponential calculation hang]" << endl;
    }

    return 0;
}