#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * UNORDERED SET BOUNDARY EXPANSION ENGINE
     * ----------------------------------------
     * The objective is to find the length of the longest consecutive elements sequence 
     * in an unsorted array. Crucially, the algorithm must achieve an O(N) runtime footprint.
     *
     * Mathematical & Algorithmic Mechanics:
     * - Sorting the array would immediately take O(N log N) time, which violates the constraint.
     * - We achieve O(N) linear time by transferring all integers into an `unordered_set`. 
     *   This grants us average constant time O(1) lookups to check if neighbors exist.
     *
     * Sequence Anchor Optimization Strategy:
     * - A naive lookup might restart calculations from every element, yielding up to an O(N^2) profile.
     * - To maintain a strict linear bound, we only attempt to build a sequence if the current number 
     *   is the **absolute starting anchor** of that sequence.
     * - A number `num` is an anchor if `num - 1` does not exist in our hash set. If `num - 1` is present, 
     *   we intentionally skip processing because the sequence containing `num` is already being 
     *   (or will be) accounted for by its smaller starting element.
     * - When an anchor is found, we expand the sequence boundary sequentially (`num + 1`, `num + 2`...) 
     *   until the consecutive sequence breaks.
     */
    int longestConsecutive(vector<int>& nums) {
        // Step 1: Populate the hash set to achieve O(1) average lookup times
        unordered_set<int> uniqueNumbersSet(nums.begin(), nums.end());
        int longestStreak = 0;

        // Step 2: Linear sweep over the collection pool
        for (int num : uniqueNumbersSet) {
            // CRITICAL STRATEGY: Check if 'num' is the absolute beginning anchor of a sequence
            if (!uniqueNumbersSet.count(num - 1)) {
                int currentNum = num;
                int currentStreak = 1;

                // Expand the boundary forward as long as adjacent consecutive numbers exist
                while (uniqueNumbersSet.count(currentNum + 1)) {
                    currentNum++;
                    currentStreak++;
                }

                // Maintain the largest contiguous sequence sequence length tracked
                longestStreak = max(longestStreak, currentStreak);
            }
        }

        return longestStreak;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  Although there is a nested while loop, each unique element is visited 
 *                  at most twice across the entire runtime (once in the main loop and at 
 *                  most once inside the sequence expansion loop), securing strict O(N) linear boundaries.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The hash set allocates internal storage proportional to the number 
 *                   of unique elements populated from the input sequence context.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting unordered set boundary expansion sweep..." << endl;
    int result = solver.longestConsecutive(nums);

    cout << "Length of the longest contiguous consecutive sequence: " << result << endl;

    return 0;
}