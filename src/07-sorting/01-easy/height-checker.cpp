#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * SORTING COMPARISON VS. FREQUENCY DISCREPANCY COUNTING ENGINE
     * ------------------------------------------------------------
     * The objective is to return the number of indices where heights[i] != expected[i], 
     * where expected[i] is the heights array sorted in non-decreasing order.
     *
     * Algorithmic Choices:
     * - Option A (Sorting Comparison): Clone and sort the array, running in O(N log N) time 
     *   and requiring O(N) space.
     * - Option B (Frequency Discrepancy Counting): Since heights are bounded between 1 and 100, 
     *   we can optimize to O(N) time and O(1) auxiliary space using an in-place counting sort bucket strategy.
     *
     * Counting Bucket Verification Protocol:
     * - Step 1: Count the frequency of each height using a fixed array of 101 slots.
     * - Step 2: Iterate through the original heights array. For each element, find the correct expected 
     *   height by scanning through our frequency buckets.
     * - Step 3: If the current element does not match the bucket height it lands in, increment our 
     *   discrepancy counter. Decrement the bucket count to move down the line.
     */
    int heightChecker(vector<int>& heights) {
        // Step 1: Record frequency distributions inside constant bucket constraints
        vector<int> heightCounts(101, 0);
        for (int h : heights) {
            heightCounts[static_cast<size_t>(h)]++;
        }

        int mismatchCount = 0;
        size_t currentBucketHeight = 1;

        // Step 2: Compare each height position against the expected bucket configurations
        for (int currentActualHeight : heights) {
            // Advance to the next available height bucket that contains elements
            while (heightCounts[currentBucketHeight] == 0) {
                currentBucketHeight++;
            }

            // Step 3: If actual height misaligns with bucket profile, increment mismatch count
            if (currentActualHeight != static_cast<int>(currentBucketHeight)) {
                mismatchCount++;
            }

            // Consume one instance from the current target height bucket
            heightCounts[currentBucketHeight]--;
        }

        return mismatchCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements in the heights collection. 
 *                  Building the buckets takes O(N) time. The subsequent matching pass increments 
 *                  through a fixed total number of 100 buckets, running in linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization uses a fixed-size count array of 101 elements, 
 *                   which consumes the same minimal stack space regardless of the input array size.
 */
int main() {
    int n;
    cout << "Enter the total number of students in the lineup: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Collection size must be greater than 0." << endl;
        return 1;
    }

    vector<int> heights(static_cast<size_t>(n));
    cout << "Enter the height values (integers between 1 and 100) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> heights[static_cast<size_t>(i)];
        if (heights[static_cast<size_t>(i)] < 1 || heights[static_cast<size_t>(i)] > 100) {
            cout << "Constraint Error: Height parameters must fall within the range [1, 100]." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting frequency discrepancy counting checker pass..." << endl;
    int discrepancies = solver.heightChecker(heights);

    cout << "Total height alignment mismatch index count: " << discrepancies << endl;

    return 0;
}