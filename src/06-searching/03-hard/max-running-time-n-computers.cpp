#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * GREEDY BATTERY ALLOCATION CHECKER
     * ---------------------------------
     * Evaluates if it is mathematically possible to run `n` computers simultaneously 
     * for a target duration of `targetTime` minutes using the available batteries.
     * 
     * Greedy Strategy:
     * - A single computer can utilize at most `targetTime` minutes from any single battery, 
     *   since a battery cannot be plugged into multiple computers at the exact same instant.
     * - Therefore, for each battery in the pool:
     *   - If its total capacity exceeds `targetTime`, it can fully support one computer for 
     *     the entire duration. We cap its contribution at `targetTime`.
     *   - If its capacity is less than `targetTime`, its entire raw charge is pooled together 
     *     as shareable modular energy to fill out the remaining runtime deficits.
     * - If the cumulative capped energy from all batteries is greater than or equal to the total 
     *   workload demand (`n * targetTime`), the configuration threshold is valid.
     */
    bool isFeasible(int n, const vector<int>& batteries, long long targetTime) {
        long long totalPooledEnergy = 0;
        long long energyRequirement = static_cast<long long>(n) * targetTime;

        for (int capacity : batteries) {
            // Cap the contribution of any individual battery at targetTime minutes
            totalPooledEnergy += min(static_cast<long long>(capacity), targetTime);
            
            // Optimization pruning: return true as soon as the total pooled charge clears the baseline requirement
            if (totalPooledEnergy >= energyRequirement) {
                return true;
            }
        }

        return totalPooledEnergy >= energyRequirement;
    }

public:
    /*
     * GREEDY ALLOCATION STRATEGY + BINARY SEARCH ENGINE
     * --------------------------------------------------
     * The objective is to determine the maximum number of minutes `n` computers can 
     * run simultaneously given an integer array of battery capacities.
     * 
     * Mathematical & Search Framework:
     * - The total achievable continuous running time exhibits monotonic scaling traits: 
     *   If the computers can run simultaneously for `T` minutes, they are structurally 
     *   guaranteed to run for any time step less than `T`. If they fail to hit `T`, then 
     *   any duration greater than `T` is impossible.
     * - This monotonic property allows us to execute a Binary Search on the Answer Range 
     *   to safely discover the global maximum runtime limit.
     *
     * Boundary Initialization Protocol:
     * - Lower Bound (`left`): 0 minutes.
     * - Upper Bound (`right`): The sum of all battery capacities divided by `n`. The absolute maximum 
     *   theoretical limit cannot exceed a perfectly even redistribution of the raw total energy pool.
     */
    long long maxRunTime(int n, vector<int>& batteries) {
        long long left = 0;
        long long totalEnergySum = 0;
        
        for (int capacity : batteries) {
            totalEnergySum += capacity;
        }

        long long right = totalEnergySum / n;
        long long optimizedMaximalRuntime = 0;

        // Execute binary search across the monotonic target runtime spectrum
        while (left <= right) {
            long long mid = left + (right - left) / 2;

            if (isFeasible(n, batteries, mid)) {
                optimizedMaximalRuntime = mid; // Register the valid runtime target candidate
                left = mid + 1;                // Attempt to expand the upper boundary limit toward higher values
            } else {
                right = mid - 1;               // Energy allocation failed; compress search frame down
            }
        }

        return optimizedMaximalRuntime;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(M * log(SumBatteries / N)) - Where M represents the total number of batteries. 
 *                  The binary search window parameters cover a maximum duration domain boundary. 
 *                  Each refinement step executes a linear O(M) greedy contribution calculation loop.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization evaluation calculates constraints completely 
 *                   in-place using local scalar loop indices, requiring zero dynamic storage.
 */
int main() {
    int n, m;
    cout << "Enter the total number of computers needing simultaneous runtime (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Computer count must be a positive integer value." << endl;
        return 1;
    }

    cout << "Enter the total number of available batteries: ";
    if (!(cin >> m) || m < n) {
        cout << "Constraint Error: Battery count must be at least equal to the number of computers." << endl;
        return 1;
    }

    vector<int> batteries(static_cast<size_t>(m));
    cout << "Enter the specific capacities of the batteries separated by spaces:\n";
    for (int i = 0; i < m; ++i) {
        cin >> batteries[static_cast<size_t>(i)];
        if (batteries[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Capacities must be non-zero positive integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting greedy energy allocation checks with binary search answer tracking..." << endl;
    long long maximumRuntimeResult = solver.maxRunTime(n, batteries);

    cout << "Calculated absolute maximum simultaneous computer runtime: " << maximumRuntimeResult << " minutes." << endl;

    return 0;
}