#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MAXIMUM SCORE OF A GOOD SUBARRAY (TWO-POINTER GREEDY EXPANSION)
     * -------------------------------------------------------------------------------------
     * A "good" subarray must contain index k. Its score is defined as:
     *   min(nums[i...j]) * (j - i + 1) where i <= k <= j.
     * 
     * 1. GREEDY OUTWARD BOUNDARY EXPANSION:
     *    We start with a window consisting solely of index k: `left = k`, `right = k`.
     *    The initial minimum element is `minVal = nums[k]`.
     * 
     * 2. OPTIMAL CHOICE STRATEGY:
     *    To maximize the score as we expand the window outwards:
     *      - If both left and right boundaries can expand, we greedily pick the adjacent element
     *        with the LARGER value (`nums[left - 1]` vs `nums[right + 1]`).
     *      - This ensures that the minimum value `minVal` drops as slowly as possible, maximizing 
     *        the product `minVal * length`.
     * 
     * 3. SCORE TRACKING:
     *    At each expansion step, we update `minVal = min(minVal, nums[chosen])` and compute 
     *    `maxScore = max(maxScore, minVal * (right - left + 1))`.
     */
    int maximumScore(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        int left = k;
        int right = k;
        int minVal = nums[static_cast<size_t>(k)];
        int maxScore = minVal;

        while (left > 0 || right < n - 1) {
            int leftVal = (left > 0) ? nums[static_cast<size_t>(left - 1)] : -1;
            int rightVal = (right < n - 1) ? nums[static_cast<size_t>(right + 1)] : -1;

            if (leftVal >= rightVal) {
                left--;
                minVal = min(minVal, nums[static_cast<size_t>(left)]);
            } else {
                right++;
                minVal = min(minVal, nums[static_cast<size_t>(right)]);
            }

            maxScore = max(maxScore, minVal * (right - left + 1));
        }

        return maxScore;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of elements in nums.
 *                  Single pass expanding outward from index k to array boundaries.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Constant extra space used for pointers and counters.
 */
int main() {
    cout << "=== Maximum Score of a Good Subarray Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter target center index k (0-based): ";
    int k;
    if (!(cin >> k) || k < 0 || k >= n) return 1;

    Solution solver;
    int result = solver.maximumScore(nums, k);

    cout << "Maximum score of a good subarray: [" << result << "]\n";

    return 0;
}