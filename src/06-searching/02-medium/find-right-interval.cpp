#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * STRUCTURAL INTERVAL ENTRY CONTAINER
     * -----------------------------------
     * Captures the structural footprint of an interval, preserving its original 
     * indexing position before sort transformations shuffle the sequence layouts.
     */
    struct IntervalEntry {
        int startValue;
        int originalIndex;

        // Custom comparator to sort intervals based primarily on their start coordinates
        bool operator<(const IntervalEntry& other) const {
            return startValue < other.startValue;
        }
    };

public:
    /*
     * SORTING WITH BINARY SEARCH UPPER BOUND MAPPING ENGINE
     * -----------------------------------------------------
     * The objective is to find the "right interval" for each interval in a given array. 
     * The right interval for interval `i` is the interval `j` that has the minimum start 
     * point such that `start_j >= end_i`. Return an array containing the indices of the 
     * right intervals, or -1 if none exist.
     *
     * Algorithmic Strategy:
     * - A brute-force nested loop approach takes O(N^2) time. We optimize this to O(N log N) 
     *   by using binary search to find the lower bound index of the valid start values.
     * - Step 1: Extract all interval start values along with their original indices into 
     *   a list of `IntervalEntry` objects.
     * - Step 2: Sort this list in ascending order based on the `startValue` field.
     * - Step 3: For each original interval, query its `end` point against our sorted list 
     *   using a custom binary search algorithm. This search aims to find the first entry where 
     *   `startValue >= currentEnd`.
     *
     * Binary Search Bounding Protocol:
     * - Initialize `left = 0` and `right = N`.
     * - Midpoint Calculation: `mid = left + (right - left) / 2`.
     * - If `sortedStarts[mid].startValue >= currentEnd`, this is a valid target candidate. 
     *   We contract the window from the right to see if a smaller valid start value exists: `right = mid`.
     * - If `sortedStarts[mid].startValue < currentEnd`, the start value is too small. We shift 
     *   the lower bound past the midpoint: `left = mid + 1`.
     * - Upon loop termination, if `left` points within the vector boundaries, it isolates the 
     *   optimal right interval. We retrieve its `originalIndex`. Otherwise, we return -1.
     */
    vector<int> findRightInterval(vector<vector<int>>& intervals) {
        int n = static_cast<int>(intervals.size());
        vector<IntervalEntry> sortedStarts(static_cast<size_t>(n));

        // Step 1: Record initial start configurations paired with original indices
        for (int i = 0; i < n; ++i) {
            sortedStarts[static_cast<size_t>(i)] = {intervals[static_cast<size_t>(i)][0], i};
        }

        // Step 2: Sort the data collection to enable binary search upper bound mapping
        sort(sortedStarts.begin(), sortedStarts.end());

        vector<int> rightIntervalsResult(static_cast<size_t>(n));

        // Step 3: Map each interval's end point to the smallest matching start value
        for (int i = 0; i < n; ++i) {
            int currentEnd = intervals[static_cast<size_t>(i)][1];

            int left = 0;
            int right = n;
            
            // Execute custom binary search to locate the lower bound index position
            while (left < right) {
                int mid = left + (right - left) / 2;

                if (sortedStarts[static_cast<size_t>(mid)].startValue >= currentEnd) {
                    right = mid; // Candidate found; compress upper bound to check for smaller starts
                } else {
                    left = mid + 1; // Start value is too small; shift search window right
                }
            }

            // If left matches n, no interval started late enough to catch currentEnd
            if (left == n) {
                rightIntervalsResult[static_cast<size_t>(i)] = -1;
            } else {
                rightIntervalsResult[static_cast<size_t>(i)] = sortedStarts[static_cast<size_t>(left)].originalIndex;
            }
        }

        return rightIntervalsResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total intervals in the collection array. 
 *                  Building and sorting the entry array takes O(N log N) time. The subsequent 
 *                  mapping loop performs N decoupled binary searches, taking O(N log N) time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization mapping algorithm instantiates a copy 
 *                   of the starting coordinates array to perform in-place sorting transformations.
 */
int main() {
    int n;
    cout << "Enter the total number of intervals in the collection: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Collection size must be greater than 0." << endl;
        return 1;
    }

    vector<vector<int>> intervals(static_cast<size_t>(n), vector<int>(2));
    cout << "Enter the intervals as [start end] coordinate pairs row by row:\n";
    for (int i = 0; i < n; ++i) {
        cin >> intervals[static_cast<size_t>(i)][0] >> intervals[static_cast<size_t>(i)][1];
        if (intervals[static_cast<size_t>(i)][0] > intervals[static_cast<size_t>(i)][1]) {
            cout << "Constraint Error: Interval start value cannot exceed its end value." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting sorting with binary search upper bound mapping sweep..." << endl;
    vector<int> outcome = solver.findRightInterval(intervals);

    cout << "\nCalculated right interval index mapping array:\n[ ";
    for (size_t i = 0; i < outcome.size(); ++i) {
        cout << outcome[i] << (i + 1 < outcome.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}