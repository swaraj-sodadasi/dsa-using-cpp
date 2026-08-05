#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Solution {
private:
    /*
     * LOMUTO PARTITIONING SUBSYSTEM
     * -----------------------------
     * Rearranges elements around a chosen pivot inside the range [left, right].
     * Elements smaller than or equal to the pivot are shifted to the left, 
     * and elements greater than the pivot are shifted to the right.
     */
    int partition(vector<int>& nums, int left, int right) {
        // Randomized selection mechanics to mitigate worst-case performance spikes
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(left, right);
        int pivotIndex = dis(gen);
        
        // Relocate pivot value safely out of the way to the right boundary
        swap(nums[static_cast<size_t>(pivotIndex)], nums[static_cast<size_t>(right)]);
        int pivot = nums[static_cast<size_t>(right)];
        
        int i = left;
        for (int j = left; j < right; ++j) {
            if (nums[static_cast<size_t>(j)] <= pivot) {
                swap(nums[static_cast<size_t>(i)], nums[static_cast<size_t>(j)]);
                i++;
            }
        }
        
        // Restore the pivot value back to its final validated sorted coordinate position
        swap(nums[static_cast<size_t>(i)], nums[static_cast<size_t>(right)]);
        return i;
    }

public:
    /*
     * QUICK-SELECT PARTITIONING OPTIMIZATION ENGINE
     * ---------------------------------------------
     * The objective is to find the k-th largest element in an unsorted array.
     * Note that it is the k-th largest element in sorted order, not the k-th distinct element.
     *
     * Algorithmic Strategy:
     * - Instead of fully sorting the collection in O(N log N) time, we apply the Quick-Select 
     *   algorithm (based on Hoare's Selection scheme) to achieve an average runtime of O(N).
     * - Finding the k-th largest element is mathematically equivalent to finding the element at 
     *   index `targetIndex = N - k` in a standard ascending sorted array.
     *
     * Selection Protocol Loop:
     * - In each iteration, we select a random pivot and partition the array around it. 
     * - After partitioning, the pivot settles into its exact final sorted index `pivotIdx`.
     * - Optimization Choice:
     *   - Case A: If `pivotIdx == targetIndex`, the pivot is our target. Return it immediately.
     *   - Case B: If `pivotIdx > targetIndex`, our target lies in the left partition. 
     *     We drop the right side and contract our search frame: `right = pivotIdx - 1`.
     *   - Case C: If `pivotIdx < targetIndex`, our target lies in the right partition. 
     *     We drop the left side and scale our search frame: `left = pivotIdx + 1`.
     */
    int findKthLargest(vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        int targetIndex = n - k; // Map k-th largest element to its corresponding ascending index position
        
        int left = 0;
        int right = n - 1;

        // Perform Quick-Select window refinements until target is captured
        while (left <= right) {
            int pivotIdx = partition(nums, left, right);

            if (pivotIdx == targetIndex) {
                return nums[static_cast<size_t>(pivotIdx)];
            } else if (pivotIdx > targetIndex) {
                right = pivotIdx - 1; // Narrow lookups downward to lower segments
            } else {
                left = pivotIdx + 1;  // Adjust lower search boundaries upward
            }
        }

        return -1; // Fallback return statement
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) Average Case - In each selection phase, the randomized partitioning 
 *                  halves the remaining array space on average, yielding the geometric sum 
 *                  series: N + N/2 + N/4 + ... which converges to 2N operations.
 *                  Worst Case: O(N^2) if pivot choices consistently select extreme elements, 
 *                  mitigated significantly by random uniform pivot shuffling.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sequence shifts partitions iteratively and completely 
 *                   in-place, requiring zero recursion tracking frames or dynamic allocation matrices.
 */
int main() {
    int n;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int k;
    cout << "Enter the target rank value to isolate (Kth largest, where 1 <= K <= N): ";
    if (!(cin >> k) || k < 1 || k > n) {
        cout << "Constraint Error: Rank value parameter falls outside allowable range bounds." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting randomized Quick-Select interval partitioning..." << endl;
    int resultVal = solver.findKthLargest(nums, k);

    cout << "The " << k << "-th largest element found in the collection: " << resultVal << endl;

    return 0;
}