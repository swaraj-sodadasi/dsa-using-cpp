#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Solution {
private:
    /*
     * FRACTIONAL CAPACITY FEASIBILITY EVALUATOR
     * -----------------------------------------
     * Determines if it is mathematically possible to achieve a maximum distance 
     * constraint `maxDist` between any adjacent gas stations by adding at most `k` new stations.
     *
     * Mechanics:
     * - For each existing gap between `stations[i]` and `stations[i-1]`, we calculate 
     *   how many stations are needed so that no single segment inside that gap exceeds `maxDist`.
     * - The number of stations required within a given `gap` is `floor(gap / maxDist)`.
     * - Using continuous floating-point arithmetic helps avoid truncation problems: `static_cast<int>(gap / maxDist)`.
     */
    bool isFeasible(const vector<int>& stations, int k, double maxDist) {
        int requiredStations = 0;
        int n = static_cast<int>(stations.size());

        for (int i = 1; i < n; ++i) {
            double gap = stations[static_cast<size_t>(i)] - stations[static_cast<size_t>(i - 1)];
            
            // Add the count of internal stations required within this gap boundary
            requiredStations += static_cast<int>(gap / maxDist);

            // Early pruning optimization: abort if the station budget is exceeded
            if (requiredStations > k) {
                return false;
            }
        }
        return true;
    }

public:
    /*
     * FRACTIONAL FLOATING-POINT BINARY SEARCH ENGINE
     * ----------------------------------------------
     * The objective is to add `k` new gas stations along a horizontal line such that 
     * the maximum distance `d` between adjacent stations is minimized.
     *
     * Mathematical & Algorithmic Strategy:
     * - Unlike standard integer-bound binary searches, this problem requires finding a continuous 
     *   floating-point value. The decision space is still monotonic, enabling binary search.
     * - **Termination Protocol:** Instead of loop conditions like `left <= right` or `left < right`, 
     *   which cause infinite loops on real numbers, we terminate using a precision delta: 
     *   `while (right - left > 1e-7)`. Alternatively, a fixed-iteration loop (e.g., 80 iterations) 
     *   can guarantee micro-precision bounds.
     *
     * Boundary Initialization:
     * - Lower Bound (`left`): 0.0 (The absolute best case scenario where stations overlap).
     * - Upper Bound (`right`): The maximum initial gap between any two adjacent stations. 
     *   The answer cannot exceed the current largest penalty gap.
     */
    double minmaxGasDist(vector<int>& stations, int k) {
        double left = 0.0;
        double right = 0.0;
        int n = static_cast<int>(stations.size());

        // Discover the maximum initial distance gap between adjacent nodes
        for (int i = 1; i < n; ++i) {
            double gap = stations[static_cast<size_t>(i)] - stations[static_cast<size_t>(i - 1)];
            if (gap > right) {
                right = gap;
            }
        }

        // Perform real-number binary search until our search space matches the precision metric
        while (right - left > 1e-7) {
            double mid = left + (right - left) / 2.0;

            if (isFeasible(stations, k, mid)) {
                // The current mid distance can be achieved; try to compress the bound further to the left
                right = mid;
            } else {
                // The current max distance is too small to fit the station limits; branch to the right
                left = mid;
            }
        }

        // Convergence point guarantees left and right meet within our chosen precision thresholds
        return left;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * log((MaxGap) / Precision)) - Where N represents the total elements inside 
 *                  the stations collection. The binary search space requires log_2(MaxGap / 10^-7) 
 *                  steps to converge, with each interval validating the entire collection linearly.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The fractional logic tracks parameters using 
 *                   scalar floating-point registers, running entirely in-place.
 */
int main() {
    int n;
    cout << "Enter the total number of initial gas stations: ";
    if (!(cin >> n) || n < 2) {
        cout << "Invalid parameter. Array size must be at least 2." << endl;
        return 1;
    }

    vector<int> stations(static_cast<size_t>(n));
    cout << "Enter the station positions on the coordinate line in strictly sorted order:\n";
    for (int i = 0; i < n; ++i) {
        cin >> stations[static_cast<size_t>(i)];
    }

    // Constraint validation ensuring positional arrays are correctly organized
    if (!is_sorted(stations.begin(), stations.end())) {
        cout << "Constraint Error: Elements must be entered in sorted order." << endl;
        return 1;
    }

    int k;
    cout << "Enter the total count of additional new gas stations to construct (K): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid parameter. K must be a positive integer value." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting fractional floating-point binary search boundary updates..." << endl;
    double optimalDistanceResult = solver.minmaxGasDist(stations, k);

    // Render results to high-precision formatting limits
    cout << fixed << setprecision(6);
    cout << "Calculated minimum possible maximum distance gap: " << optimalDistanceResult << endl;

    return 0;
}