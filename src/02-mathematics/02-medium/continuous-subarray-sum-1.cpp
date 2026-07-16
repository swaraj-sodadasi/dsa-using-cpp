#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/*
CORE LOGIC (MODULAR PREFIX SUM CHECK):
- The objective is to find a contiguous subarray of size at least two whose elements sum up to a multiple of `k`.
- Mathematically, a sum is a multiple of `k` if `sum % k == 0`.
- We use a Prefix Sum approach combined with a Hash Map. Let `prefixSum[i]` be the sum of elements from index 0 to `i`.
  The sum of a subarray from index `j + 1` to `i` is `prefixSum[i] - prefixSum[j]`.
- For this subarray sum to be a multiple of `k`:
  $$ (prefixSum[i] - prefixSum[j]) \pmod k = 0 \implies prefixSum[i] \pmod k = prefixSum[j] \pmod k $$
- Therefore, if we find the same remainder twice at two different indices `j` and `i`, the elements between them sum to a multiple of `k`.
- We iterate through the array, tracking the running `prefixSumRemainder = (prefixSumRemainder + nums[i]) % k`.
  - To handle negative numbers cleanly (if applicable), we normalize the remainder: `((remainder % k) + k) % k`.
- We check if this remainder has been seen before:
  - If it exists in our map, we verify the distance constraint: `i - remainderMap[remainder] >= 2`. If satisfied, we return `true`.
  - If it does not exist, we store the remainder along with its first occurrence index: `remainderMap[remainder] = i`. We do not update it on subsequent matches to maximize the window width.
- **Edge Case Initialization**: We initialize the map with `{0, -1}`. This covers cases where a valid prefix subarray itself (starting from index 0) sums to a multiple of `k`. If it happens at index `i`, the length is `i - (-1) = i + 1`, which correctly evaluates the length.

TIME COMPLEXITY: O(N) - We perform a single linear pass over the array of size N. Hash map lookups and insertions take O(1) time on average.
SPACE COMPLEXITY: O(min(N, k)) - In the worst-case scenario, the map stores unique remainders, which is bounded by the size of the array N or the modulo divisor k.
*/
bool checkSubarraySum(const vector<int>& nums, int k) {
    if (k == 0) {
        // Handling k = 0 division edge case: a multiple of 0 can only be 0.
        // We look for at least two consecutive zeros.
        for (size_t i = 1; i < nums.size(); ++i) {
            if (nums[i] == 0 && nums[i - 1] == 0) {
                return true;
            }
        }
        return false;
    }

    // Map to store the first occurrence of a remainder: key = remainder, value = index
    unordered_map<int, int> remainderMap;
    remainderMap[0] = -1; // Base case initialization

    int runningRemainder = 0;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        runningRemainder += nums[static_cast<size_t>(i)];
        runningRemainder %= k;

        // Normalize negative remainders if input contains negative values
        if (runningRemainder < 0) {
            runningRemainder += k;
        }

        if (remainderMap.count(runningRemainder)) {
            // Check if the subarray length is at least 2
            if (i - remainderMap[runningRemainder] >= 2) {
                return true;
            }
        } else {
            // Store only the first occurrence to maintain the maximum possible subarray length
            remainderMap[runningRemainder] = i;
        }
    }

    return false;
}

int main() {
    int n, k;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid array size." << endl;
        return 1;
    }

    vector<int> nums(n);
    cout << "Enter " << n << " integers separated by space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    cout << "Enter the integer divisor (k): ";
    cin >> k;

    if (checkSubarraySum(nums, k)) {
        cout << "\nYes, there is a continuous subarray of size >= 2 that sums to a multiple of " << k << "." << endl;
    } else {
        cout << "\nNo, there is NO continuous subarray of size >= 2 that sums to a multiple of " << k << "." << endl;
    }

    return 0;
}