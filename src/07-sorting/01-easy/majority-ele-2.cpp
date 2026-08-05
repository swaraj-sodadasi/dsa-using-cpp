#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * BOYER-MOORE VARIANT WITH VALIDATION PASS ENGINE
     * ------------------------------------------------
     * The objective is to find all elements in an integer array that appear more 
     * than ⌊ n / 3 ⌋ times, using O(1) auxiliary space and running in linear time.
     *
     * Algorithmic & Mathematical Strategy:
     * - Based on the pigeonhole principle, an array can contain at most two distinct 
     *   elements that appear strictly more than ⌊ n / 3 ⌋ times.
     * - We extend the standard Boyer-Moore Majority Vote algorithm to track two potential 
     *   candidates (`candidate1`, `candidate2`) along with their respective voting counters 
     *   (`count1`, `count2`).
     *
     * Voting State Transition Rules:
     * - Rule 1: If the current number matches `candidate1`, increment `count1`.
     * - Rule 2: If it matches `candidate2`, increment `count2`.
     * - Rule 3: If `count1` is 0, claim this number as `candidate1` and set `count1 = 1`.
     * - Rule 4: If `count2` is 0, claim this number as `candidate2` and set `count2 = 1`.
     * - Rule 5: If the number matches neither candidate and both counters are non-zero, 
     *   decrement both counters (a 3-way balancing cancellation step).
     *
     * Verification Pass Protocol:
     * - The voting process only identifies potential candidates. We must run a second linear 
     *   pass to count the actual occurrences of the two candidates and verify whether they 
     *   strictly exceed the ⌊ n / 3 ⌋ threshold.
     */
    vector<int> majorityElement(vector<int>& nums) {
        vector<int> result;
        if (nums.empty()) return result;

        // Step 1: Initialize dual candidates and their respective voting counters
        int candidate1 = 0, candidate2 = 0;
        int count1 = 0, count2 = 0;

        // Step 2: Execute the variant Boyer-Moore voting elimination pass
        for (int num : nums) {
            if (num == candidate1) {
                count1++;
            } else if (num == candidate2) {
                count2++;
            } else if (count1 == 0) {
                candidate1 = num;
                count1 = 1;
            } else if (count2 == 0) {
                candidate2 = num;
                count2 = 1;
            } else {
                count1--;
                count2--;
            }
        }

        // Step 3: Explicit validation pass to verify candidate frequency thresholds
        count1 = 0;
        count2 = 0;
        for (int num : nums) {
            if (num == candidate1) count1++;
            else if (num == candidate2) count2++;
        }

        int threshold = static_cast<int>(nums.size()) / 3;
        if (count1 > threshold) result.push_back(candidate1);
        if (count2 > threshold) result.push_back(candidate2);

        // Optional convenience step: Sort the output structure to ensure deterministic results
        sort(result.begin(), result.end());

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The voting loop takes one linear pass, and the validation loop takes another. 
 *                  Sorting the final results vector takes constant time O(1) since it contains 
 *                  at most 2 elements.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The matching registers run completely in-place using scalar 
 *                   trackers, requiring zero extra dynamic memory allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid parameter. Array size cannot be negative." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter the array elements separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> nums[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nExecuting variant Boyer-Moore multi-candidate voting selection..." << endl;
    vector<int> majorities = solver.majorityElement(nums);

    cout << "Elements appearing strictly more than floor(N / 3) times:\n[ ";
    for (size_t i = 0; i < majorities.size(); ++i) {
        cout << majorities[i] << (i + 1 < majorities.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}