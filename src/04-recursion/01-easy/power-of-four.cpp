#include <iostream>

using namespace std;

/*
CORE LOGIC (PURE RECURSIVE SUB-PROBLEM REDUCTION):
- The objective is to determine whether a given integer $n$ is a power of four ($n = 4^k$ for some integer $k$).
- **Recursive Reduction Breakdown**:
  - Any number $n$ that is a power of four must satisfy specific mathematical properties:
    - It must be strictly positive ($n > 0$).
    - It must be perfectly divisible by 4 ($n \% 4 == 0$).
  - If these conditions are met, we can reduce the problem size. Determining if $n$ is a power of four is equivalent to checking if $n / 4$ is a power of four.
- **Base Cases**:
  - **Case 1**: $n \le 0 \rightarrow$ Numbers less than or equal to zero cannot be powers of four $\rightarrow$ return `false`.
  - **Case 2**: $n == 1 \rightarrow$ Since $4^0 = 1$, this represents the successful convergence baseline $\rightarrow$ return `true`.
  - **Case 3**: $n \% 4 \ne 0 \rightarrow$ If a number greater than 1 is not evenly divisible by 4, it cannot be a power of four $\rightarrow$ return `false`.
- **Recursive Step**:
  - If $n$ passes the initial filters, we recursively compute `isPowerOfFour(n / 4)`.

TIME COMPLEXITY: O(log_4 N) - The input size is divided by 4 at each successive stack layer, leading to logarithmic execution depth.
SPACE COMPLEXITY: O(log_4 N) auxiliary - Storage space allocated on the system call stack frames for the execution lifecycle tracking.
*/

bool isPowerOfFour(int n) {
    // Base Case 1: Numbers less than or equal to zero cannot be powers of four
    if (n <= 0) {
        return false;
    }
    // Base Case 2: 4^0 equals 1, marking a successful matching termination point
    if (n == 1) {
        return true;
    }
    // Base Case 3: If the number isn't perfectly divisible by 4, it cannot be a power of four
    if (n % 4 != 0) {
        return false;
    }
    
    // Recursive Sub-problem Reduction: Check if the quotient is a power of four
    return isPowerOfFour(n / 4);
}

int main() {
    int inputVal;
    cout << "Enter an integer to evaluate: ";
    if (!(cin >> inputVal)) {
        cout << "Invalid numeric structural configuration entry." << endl;
        return 1;
    }

    cout << "\nExecuting pure recursive sub-problem reduction check..." << endl;
    if (isPowerOfFour(inputVal)) {
        cout << "Result: The integer " << inputVal << " is a valid power of four." << endl;
    } else {
        cout << "Result: The integer " << inputVal << " is NOT a power of four." << endl;
    }

    return 0;
}