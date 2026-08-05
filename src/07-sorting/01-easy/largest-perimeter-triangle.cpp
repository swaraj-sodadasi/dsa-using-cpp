#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * SORTING WITH GREEDY DEGENERATE TRIANGLE CHECK ENGINE
     * ----------------------------------------------------
     * The objective is to find the largest perimeter of a triangle that can be 
     * formed from an array of positive integer side lengths. If no valid triangle 
     * can be formed, return 0.
     *
     * Mathematical & Algorithmic Strategy:
     * - The Triangle Inequality Theorem states that three side lengths a, b, and c 
     *   (where a <= b <= c) form a valid non-degenerate triangle if and only if:
     *   a + b > c
     * - To maximize the perimeter (a + b + c), we should greedily test the largest 
     *   available side lengths first.
     *
     * Execution Protocol:
     * - Step 1: Sort the array in ascending order. This groups largest values at the tail.
     * - Step 2: Scan the array backwards starting from the largest triplet at the end: 
     *   c = nums[i], b = nums[i-1], a = nums[i-2].
     * - Step 3: Check the validation condition: `nums[i-2] + nums[i-1] > nums[i]`.
     *   - If this condition holds, this triplet is guaranteed to yield the absolute maximum 
     *     possible perimeter because `c` is the largest remaining value in the array. 
     *     We immediately return `a + b + c`.
     *   - If this condition fails, then no other elements smaller than `nums[i-2]` can 
     *     possibly satisfy the inequality with `nums[i-1]` and `nums[i]`. Thus, `nums[i]` 
     *     cannot be used as the longest side of any valid triangle. We shift our window 
     *     leftward to test the next candidate value for `c`.
     * - If the scan completes without finding a valid triplet, return 0.
     */
    int largestPerimeter(vector<int>& nums) {
        // Step 1: Sort the collection to position candidates in monotonic order
        sort(nums.begin(), nums.end());

        // Step 2: Run a greedy backwards scan testing adjacent triplets
        for (int i = static_cast<int>(nums.size()) - 1; i >= 2; --i) {
            size_t idxC = static_cast<size_t>(i);
            size_t idxB = static_cast<size_t>(i - 1);
            size_t idxA = static_cast<size_t>(i - 2);

            // Step 3: Verify the non-degenerate triangle condition
            if (nums[idxA] + nums[idxB] > nums[idxC]) {
                return nums[idxA] + nums[idxB] + nums[idxC];
            }
        }

        return 0; // No valid triangle configuration could be extracted
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements in the collection array. 
 *                  Sorting the side lengths constitutes the primary time bottleneck at O(N log N). 
 *                  The subsequent greedy triplet validation sweep runs in linear O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization strategy rearranges values completely 
 *                   in-place, requiring zero extra dynamic memory heap partitions.
 */
int main() {
    int n;
    cout << "Enter the total number of side length options available: ";
    if (!(cin >> n) || n < 3) {
        cout << "Invalid parameter. You must enter at least 3 side lengths." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter positive integer side lengths separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Side lengths must be positive integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting sorting and greedy non-degenerate triplet verification loops..." << endl;
    int maxPerimeterResult = solver.largestPerimeter(nums);

    if (maxPerimeterResult > 0) {
        cout << "Calculated maximum possible non-degenerate triangle perimeter: " << maxPerimeterResult << endl;
    } else {
        cout << "It is mathematically impossible to form a valid triangle from the given options." << endl;
    }

    return 0;
}