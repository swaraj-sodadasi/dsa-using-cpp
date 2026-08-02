#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * SORTING-BASED OVERLAPPING BOUNDARY CONSOLIDATION ENGINE
     * -------------------------------------------------------
     * The objective is to merge all overlapping intervals and return an array 
     * of the non-overlapping intervals that cover all the input intervals.
     *
     * Algorithmic Optimization Strategy:
     * - By sorting the intervals based on their start times, we guarantee that 
     *   any intervals that could potentially merge are positioned adjacent to each other.
     * - This transforms a complex matching problem into a simple single-pass linear sweep.
     *
     * State Boundary Consolidation Sequence:
     * - Step 1: Sort the intervals vector by their start times (`intervals[i][0]`) in ascending order.
     * - Step 2: Initialize the `mergedResult` collection by pushing the first sorted interval into it. 
     *   This serves as our active tracking boundary anchor.
     * - Step 3: Iterate sequentially through the remaining intervals from index 1:
     *   - Access the last inserted interval in our result pool (`mergedResult.back()`).
     *   - **Condition A (Overlap):** If the current interval's start time is less than or equal to 
     *     the tracking interval's end time (`intervals[i][0] <= mergedResult.back()[1]`), an overlap exists. 
     *     We consolidate the boundary by updating the tracking end time to the maximum of both: 
     *     `mergedResult.back()[1] = max(mergedResult.back()[1], intervals[i][1])`.
     *   - **Condition B (No Overlap):** If the start time is strictly greater than the tracking interval's 
     *     end time, they are disjoint. We push the current interval directly into `mergedResult` to serve 
     *     as the new tracking anchor.
     */
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};

        // Step 1: Sort intervals by start times in ascending order
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0];
        });

        vector<vector<int>> mergedResult;
        
        // Step 2: Seed the output structure with the initial baseline interval
        mergedResult.push_back(intervals[0]);

        size_t n = intervals.size();

        // Step 3: Forward linear sweep to consolidate boundaries in place
        for (size_t i = 1; i < n; ++i) {
            // Check if the current interval overlaps with the last tracked merged interval
            if (intervals[i][0] <= mergedResult.back()[1]) {
                // Consolidate the upper limit boundary to include the overlapping segment range
                mergedResult.back()[1] = max(mergedResult.back()[1], intervals[i][1]);
            } else {
                // Disjoint interval discovered -> push directly as a new baseline target tracking slot
                mergedResult.push_back(intervals[i]);
            }
        }

        return mergedResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total number of intervals in the input pool.
 *                  The sorting process dominates the timeline consuming O(N log N) time steps. The 
 *                  subsequent unification sweep evaluates each interval exactly once in linear O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization updates operate directly on the elements or 
 *                   use standard execution stack allocations within `std::sort` without allocating 
 *                   auxiliary tracking tables on the heap.
 */
int main() {
    int n;
    cout << "Enter the total number of intervals: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The interval pool count must be greater than 0." << endl;
        return 1;
    }

    vector<vector<int>> intervals(static_cast<size_t>(n), vector<int>(2));
    cout << "Enter intervals as 'start end' pairs separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> intervals[static_cast<size_t>(i)][0] >> intervals[static_cast<size_t>(i)][1];
        if (intervals[static_cast<size_t>(i)][0] > intervals[static_cast<size_t>(i)][1]) {
            cout << "Constraint Error: Interval start time cannot be greater than end time." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting sorting-based overlapping boundary consolidation..." << endl;
    vector<vector<int>> result = solver.merge(intervals);

    cout << "\nConsolidated Non-Overlapping Interval Configurations:\n[\n";
    for (const auto& interval : result) {
        cout << "  [" << interval[0] << ", " << interval[1] << "]\n";
    }
    cout << "]\n";
    cout << "Total distinct consolidated segments remaining: " << result.size() << endl;

    return 0;
}