#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
private:
    /*
     * MONOTONIC CAPACITY FEASIBILITY CHECKER
     * --------------------------------------
     * Determines whether the packages can be shipped within `days` given a maximum 
     * weight capacity per conveyor belt batch configuration.
     */
    bool isFeasible(const vector<int>& weights, int days, int capacity) {
        int currentDaysUsed = 1;
        int currentRunningLoad = 0;

        for (int weight : weights) {
            // If a single item exceeds the capacity bounds, this threshold allocation fails immediately
            if (weight > capacity) return false;

            if (currentRunningLoad + weight > capacity) {
                // Seal this partition container window and load the cargo onto a new day stream
                currentDaysUsed++;
                currentRunningLoad = weight;
                
                // If days threshold exceeds limits, abort exploration branch
                if (currentDaysUsed > days) return false;
            } else {
                currentRunningLoad += weight;
            }
        }
        return true;
    }

public:
    /*
     * MONOTONIC CAPACITY ALLOCATION CHECKING ENGINE
     * ---------------------------------------------
     * The objective is to calculate the minimum weight capacity of a ship that will result 
     * in all the packages on the conveyor belt being shipped within `days` days.
     *
     * Mathematical & Search Framework:
     * - The required shipping capacity exhibits a monotonic trend layout properties: 
     *   If a capacity `C` can successfully ship all cargo within the target days, 
     *   any capacity greater than `C` will also succeed. Conversely, if it fails, 
     *   any lower capacity is guaranteed to fail.
     * - This observation allows us to search for the ideal minimum capacity using 
     *   binary search across the potential answer range, rather than a linear scan.
     *
     * Boundary Initialization Protocol:
     * - Lower Bound (`left`): The single heaviest item in `weights`. The ship must be 
     *   at least large enough to carry this package alone.
     * - Upper Bound (`right`): The sum of all elements in `weights`. This is the capacity 
     *   needed to ship everything together in a single day.
     *
     * Convergence Optimization Path:
     * - Midpoint Candidate: `mid = left + (right - left) / 2`.
     * - If `isFeasible(mid)` holds true, `mid` is registered as a valid solution candidate. 
     *   We then shrink the window from the right to search for a smaller viable capacity option: 
     *   `right = mid - 1`.
     * - If false, the capacity is insufficient. We increase the lower bound search boundary: 
     *   `left = mid + 1`.
     */
    int shipWithinDays(vector<int>& weights, int days) {
        if (weights.empty()) return 0;

        // Establish boundaries: left is the maximum single item, right is the total sum load
        int left = *max_element(weights.begin(), weights.end());
        int right = accumulate(weights.begin(), weights.end(), 0);
        int minimumCapacityCandidate = right;

        // Execute binary search across the monotonic target capacity interval space
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (isFeasible(weights, days, mid)) {
                minimumCapacityCandidate = mid; // Register the valid capacity candidate
                right = mid - 1;               // Try to find a tighter, lower capacity target bounds
            } else {
                left = mid + 1;                // Insufficient space allocation; scale lower bound upwards
            }
        }

        return minimumCapacityCandidate;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * log(SumWeights - MaxWeight)) - Where N represents the total package items. 
 *                  The binary search space spans between the maximum single weight and total sum weight. 
 *                  Each step invokes an O(N) linear iteration scan verification loop.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The feasibility bounds check runs completely in-place 
 *                   using simple local iteration values, requiring zero extra frame dynamic buffers.
 */
int main() {
    int n;
    cout << "Enter the total number of cargo packages on the conveyor stream list: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The collection size must be greater than 0." << endl;
        return 1;
    }

    vector<int> weights(static_cast<size_t>(n));
    cout << "Enter the weight sequence configuration (positive integers) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> weights[static_cast<size_t>(i)];
        if (weights[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Cargo items must hold non-zero positive weight properties." << endl;
            return 1;
        }
    }

    int days;
    cout << "Enter the targeted scheduling threshold limit parameter (Days): ";
    if (!(cin >> days) || days <= 0) {
        cout << "Invalid parameter. Days limit must be greater than 0." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting monotonic capacity allocation binary search tracking pass..." << endl;
    int optimalCapacityResult = solver.shipWithinDays(weights, days);

    cout << "Minimum required vessel load capacity discovered: " << optimalCapacityResult << " units." << endl;

    return 0;
}