#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (DIGIT FREQUENCY COUNTING AND MATCHING):
- The problem asks whether we can reorder the digits of a given integer `n` (without leading zeros) 
  so that the resulting number forms a power of 2.
- A direct permutation approach (generating all arrangements of digits) would be highly inefficient 
  and complex due to handling duplicate digits and leading zeros.
- Instead, we can observe that any two numbers are reordered versions of each other if and only if they 
  have the exact same count of each digit (0-9).
- Since `n` fits within the bounds of a standard 32-bit signed integer ($n \le 10^9$), the maximum possible 
  power of 2 we ever need to consider is $2^{30} = 1,073,741,824$.
- We can precompute a helper function `countDigits(num)` that returns a fixed-size vector containing 
  the frequency of each digit from 0 to 9 for any number.
- We then extract the digit frequencies of our input `n`. We compare this frequency signature against 
  the digit frequency signatures of all powers of 2 from $2^0$ up to $2^{30}$.
- If a match is found, it means `n` can be reordered into that specific power of 2, so we return `true`. 
  If no powers of 2 match after checking the entire range, we return `false`.

TIME COMPLEXITY: O(log10(N)) - The input integer `n` has at most 10 digits. We check against 31 fixed powers of 2. 
                 Counting digits for each power takes at most 10 steps, making the total number of operations constant and small.
SPACE COMPLEXITY: O(1) auxiliary - The digit frequency arrays use a constant size of 10 elements, satisfying the O(1) space constraint.
*/

// Helper function to count the frequency of each digit (0-9) in a number
vector<int> countDigits(int num) {
    vector<int> counts(10, 0);
    while (num > 0) {
        counts[static_cast<size_t>(num % 10)]++;
        num /= 10;
    }
    return counts;
}

bool reorderedPowerOf2(int n) {
    // Extract the digit frequency profile of the target number
    vector<int> targetCounts = countDigits(n);

    // Iterate through all possible powers of 2 that fit inside a 32-bit integer range
    for (int i = 0; i <= 30; ++i) {
        int powerOf2 = 1 << i;
        
        // Compare the digit frequency profile of the current power of 2
        if (targetCounts == countDigits(powerOf2)) {
            return true;
        }
    }

    return false;
}

int main() {
    int n;
    cout << "Enter a positive integer N: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid input. Please enter a positive integer." << endl;
        return 1;
    }

    if (reorderedPowerOf2(n)) {
        cout << "\nYes, the digits of " << n << " can be reordered to form a power of 2." << endl;
    } else {
        cout << "\nNo, the digits of " << n << " CANNOT be reordered to form a power of 2." << endl;
    }

    return 0;
}