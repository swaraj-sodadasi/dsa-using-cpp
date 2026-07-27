#include <iostream>

using namespace std;

/*
CORE LOGIC (RECURSIVE CALL-STACK INDUCTION):
- The objective is to print all integers from 1 to N sequentially without utilizing any iterative loop constructs (like `for`, `while`, or `do-while`).
- **Mathematical Induction Principle**:
  - To solve this problem using recursion, we break it down into a base case and an inductive step:
    - Base Case: If $N == 0$, there is nothing left to process, so we terminate the current execution path.
    - Inductive Step: To print numbers from 1 to $N$, we can first delegate the task of printing the numbers from 1 to $N-1$ to a recursive call. Once that call completes and yields control back, we print the current value $N$.
- **Call-Stack Execution Flow**:
  - By invoking `printNos(N - 1)` *before* executing the `cout << N` print statement, we leverage the system call stack to defer printing. 
  - The stack builds up downward from $N$ to 1. When it hits the base case (0), it starts popping frames backward, executing the print operations in exact ascending order (1, 2, ..., N).

TIME COMPLEXITY: O(N) - The algorithm executes exactly N recursive calls, processing each numeric value precisely once.
SPACE COMPLEXITY: O(N) auxiliary - The depth of the runtime system call stack scales linearly with N to manage the active function tracking frames.
*/

void printNos(int n) {
    // Base Case: If n drops down to 0, stop descending the call stack
    if (n == 0) {
        return;
    }

    // Inductive Step: Defer printing the current value until all numbers before it are printed
    printNos(n - 1);

    // Print the current value during stack winding back up
    cout << n << " ";
}

int main() {
    int n;
    cout << "Enter the upper bound integer limit (N): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input. N must be a non-negative integer." << endl;
        return 1;
    }

    cout << "\nOutput sequence (1 to " << n << ") generated via call-stack induction:\n";
    if (n == 0) {
        cout << "[Empty sequence]";
    } else {
        printNos(n);
    }
    cout << endl;

    return 0;
}