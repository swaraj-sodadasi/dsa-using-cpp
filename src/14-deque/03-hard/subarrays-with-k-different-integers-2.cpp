#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
private:
    /*
     * HELPER LOGIC: AT MOST K DISTINCT INTEGERS (SLIDING WINDOW)
     * Calculates total continuous subarrays containing AT MOST k distinct integers.
     */
    int atMostK(const vector<int>& nums, int k) {
        if (k <= 0) return 0;

        unordered_map<int, int> freq;
        int left = 0;
        int totalSubarrays = 0;

        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            int rightVal = nums[static_cast<size_t>(right)];
            freq[rightVal]++;

            // Shrink window from left if distinct count exceeds k
            while (static_cast<int>(freq.size()) > k) {
                int leftVal = nums[static_cast<size_t>(left)];
                freq[leftVal]--;
                if (freq[leftVal] == 0) {
                    freq.erase(leftVal);
                }
                left++;
            }

            // All valid subarrays ending at 'right' starting between 'left' and 'right'
            totalSubarrays += (right - left + 1);
        }

        return totalSubarrays;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SUBARRAYS WITH K DIFFERENT INTEGERS (EXACTLY K REDUCTION)
     * -------------------------------------------------------------------------------------
     * The objective is to count continuous subarrays with EXACTLY k distinct integers.
     * 
     * 1. MATHEMATICAL REDUCTION:
     *    `Exactly(K) = AtMost(K) - AtMost(K - 1)`
     * 
     * 2. O(N) SLIDING WINDOW TRAVERSAL:
     *    `atMostK(nums, k)` calculates total subarrays with <= k distinct elements.
     *    `atMostK(nums, k - 1)` calculates total subarrays with <= k - 1 distinct elements.
     *    Subtracting the two yields the count of subarrays with exactly k distinct integers.
     */
    int subarraysWithKDistinct(const vector<int>& nums, int k) {
        return atMostK(nums, k) - atMostK(nums, k - 1);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of elements in nums.
 *                  Two linear passes across the array.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Hash map stores at most K + 1 distinct elements.
 */
int main() {
    cout << "=== Subarrays with K Different Integers Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter target distinct count k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    int result = solver.subarraysWithKDistinct(nums, k);

    cout << "Total subarrays with exactly " << k << " distinct integers: [" << result << "]\n";

    return 0;
}