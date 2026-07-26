#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (ISOLATION OF LOW-SET BIT VIA XOR PARTITIONING):
- The objective is to find the two unique elements in an array where every other element appears exactly twice.
- **Step 1: Compute the Global XOR Signature**:
  - We XOR all elements in the array. Pairs of identical elements cancel out to 0, leaving us with `xorSum = globalXor = a ^ b`, 
    where `a` and `b` are the two unique numbers we are looking for.
  - Since `a` and `b` are distinct, `a ^ b` must contain at least one set bit (1). This bit represents a position where `a` and `b` have completely different bits.
- **Step 2: Isolate the Rightmost Set Bit**:
  - We extract this single distinguishing bit using the two's complement expression: `diffBitMask = xorSum & (-xorSum)`. 
  - This mask contains a single 1 bit at the lowest position where `a` and `b` differ.
- **Step 3: Partition and Resolve**:
  - We loop through the array again and divide the numbers into two independent groups based on this bit mask:
    1. Group 1: Numbers that have the bit set at this position (`(num & diffBitMask) != 0`).
    2. Group 2: Numbers that have a 0 at this position (`(num & diffBitMask) == 0`).
  - The two unique numbers `a` and `b` are guaranteed to fall into different groups. 
  - All other duplicate pairs will fall into the same group together and cancel each other out when we XOR the groups, leaving exactly `a` in one group and `b` in the other.
- **Boundary Check**: We cast `globalXor` to an `unsigned int` before performing `-globalXor` to safely avoid any signed integer overflow errors when the minimum integer bound is passed.

TIME COMPLEXITY: O(N) - Where N is the number of elements in the vector array. We loop through the array exactly twice.
SPACE COMPLEXITY: O(1) auxiliary - The calculation runs completely in-place using localized scalar trackers.
*/
vector<int> singleNumber(const vector<int>& nums) {
    long long globalXor = 0;
    
    // Step 1: Compute the collective XOR sum of all values
    for (int num : nums) {
        globalXor ^= num;
    }
    
    // Step 2: Isolate the rightmost distinguishing set bit using unsigned tracking to prevent arithmetic overflow
    unsigned int diffBitMask = static_cast<unsigned int>(globalXor) & -static_cast<unsigned int>(globalXor);
    
    int uniqueA = 0;
    int uniqueB = 0;
    
    // Step 3: Partition the elements and isolate unique values within their respective bit groupings
    for (int num : nums) {
        if ((static_cast<unsigned int>(num) & diffBitMask) != 0) {
            uniqueA ^= num;
        } else {
            uniqueB ^= num;
        }
    }
    
    return {uniqueA, uniqueB};
}

int main() {
    int n;
    cout << "Enter the total number of elements: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input array configuration size." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the integer array elements (where exactly two appear once, and all others appear twice):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    vector<int> result = singleNumber(nums);
    if (result.size() == 2) {
        cout << "\nThe two unique elements are: " << result[0] << " and " << result[1] << endl;
    }

    return 0;
}