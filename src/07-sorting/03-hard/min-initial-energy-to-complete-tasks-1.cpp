#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * GREEDY SORT BASED ON VARIANCE DEFICIT ENGINE
     * --------------------------------------------
     * The objective is to find the minimum initial energy required to complete 
     * a series of tasks where each task i requires minimum[i] energy to start 
     * and consumes actual[i] energy upon completion.
     *
     * Algorithmic & Greedy Strategy:
     * - Let the "variance deficit" of a task be defined as (minimum[i] - actual[i]). 
     *   This represents the absolute buffer energy that must be present but is 
     *   not permanently spent.
     * - To minimize our total starting capital, we should prioritize tasks that 
     *   require a large buffer early in the process. The energy held to satisfy 
     *   a large threshold buffer early on can cover the actual energy costs of 
     *   subsequent tasks.
     * - Therefore, we sort the tasks in descending order of (minimum[i] - actual[i]).
     *
     * Execution Protocol:
     * - Step 1: Sort the task matrix using a custom comparator evaluating the deficit.
     * - Step 2: Track two running scalar variables:
     *     - `currentEnergy`: The energy left at any given point during execution.
     *     - `initialEnergyRequired`: The total starting energy we must provision.
     * - Step 3: Iterate through the sorted tasks. If `currentEnergy` is less than the 
     *   task's required `minimum`, calculate the shortfall, inject it into both 
     *   `initialEnergyRequired` and `currentEnergy`, and then deduct the `actual` cost.
     */
    int minimumEffort(vector<vector<int>>& tasks) {
        // Step 1: Sort tasks greedily based on descending variance deficit
        sort(tasks.begin(), tasks.end(), [](const vector<int>& a, const vector<int>& b) {
            return (a[1] - a[0]) > (b[1] - b[0]);
        });

        int initialEnergyRequired = 0;
        int currentEnergy = 0;

        // Step 2 & 3: Simulate execution and dynamically inject energy upon shortfalls
        for (const auto& task : tasks) {
            int actualCost = task[0];
            int minimumThreshold = task[1];

            // If current energy reservoir cannot clear the threshold, inject the difference
            if (currentEnergy < minimumThreshold) {
                int energyShortfall = minimumThreshold - currentEnergy;
                initialEnergyRequired += energyShortfall;
                currentEnergy += energyShortfall;
            }

            // Consume the actual energy required to complete the task
            currentEnergy -= actualCost;
        }

        return initialEnergyRequired;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total number of tasks. Sorting the 
 *                  tasks using the custom lambda function dictates the primary bottleneck. 
 *                  The subsequent simulation pass runs in linear O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sorting step rearranges elements completely 
 *                   in-place, requiring zero extra dynamic memory allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of tasks: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Task count must be a positive integer." << endl;
        return 1;
    }

    vector<vector<int>> tasks(static_cast<size_t>(n), vector<int>(2));
    cout << "\nEnter the energy attributes for each task (Format: actual energy, then minimum energy):\n";
    for (int i = 0; i < n; ++i) {
        cout << "Task [" << i << "]: ";
        cin >> tasks[static_cast<size_t>(i)][0] >> tasks[static_cast<size_t>(i)][1];

        // Constraint sanity check
        if (tasks[static_cast<size_t>(i)][0] > tasks[static_cast<size_t>(i)][1]) {
            cout << "Constraint Error: Actual energy consumed cannot exceed minimum starting energy." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting custom greedy sorting and simulation pipeline..." << endl;
    int optimalStartingEnergy = solver.minimumEffort(tasks);

    cout << "Minimum initial energy required to complete all tasks safely: " << optimalStartingEnergy << endl;

    return 0;
}