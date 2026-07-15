#include <iostream>

using namespace std;

/*
CORE LOGIC (BINARY SEARCH APPROACH):
- A number N is a perfect square if there exists an integer 'X' such that X * X = N.
- Any integer strictly less than 1 cannot be a valid perfect square in this context (negative numbers and zero are ruled out or handled separately).
- We can frame this as a search problem within the range [1, num]. Since the range is sorted, we can use Binary Search to find the integer root efficiently.
- To prevent integer overflow when calculating the square of our midpoint (`mid * mid`), we cast the operands to `long long`: `long long square = static_cast<long long>(mid) * mid`.
- If `square == num`, we return `true` immediately.
- If `square < num`, our midpoint is too small, so we shift the lower bound: `low = mid + 1`.
- If `square > num`, our midpoint is too large, so we shift the upper bound: `high = mid - 1`.
- If the binary search space is exhausted without a match, the number is not a perfect square.

TIME COMPLEXITY: O(log N) - The search space is halved in every iteration of the loop.
SPACE COMPLEXITY: O(1) - The range matching is evaluated in-place using scalar boundaries.
*/
bool isPerfectSquare(int num) {
    if (num < 1) return false;

    int low = 1;
    int high = num;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        long long square = static_cast<long long>(mid) * mid;

        if (square == num) {
            return true;
        } else if (square < num) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return false;
}

int main() {
    int num;
    cout << "Enter a positive integer: ";
    cin >> num;

    if (isPerfectSquare(num)) {
        cout << num << " is a valid perfect square." << endl;
    } else {
        cout << num << " is NOT a valid perfect square." << endl;
    }

    return 0;
}