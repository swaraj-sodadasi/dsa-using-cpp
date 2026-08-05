#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * FACTORY LOCATION COORDINATE SORTING WITH MATRIX DP ENGINE
     * --------------------------------------------------------
     * The objective is to pair an array of robot positions with an array of factory locations 
     * (where each factory has a specific repair capacity) such that the sum of the absolute 
     * distances traveled by all robots is minimized.
     *
     * Algorithmic Strategy:
     * - Sorting Constraint Validation: To minimize total crossing paths, if we sort both the 
     *   robot positions and the factory locations by their linear coordinates, the optimal matching 
     *   will never require crossing paths. This allows us to use dynamic programming.
     * - Multi-Instance Flattening: To simplify capacity constraints, we flatten the factories array 
     *   so that if a factory at position `P` has capacity `C`, we insert `C` separate factory instances 
     *   at position `P` into a flat sequence (`flatFactories`).
     * - State Space Definition: Let `dp[i][j]` represent the minimum distance required to fix the first 
     *   `i` robots using the first `j` flattened factory slots.
     * 
     * Dynamic Programming Transitions:
     * - Option A (Skip current factory slot): `dp[i][j] = dp[i][j-1]`
     * - Option B (Match current robot to current factory slot): `dp[i][j] = dp[i-1][j-1] + abs(robots[i-1] - flatFactories[j-1])`
     * - Base Cases: `dp[0][j] = 0` (zero robots cost nothing to repair), and `dp[i][0] = LLONG_MAX / 2` 
     *   (robots remaining but no factory slots available is an impossible state).
     */
    long long minimumTotalDistance(vector<int>& robot, vector<vector<int>>& factory) {
        // Step 1: Sort the primary robot coordinate elements
        sort(robot.begin(), robot.end());
        
        // Sort the factory list based on position coordinates
        sort(factory.begin(), factory.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0];
        });

        // Step 2: Flatten factory slots based on their individual capacity parameters
        vector<int> flatFactories;
        for (const auto& f : factory) {
            int position = f[0];
            int capacity = f[1];
            for (int c = 0; c < capacity; ++c) {
                flatFactories.push_back(position);
            }
        }

        size_t numRobots = robot.size();
        size_t numSlots = flatFactories.size();
        
        // Overflow safety guard value allocation
        long long INF = LLONG_MAX / 2;

        // Step 3: Initialize the Matrix DP table structure
        // Rows represent robots matched, columns represent flattened factory capacities explored
        vector<vector<long long>> dp(numRobots + 1, vector<long long>(numSlots + 1, INF));

        // Base case: Zero robots matched costs exactly 0 distance
        for (size_t j = 0; j <= numSlots; ++j) {
            dp[0][j] = 0;
        }

        // Step 4: Evaluate optimization matrix transitions
        for (size_t i = 1; i <= numRobots; ++i) {
            for (size_t j = 1; j <= numSlots; ++j) {
                // Scenario A: Do not allocate the current factory slot to the current robot
                dp[i][j] = dp[i][j - 1];

                // Scenario B: Match the current robot to the current factory slot if valid
                if (dp[i - 1][j - 1] != INF) {
                    long long currentDistance = abs(robot[i - 1] - flatFactories[j - 1]);
                    dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + currentDistance);
                }
            }
        }

        return dp[numRobots][numSlots];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * F_tot) - Where R represents the total count of robots and F_tot represents 
 *                  the sum of all factory structural capacities. The double nested loops iterate over 
 *                  the dimensions of the matrix DP space, performing O(1) mathematical lookups.
 *
 * SPACE COMPLEXITY: O(R * F_tot) Auxiliary - Required to host the state space values within the 2D matrix 
 *                   DP table alongside the flattened coordinate vector.
 */
int main() {
    int rCount;
    cout << "Enter the total number of robots present: ";
    if (!(cin >> rCount) || rCount <= 0) return 1;

    vector<int> robot(static_cast<size_t>(rCount));
    cout << "Enter the space-separated position coordinates for each robot:\n";
    for (int i = 0; i < rCount; ++i) {
        cin >> robot[static_cast<size_t>(i)];
    }

    int fCount;
    cout << "Enter the total number of physical factory locations: ";
    if (!(cin >> fCount) || fCount <= 0) return 1;

    vector<vector<int>> factory(static_cast<size_t>(fCount), vector<int>(2));
    cout << "\nEnter attributes for each factory (Format: position, then processing capacity):\n";
    int absoluteTotalCapacity = 0;
    for (int i = 0; i < fCount; ++i) {
        cout << "Factory [" << i << "]: ";
        cin >> factory[static_cast<size_t>(i)][0] >> factory[static_cast<size_t>(i)][1];
        absoluteTotalCapacity += factory[static_cast<size_t>(i)][1];
    }

    // Constraint validation sweep
    if (absoluteTotalCapacity < rCount) {
        cout << "Constraint Error: Total processing capacity cannot accommodate the total number of robots." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting factory coordinate flattening and dynamic matching routines..." << endl;
    long long minimumDistanceAccumulated = solver.minimumTotalDistance(robot, factory);

    cout << "Minimum absolute travel distance required to service all robots: " << minimumDistanceAccumulated << endl;

    return 0;
}