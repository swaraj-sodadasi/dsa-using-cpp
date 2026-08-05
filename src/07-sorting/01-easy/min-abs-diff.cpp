#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * SORTING WITH SINGLE-PASS MINIMUM DISTANCE TRACKING ENGINE
     * ---------------------------------------------------------
     * The objective is to find all pairs of elements with the minimum absolute 
     * difference in an array of distinct integers. The pairs must be returned 
     * in ascending order (both list order and element pair order).
     *
     * Algorithmic Strategy:
     * - The absolute difference between any two elements is minimized when they 
     *   are as close to each other as possible on the number line. 
     * - Sorting the array immediately aligns elements with their closest neighbors, 
     *   reducing our comparison candidates from O(N^2) global combinations to just 
     *   O(N) adjacent pairs.
     *
     * Tracking Protocol:
     * - Step 1: Sort the input vector `arr` in ascending order.
     * - Step 2: Initialize `minDiff = INT_MAX` to track the minimal gap encountered.
     * - Step 3: Run a single linear pass comparing adjacent elements `arr[i]` and `arr[i-1]`:
     *   - Calculate the current gap: `currentDiff = arr[i] - arr[i-1]`.
     *   - Case A: `currentDiff < minDiff`. We have found a new global minimum gap. 
     *     We reset our results matrix, update `minDiff = currentDiff`, and push the new pair.
     *   - Case B: `currentDiff == minDiff`. This pair matches our current minimal gap profile. 
     *     We push it directly into our results matrix.
     *   - Case C: `currentDiff > minDiff`. Skip the pair, as its gap is too large.
     */
    vector<vector<int>> minimumAbsDifference(vector<int>& arr) {
        // Step 1: Sort the data collection to place closest numerical neighbors adjacent to each other
        sort(arr.begin(), arr.end());

        vector<vector<int>> pairsResult;
        int minDiff = INT_MAX;
        size_t length = arr.size();

        // Step 2 & 3: Perform a single-pass minimum distance tracking sweep
        for (size_t i = 1; i < length; ++i) {
            int currentDiff = arr[i] - arr[i - 1];

            if (currentDiff < minDiff) {
                // A smaller absolute difference gap discovered; flush previous pairs and update threshold
                minDiff = currentDiff;
                pairsResult.clear();
                pairsResult.push_back({arr[i - 1], arr[i]});
            } else if (currentDiff == minDiff) {
                // Gap matches the active minimum threshold; record the pair profile configuration
                pairsResult.push_back({arr[i - 1], arr[i]});
            }
        }

        return pairsResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements inside the collection array. 
 *                  Sorting the sequence takes O(N log N) time. The subsequent linear distance 
 *                  tracking pass runs in O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sorting step works entirely in-place. Ignoring the output 
 *                   container space required to return results, the calculations consume zero extra 
 *                   dynamic memory heap spaces.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n < 2) {
        cout << "Invalid parameter. Array size must be at least 2." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter distinct integer array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting sorting with single-pass adjacent distance validation checks..." << endl;
    vector<vector<int>> minimalPairs = solver.minimumAbsDifference(arr);

    cout << "\nPairs isolating the absolute minimum element distance gap:\n[ ";
    for (size_t i = 0; i < minimalPairs.size(); ++i) {
        cout << "[" << minimalPairs[i][0] << ", " << minimalPairs[i][1] << "]" 
             << (i + 1 < minimalPairs.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}