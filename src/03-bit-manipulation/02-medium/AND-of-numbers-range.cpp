#include <iostream>

using namespace std;

/*
CORE LOGIC (COMMON MSB PREFIX IDENTIFICATION):
- The problem asks us to find the bitwise AND of all numbers in the range [left, right], inclusive.
- **The Bit-Flipping Nature of Consecutive Ranges**:
  - As numbers increment sequentially from `left` to `right`, the lower-order bits alternate rapidly between 0 and 1.
  - If we perform a bitwise AND across all consecutive values in a range, any bit position that flips to 0 even once at any point in the sequence will ultimately be forced to 0 in the final answer.
  - The only bits that can survive this consecutive range AND operation are the high-order bits that stay completely identical and unchanged across every single number from `left` all the way to `right`. This is known as the **Common Most Significant Bit (MSB) Prefix**.
- **Algorithmic Reduction via Bit Shifting**:
  - We compare the numbers `left` and `right`. If they are not equal, it means there is at least one value between them that will cause the lowest bit position to flip to 0.
  - We shift both `left` and `right` one bit position to the right (`left >>= 1`, `right >>= 1`) to discard this changing lower bit, and we increment a shift tracker variable `shiftCount`.
  - We repeat this shifting process until `left` becomes exactly equal to `right`. When they match, the remaining bits represent the shared, unchanged prefix.
  - Finally, we shift the matched prefix back to its original position by multiplying it by $2^{\text{shiftCount}}$, which is implemented efficiently using the left-shift expression: `left << shiftCount`.

TIME COMPLEXITY: O(1) - The loop runs at most 31 times for positive 32-bit signed integers, bounding the execution to constant time.
SPACE COMPLEXITY: O(1) auxiliary - The structural matching is performed completely in-place using localized scalar trackers.
*/
int rangeBitwiseAnd(int left, int right) {
    int shiftCount = 0;
    
    // Shift both boundaries rightward until they converge on their common MSB prefix
    while (left < right) {
        left >>= 1;
        right >>= 1;
        shiftCount++;
    }
    
    // Realign the isolated shared prefix back to its original scale
    return left << shiftCount;
}

int main() {
    int left, right;
    cout << "Enter the lower bound integer (left): ";
    if (!(cin >> left) || left < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 1;
    }
    cout << "Enter the upper bound integer (right): ";
    if (!(cin >> right) || right < left) {
        cout << "Invalid range bounds configuration entry." << endl;
        return 1;
    }

    int result = rangeBitwiseAnd(left, right);
    cout << "\nThe bitwise AND result of all numbers within the range [" << left << ", " << right << "] is: " << result << endl;

    return 0;
}