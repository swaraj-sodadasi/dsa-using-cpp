#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BINARY TO GRAY CODE CONVERSION MATH):
- The objective is to generate an n-bit Gray code sequence, which is a binary numeral system where two successive values differ in only one bit position.
- **Mathematical Formula**:
  - For any non-negative binary integer `i`, its corresponding Gray code representation can be computed directly using a bitwise right-shift and a bitwise XOR operation:
    $$ G(i) = i \oplus (i \gg 1) $$
- **Why this works**:
  - Shifting `i` to the right by 1 drops the lowest bit. When we XOR this shifted value with the original `i`, a bit at position `k` in the output will be 1 if and only if the bits at position `k` and `k + 1` in `i` are different.
  - Because consecutive binary numbers change their bits in predictable cascading blocks, this transformation ensures that the resulting Gray code values change by exactly one bit at each sequential step.
- **Sequence Generation**:
  - An n-bit Gray code sequence contains exactly $2^n$ unique values. 
  - We can construct the entire sequence in order by looping from `0` up to `(1 << n) - 1` and applying the formula to each index.

TIME COMPLEXITY: O(2^N) - We iterate exactly $2^N$ times to compute and store each sequence element in constant time.
SPACE COMPLEXITY: O(1) auxiliary - Beyond allocating the vector used to return the result, the sequence is generated entirely in-place.
*/
vector<int> grayCode(int n) {
    if (n < 0) return {};
    
    // Determine the total elements in the sequence sequence via 2^N expansion
    size_t sequenceLength = 1ULL << n;
    vector<int> result;
    result.reserve(sequenceLength);

    // Apply the binary-to-Gray transformation formula to each index sequentially
    for (size_t i = 0; i < sequenceLength; ++i) {
        result.push_back(static_cast<int>(i ^ (i >> 1)));
    }

    return result;
}

int main() {
    int n;
    cout << "Enter the number of bits (n): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid sizing configuration input bounds." << endl;
        return 1;
    }

    vector<int> result = grayCode(n);

    cout << "\nThe generated " << n << "-bit Gray code sequence is:\n[ ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << "]\n";

    return 0;
}