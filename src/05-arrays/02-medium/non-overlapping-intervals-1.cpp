#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * GREEDY INTERVAL SCHEDULING ENGINE (BASED ON END TIMES)
     * -----------------------------------------------------
     * The objective is to find the minimum number of intervals to remove so that 
     * the remaining intervals do not overlap.
     *
     * Algorithmic Optimization Strategy (Greedy Choice):
     * - Minimizing deletions is mathematically equivalent to maximizing the number of 
     *   non-overlapping intervals we keep (Classic Interval Scheduling Problem).
     * - To fit as many intervals as possible into the timeline, we should always pick the 
     *   interval that **finishes earliest**. By minimizing the finish time, we maximize 
     *   the remaining open space left for future intervals.
     *
     * State Window Evaluation Sequence:
     * - Step 1: Sort the intervals by their **end times** in ascending order. If two intervals 
     *   share the same end time, their start time order does not affect the correctness.
     * - Step 2: Initialize `nonOverlappingCount = 1` (taking the first sorted interval) and 
     *   track its end time in `currentEnd`.
     * - Step 3: Iterate through the remaining intervals from index 1 to the end:
     *   - If the current interval's start time is greater than or equal to `currentEnd`, it does 
     *     not overlap. We accept this interval, increment `nonOverlappingCount`, and update `currentEnd` 
     *     to this interval's end time.
     *   - If its start time is less than `currentEnd`, it overlaps. We must drop it.
     * - Step 4: The final minimum removals required is: `totalIntervals - nonOverlappingCount`.
     */
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0;

        // Step 1: Sort the intervals by their end times (index 1) in ascending order
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        int nonOverlappingCount = 1;
        int currentEnd = intervals[0][1];
        size_t n = intervals.size();

        // Step 3: Forward sweep to collect non-overlapping intervals greedily
        for (size_t i = 1; i < n; ++i) {
            // If the start time is outside the current tracking boundary, no overlap occurs
            if (intervals[i][0] >= currentEnd) {
                nonOverlappingCount++;
                currentEnd = intervals[i][1]; // Advance the active end-time horizon anchor
            }
        }

        // Step 4: Minimum removals is total intervals minus maximum valid kept intervals
        return static_cast<int>(n) - nonOverlappingCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total number of intervals in the collection.
 *                  Sorting the array dominates the execution timeline taking O(N log N) time. The 
 *                  subsequent greedy evaluation sweep checks each interval exactly once in linear O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sorting step works directly on the input structure (or uses O(log N) 
 *                   stack space inside `std::sort`), requiring no extra dynamic tracking buffers on the heap.
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
    cout << "\nExecuting greedy interval scheduling sweep based on end times..." << endl;
    int itemsRemoved = solver.eraseOverlapIntervals(intervals);

    cout << "Minimum overlapping intervals required to be pruned away: " << itemsRemoved << endl;

    return 0;
}