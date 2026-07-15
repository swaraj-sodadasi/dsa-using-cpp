#include <iostream>

using namespace std;

/*
CORE LOGIC (FLOYD'S CYCLE-FINDING ALGORITHM ON DIGIT SQUARES):
- A happy number is defined by a process where we replace the number with the sum of the squares of its digits. 
  We repeat this process until the number either equals 1 (where it stays), or loops endlessly in a cycle that does not include 1.
- Because this process forms a linked list-like state graph, we can detect cycles without using an auxiliary hash set by applying **Floyd's Cycle-Finding Algorithm** (Tortoise and Hare technique).
- We maintain two tracking pointers: `slow` and `fast`. 
  - `slow` moves forward by computing the digit square sum **once** per iteration.
  - `fast` moves forward by computing the digit square sum **twice** per iteration.
- If the number is happy, the `fast` runner will reach 1 first. If the number is not happy and enters an infinite loop, the `fast` runner will eventually catch up and meet the `slow` runner inside the cycle (`slow == fast`).
- We loop until either `fast == 1` or `slow == fast`. Afterward, if `fast == 1`, the number is a happy number.

TIME COMPLEXITY: O(log N) - The digit length drops quickly, and cycle detection bounds the operations to a small number of states.
SPACE COMPLEXITY: O(1) - Executed completely in-place using two scalar tracking state variables.
*/

// Helper function to calculate the sum of the squares of the digits of a number
int getNextValue(int n) {
    int totalSum = 0;
    while (n > 0) {
        int digit = n % 10;
        totalSum += digit * digit;
        n /= 10;
    }
    return totalSum;
}

bool isHappy(int n) {
    int slow = n;
    int fast = getNextValue(n);

    // Loop until the fast pointer reaches 1 or a cycle is encountered
    while (fast != 1 && slow != fast) {
        slow = getNextValue(slow);
        fast = getNextValue(getNextValue(fast));
    }

    return fast == 1;
}

int main() {
    int n;
    cout << "Enter a positive integer: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid input. Please enter a positive integer." << endl;
        return 1;
    }

    if (isHappy(n)) {
        cout << n << " is a Happy Number!" << endl;
    } else {
        cout << n << " is NOT a Happy Number." << endl;
    }

    return 0;
}