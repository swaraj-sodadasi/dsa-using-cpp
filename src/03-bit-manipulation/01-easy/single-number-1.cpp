#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (XOR CANCELLATION PROPERTY):
- The problem asks us to find the single unique element in an array where every other element appears exactly twice.
- **Bitwise XOR Properties**:
  1. Identity property: $x \oplus 0 = x$ (XORing a number with 0 leaves it unchanged).
  2. Self-inverse property: $x \oplus x = 0$ (XORing a number with itself cancels out completely and results in 0).
  3. Commutative and Associative properties: The order in which operations are executed does not matter ($a \oplus b \oplus a = a \oplus a \oplus b = 0 \oplus b = b$).
- **Algorithmic Application**:
  - We initialize an accumulator variable `result` to 0.
  - We iterate through all numbers in the array and apply a running bitwise XOR.
  - Pairs of identical elements will naturally cancel each other out to 0, leaving behind only the single element that appears once.

TIME COMPLEXITY: O(N) - Where N is the total number of elements in the vector. We traverse the array exactly once.
SPACE COMPLEXITY: O(1) auxiliary - Operates in-place using a single scalar tracking accumulator.
*/
int singleNumber(const vector<int>& nums) {
    int result = 0;
    
    // Accumulate the continuous bitwise XOR state variations across all elements
    for (int num : nums) {
        result ^= num;
    }
    
    return result;
}

int main() {
    int n;
    cout << "Enter the total number of elements: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid layout array dimensions." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the integer array elements (where all except one appear twice):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int result = singleNumber(nums);
    cout << "\nThe unique element that appears only once is: " << result << endl;

    return 0;
}