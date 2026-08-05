#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * PIGEONHOLE PRINCIPLE BASED BUCKET BOUNDARY SORTING ENGINE
     * ---------------------------------------------------------
     * The objective is to find the maximum difference between two successive elements 
     * in its sorted form, running in linear O(N) time and O(N) space.
     *
     * Mathematical & Algorithmic Strategy:
     * - A standard sorting approach takes O(N log N) time. To break the comparison-based lower 
     *   bound and achieve a strict linear O(N) runtime, we apply the Pigeonhole Principle.
     * - Given an array of $N$ elements with a global minimum `minVal` and global maximum `maxVal`, 
     *   the total range width is `totalRange = maxVal - minVal`.
     * - The absolute maximum possible gap must be at least $\lceil (maxVal - minVal) / (N - 1) \rceil$. 
     *   Let this minimal maximum gap size be `bucketSize`.
     * - We establish uniform buckets of size `bucketSize`. Because the gap between elements *inside* 
     *   the same bucket is strictly less than `bucketSize`, the maximum gap can never occur between 
     *   two elements in the same bucket.
     * - Therefore, the maximum gap *must* occur across bucket boundaries—specifically, between the 
     *   maximum value of a populated bucket and the minimum value of the next non-empty bucket.
     *
     * Verification Protocol:
     * - Step 1: Scan the array to find `minVal` and `maxVal`. If $N < 2$ or `minVal == maxVal`, return 0.
     * - Step 2: Compute `bucketSize = max(1, (maxVal - minVal) / (N - 1))` and the total number of 
     *   buckets: `bucketCount = (maxVal - minVal) / bucketSize + 1`.
     * - Step 3: Instantiate two parallel tracking structures (`bucketMin` initialized to `INT_MAX`, 
     *   `bucketMax` initialized to `INT_MIN`) along with a boolean mask `bucketVisited`.
     * - Step 4: Map each array element into its corresponding bucket slot: `idx = (num - minVal) / bucketSize`. 
     *   Update the bucket's min and max thresholds.
     * - Step 5: Iterate through the bucket array sequentially. Track the continuous gap between the current 
     *   bucket's minimum and the previous active bucket's maximum value, keeping track of the global maximum gap.
     */
    int maximumGap(vector<int>& nums) {
        size_t n = nums.size();
        if (n < 2) return 0;

        // Step 1: Track global minimum and maximum limits
        int minVal = INT_MAX;
        int maxVal = INT_MIN;
        for (int num : nums) {
            minVal = min(minVal, num);
            maxVal = max(maxVal, num);
        }

        // Base case: All elements in the list are identical
        if (minVal == maxVal) return 0;

        // Step 2: Calculate maximum minimum possible gap and bucket allocation sizing limits
        int bucketSize = max(1, (maxVal - minVal) / (static_cast<int>(n) - 1));
        size_t bucketCount = static_cast<size_t>((maxVal - minVal) / bucketSize) + 1;

        // Step 3: Initialize interval tracking structures
        vector<int> bucketMin(bucketCount, INT_MAX);
        vector<int> bucketMax(bucketCount, INT_MIN);
        vector<bool> bucketVisited(bucketCount, false);

        // Step 4: Distribute elements into targeted bucket intervals
        for (int num : nums) {
            size_t idx = static_cast<size_t>((num - minVal) / bucketSize);
            bucketMin[idx] = min(bucketMin[idx], num);
            bucketMax[idx] = max(bucketMax[idx], num);
            bucketVisited[idx] = true;
        }

        // Step 5: Scan across adjacent non-empty bucket boundaries to find the maximum gap
        int maxGapResult = 0;
        int previousMax = minVal; // Track the maximum value of the preceding populated bucket

        for (size_t i = 0; i < bucketCount; ++i) {
            if (!bucketVisited[i]) continue; // Skip unpopulated bucket nodes

            // Gap matches distance from current bucket minimum to preceding bucket maximum
            maxGapResult = max(maxGapResult, bucketMin[i] - previousMax);
            previousMax = bucketMax[i];
        }

        return maxGapResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements inside the collection array. 
 *                  Finding limits takes O(N) time. Distributing elements across buckets takes 
 *                  O(N) time. Finally, parsing across bucket bounds takes O(B) where B is proportional 
 *                  to N, cementing a linear runtime execution flow.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The pigeonhole structural mapping system instantiates parallel 
 *                   bucket tracking containers scaling up to at most N + 1 elements.
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
        cout << "Enter non-negative integer elements separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> nums[static_cast<size_t>(i)];
            if (nums[static_cast<size_t>(i)] < 0) {
                cout << "Constraint Error: Elements must be non-negative integers." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting pigeonhole partition boundary distance calculation loops..." << endl;
    int maxGapVal = solver.maximumGap(nums);

    cout << "Maximum absolute sorting sequence gap difference isolated: " << maxGapVal << endl;

    return 0;
}