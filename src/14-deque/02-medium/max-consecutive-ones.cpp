#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MAX CONSECUTIVE ONES III (VARIABLE LENGTH SLIDING WINDOW)
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum number of consecutive 1s in a binary array 
     * if you can flip at most `k` 0s.
     * 
     * 1. TWO-POINTER SLIDING WINDOW:
     *    We maintain a expanding window defined by `[left, right]`.
     *    - We expand `right` on each iteration.
     *    - If `nums[right] == 0`, we increment our zero counter `zeroCount`.
     * 
     * 2. WINDOW SHRINKING CONDITION:
     *    - Whenever `zeroCount > k`, the current window contains more zeros than allowed.
     *    - We contract the window from the left by advancing `left` until `zeroCount <= k`.
     * 
     * 3. MAXIMUM LENGTH UPDATE:
     *    At each step with a valid window (`zeroCount <= k`), the length of the window is 
     *    `right - left + 1`. We update `maxLen` with this value.
     */
    int longestOnes(const vector<int>& nums, int k) {
        int left = 0;
        int zeroCount = 0;
        int maxLen = 0;

        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            if (nums[static_cast<size_t>(right)] == 0) {
                zeroCount++;
            }

            // Shrink window if zero count exceeds allowed flips k
            while (zeroCount > k) {
                if (nums[static_cast<size_t>(left)] == 0) {
                    zeroCount--;
                }
                left++;
            }

            maxLen = max(maxLen, right - left + 1);
        }

        return maxLen;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the size of array `nums`.
 *                  Both `left` and `right` pointers traverse the array at most once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Constant extra memory space.
 */
int main() {
    cout << "=== Max Consecutive Ones III Sliding Window Engine ===\n";
    cout << "Enter the size of the binary array: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter binary array elements (0s and 1s):\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter maximum allowed zero flips k: ";
    int k;
    if (!(cin >> k) || k < 0) return 1;

    Solution solver;
    int result = solver.longestOnes(nums, k);

    cout << "Maximum consecutive 1s achievable: [" << result << "]\n";

    return 0;
}