#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (XOR INDEX-VALUE PAIRING):
- The problem asks us to find the single missing number from an array containing $n$ distinct numbers in the range $[0, n]$.
- **Bitwise XOR Properties**:
  1. Self-inverse property: $x \oplus x = 0$ (XORing any number with itself completely cancels out).
  2. Identity property: $x \oplus 0 = x$ (XORing a number with 0 preserves the original number).
  3. Commutative and Associative properties: The absolute evaluation order of operations does not alter the mathematical conclusion.
- **Algorithmic Application**:
  - An array of size $n$ represents indexes from $0$ to $n-1$, while the full range of expected values contains elements from $0$ to $n$.
  - If we pair every vector element with its corresponding index, every number present will appear twice (once as an index and once as an array element).
  - The missing value, however, will appear exactly once (only as a loop index or range boundary, since it is absent from the array elements).
  - By XORing the range value $n$, along with every valid index `i` and array value `nums[i]`, the matching pairs cancel out completely, leaving the missing number.

TIME COMPLEXITY: O(N) - Where N is the total number of elements in the vector array. We run a single linear evaluation pass.
SPACE COMPLEXITY: O(1) auxiliary - Memory footprints are bounded entirely by scalar local tracking states.
*/
int missingNumber(const vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int missing = n; // Initialize with the boundary out-of-index value n

    for (int i = 0; i < n; ++i) {
        // XOR the current array element and the loop index to filter out duplicates
        missing ^= i ^ nums[static_cast<size_t>(i)];
    }

    return missing;
}

int main() {
    int n;
    cout << "Enter the total number of elements present in the array (n): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input length constraints." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the " << n << " distinct elements inside the range [0, " << n << "]:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int result = missingNumber(nums);
    cout << "\nThe missing number inside the sequence range is: " << result << endl;

    return 0;
}