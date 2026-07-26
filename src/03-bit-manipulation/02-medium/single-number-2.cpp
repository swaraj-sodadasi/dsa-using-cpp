#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BITWISE BIT-POSITION MODULO-3 COUNTING):
- The problem asks us to find a single unique element in an array where every other element appears exactly three times.
- **Modulo-3 Arithmetic on Bit Columns**:
  - Consider the binary representation of all integers in the array. For any given bit position (from 0 to 31):
    - If a number appears three times, the bit at this position will contribute either 0 or 3 to the total count of 1s at that position across the entire array.
    - Therefore, if we sum the bits at a specific position for all numbers in the array, the total sum for that bit position must be of the form $3k$ (if the unique number has a 0 at this position) or $3k + 1$ (if the unique number has a 1 at this position).
  - By taking the sum of each bit column modulo 3 (`sum % 3`), we isolate the exact bit configuration of the unique number.
- **Algorithmic Application**:
  - We loop through every bit position `i` from 0 to 31.
  - For each position, we count how many numbers in the array have a 1 at the `i`-th bit using the check `(num >> i) & 1`.
  - If the count modulo 3 is non-zero (`count % 3 != 0`), it means the unique number has a 1 at this position, so we set that bit in our `result` variable using `result |= (1 << i)`.

TIME COMPLEXITY: O(32 * N) = O(N) - Where N is the total number of elements in the array. We perform a linear pass over the dataset exactly 32 times.
SPACE COMPLEXITY: O(1) auxiliary - Operates entirely in-place without generating any extra data structures.
*/
int singleNumber(const vector<int>& nums) {
    int result = 0;

    // Iterate through every bit position for a standard 32-bit integer
    for (int i = 0; i < 32; ++i) {
        int sumOfBits = 0;
        
        // Count how many numbers have the i-th bit set to 1
        for (int num : nums) {
            sumOfBits += ((num >> i) & 1);
        }

        // If the column sum is not a multiple of 3, this bit belongs to the unique number
        if (sumOfBits % 3 != 0) {
            result |= (1 << i);
        }
    }

    return result;
}

int main() {
    int n;
    cout << "Enter the total number of elements: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid array size configuration." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the array elements (where all except one appear exactly three times):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int result = singleNumber(nums);
    cout << "\nThe unique element that appears only once is: " << result << endl;

    return 0;
}