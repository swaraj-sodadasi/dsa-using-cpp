#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (RECURSIVE BUCKET FILLING WITH PRUNING OPTIMIZATION):
- The problem asks us to determine if an array of integers can be partitioned into k non-empty subsets whose sums are all equal.
- **Mathematical Preconditions**:
  - The total sum of the array must be perfectly divisible by $k$. If `totalSum % k != 0`, partitioning is impossible $\rightarrow$ return `false`.
  - The target sum for each individual subset (bucket) is defined as `target = totalSum / k`.
  - If any single element in the input array exceeds `target`, it cannot fit into any bucket $\rightarrow$ return `false`.
- **Recursive Bucket Filling Strategy**:
  - We can visualize this problem as dynamically filling $k$ distinct buckets, each aiming to accumulate exactly `target` sum.
  - We sort the array in descending order. This is a critical heuristic optimization: placing larger numbers first forces early failures on invalid paths, significantly cutting down the search space.
  - The recursive helper function `canPartition` processes elements one by one from index `numsIndex = 0` down to the end of the array.
  - For the current element `nums[numsIndex]`, we loop through each of the $k$ buckets:
    - If the bucket has enough space (`buckets[i] + nums[numsIndex] <= target`), we provisionally add the element to `buckets[i]`.
    - We then recurse to process the next element: `canPartition(numsIndex + 1)`.
    - If that choice path fails to yield a solution, we remove the element from `buckets[i]` (Backtracking) and try the next bucket.
- **Key Pruning Optimizations**:
  - **Optimization 1 (Descending Sort)**: Covered above. Helps hit the capacity limits faster.
  - **Optimization 2 (Skip Duplicate Buckets)**: If `buckets[i] == 0`, trying this element in subsequent empty buckets will produce identical search states. Thus, if a bucket is empty and the element fails, we break out of the loop immediately.
  - **Optimization 3 (Skip Redundant States)**: If adding the element makes `buckets[i]` match a value we already checked in this step, we skip it.

TIME COMPLEXITY: O(k^N) worst-case - Where N is the total number of elements. Each element has up to k choices. However, sorting and structural pruning drastically reduce this in practice.
SPACE COMPLEXITY: O(N) auxiliary - Bounded by the maximum recursion depth, which matches the size of the array N, alongside O(k) for bucket storage trackers.
*/

class Solution {
private:
    bool canPartition(const vector<int>& nums, size_t numsIndex, vector<int>& buckets, int target) {
        // Base Case: All elements have been successfully distributed into buckets
        if (numsIndex == nums.size()) {
            return true;
        }

        // Attempt to place the current element into one of the k buckets
        for (size_t i = 0; i < buckets.size(); ++i) {
            // Pruning check: Optimization 3 - Skip redundant search spaces for buckets with identical current sums
            if (i > 0 && buckets[i] == buckets[i - 1]) {
                continue;
            }

            // Check if the current element fits within the target constraint of the bucket
            if (buckets[i] + nums[numsIndex] <= target) {
                // Place element into bucket
                buckets[i] += nums[numsIndex];

                // Move forward recursively to assign the next element
                if (canPartition(nums, numsIndex + 1, buckets, target)) {
                    return true;
                }

                // Backtracking state recovery step
                buckets[i] -= nums[numsIndex];
            }

            // Pruning check: Optimization 2 - If this bucket was empty and the element failed, 
            // putting it in subsequent empty buckets will yield the exact same failure.
            if (buckets[i] == 0) {
                break;
            }
        }

        return false;
    }

public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        if (nums.empty() || k <= 0) return false;
        if (k == 1) return true; // Trivial condition: everything fits in 1 subset

        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        
        // Base Filter: If total sum isn't divisible by k, equal partitioning is impossible
        if (totalSum % k != 0) {
            return false;
        }

        int target = totalSum / k;

        // Optimization 1: Sort in descending order to process larger values first
        sort(nums.rbegin(), nums.rend());

        // Base Filter: If the largest element exceeds target, no valid distribution exists
        if (nums[0] > target) {
            return false;
        }

        // Instantiate k tracking buckets initialized to zero sum
        vector<int> buckets(static_cast<size_t>(k), 0);
        
        return canPartition(nums, 0, buckets, target);
    }
};

int main() {
    int n, k;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid sizing parameter configuration." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the integer array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the number of target subsets (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid subset quantity constraint parameter." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting recursive bucket filling with pruning optimization..." << endl;
    
    if (solver.canPartitionKSubsets(nums, k)) {
        cout << "Result: The array can be partitioned into " << k << " equal sum subsets." << endl;
    } else {
        cout << "Result: The array CANNOT be partitioned into " << k << " equal sum subsets." << endl;
    }

    return 0;
}