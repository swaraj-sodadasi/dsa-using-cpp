#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BITMASK DP / TERNARY MASK SYSTEM MATCHING):
- The problem asks us to place an array of integers `nums` into a set of slots labeled 1 to `numSlots`.
  Each slot can hold at most 2 numbers. The score of placing a number `nums[i]` into slot `j` is `nums[i] & j`.
  Our goal is to maximize the total sum of these bitwise AND operations.
- **Ternary State Tracking via Base-3 Bitmasks**:
  - Since each slot can hold up to 2 numbers, a standard binary mask (0 or 1) cannot represent the states of the slots.
  - Instead, we use a base-3 (ternary) numbering system. Each slot `j` can be in one of three states:
    - `0`: The slot is empty.
    - `1`: The slot holds exactly 1 number.
    - `2`: The slot holds exactly 2 numbers (completely full).
  - With `numSlots <= 9`, the maximum number of unique slot configurations is 3^9 = 19683.
- **Dynamic Programming Design**:
  - We can define our DP state based on the slot configuration. 
  - To track where we are in `nums`, we count how many items have been placed by summing up the digits of our ternary mask.
  - For example, if a ternary mask has digits that add up to `k`, it means we are currently trying to place `nums[k]`.
- **State Transition Strategy**:
  - We use a 1D `dp` array of size 3^numSlots initialized to 0.
  - For each ternary state `mask` from 0 up to 3^numSlots - 1:
    - We calculate how many slots are occupied (this tells us the index `i` of the next number to place from `nums`).
    - If `i` is greater than or equal to the total number of elements in `nums`, we have successfully placed all numbers.
    - Otherwise, we iterate through each slot `j` (from 0 to `numSlots - 1`). We check the value of the $j$-th ternary digit:
      - If the digit is less than 2, the slot has room.
      - We transition to `nextMask = mask + powerOf3[j]`.
      - The score updates to `dp[nextMask] = max(dp[nextMask], dp[mask] + (nums[i] & (j + 1)))`.

TIME COMPLEXITY: O(numSlots * 3^numSlots) - The algorithm iterates through all 3^numSlots states.
SPACE COMPLEXITY: O(3^numSlots) auxiliary - Size of the DP array lookup structure.
*/

int maximumANDSum(const vector<int>& nums, int numSlots) {
    // Precompute powers of 3 to quickly extract and manipulate ternary digits
    vector<int> powerOf3(static_cast<size_t>(numSlots) + 1, 1);
    for (size_t i = 1; i <= static_cast<size_t>(numSlots); ++i) {
        powerOf3[i] = powerOf3[i - 1] * 3;
    }

    size_t totalStates = static_cast<size_t>(powerOf3[static_cast<size_t>(numSlots)]);
    vector<int> dp(totalStates, 0);

    int maxFinalScore = 0;

    // Iterate through every ternary slot configuration mask state
    for (size_t mask = 0; mask < totalStates; ++mask) {
        // Calculate the number of items currently placed by expanding the ternary mask digits
        int placedCount = 0;
        size_t tempMask = mask;
        for (int j = 0; j < numSlots; ++j) {
            placedCount += (tempMask % 3);
            tempMask /= 3;
        }

        // FIX: If placedCount is greater than or equal to the total elements, we track the score and skip 
        // processing to prevent accessing an out-of-bounds index in the nums vector.
        if (placedCount >= static_cast<int>(nums.size())) {
            maxFinalScore = max(maxFinalScore, dp[mask]);
            continue;
        }

        // Attempt to place the next element (nums[placedCount]) into an available slot
        for (int j = 0; j < numSlots; ++j) {
            // Isolate the ternary digit at the j-th position: (mask / 3^j) % 3
            int currentSlotOccupancy = (static_cast<int>(mask) / powerOf3[static_cast<size_t>(j)]) % 3;

            // Transition forward if the slot has room to receive another element (occupancy is less than 2)
            if (currentSlotOccupancy < 2) {
                size_t nextMask = mask + static_cast<size_t>(powerOf3[static_cast<size_t>(j)]);
                int gain = nums[static_cast<size_t>(placedCount)] & (j + 1);
                
                dp[nextMask] = max(dp[nextMask], dp[mask] + gain);
            }
        }
    }

    return maxFinalScore;
}

int main() {
    int n, numSlots;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid array size tracking parameter bounds." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the integer elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the number of slots available (maximum 9): ";
    if (!(cin >> numSlots) || numSlots <= 0 || numSlots > 9) {
        cout << "Invalid constraint bounds. Slots count must be an integer between 1 and 9." << endl;
        return 1;
    }

    // A logical integrity check: the total slot capacity must be able to hold all input array items
    if (n > 2 * numSlots) {
        cout << "\nWarning: Element count (" << n << ") exceeds the combined maximum capacity of the slots (" << 2 * numSlots << ")." << endl;
        return 1;
    }

    int result = maximumANDSum(nums, numSlots);
    cout << "\nThe maximum achievable bitwise AND sum matches: " << result << endl;

    return 0;
}