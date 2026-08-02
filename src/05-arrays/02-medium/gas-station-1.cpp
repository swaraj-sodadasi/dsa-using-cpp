#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * GREEDY ACCUMULATION / SINGLE-PASS DEFICIT TRAVERSAL ENGINE
     * ----------------------------------------------------------
     * The objective is to find the starting gas station index from which a car can 
     * complete a full circular journey clockwise around N gas stations. If it is 
     * impossible, return -1.
     *
     * Mathematical & Algorithmic Mechanics:
     * - Let `gas[i]` be the fuel available at station i, and `cost[i]` be the fuel needed to reach station i+1.
     * - Property 1 (Global Feasibility): If the total gas across all stations is less than the total cost 
     *   (`sum(gas) < sum(cost)`), a valid journey is mathematically impossible. We return -1 immediately.
     * - Property 2 (Local Sub-path Failure): If a car starts at station A and runs out of fuel at station B, 
     *   it cannot complete the circle starting from *any* station between A and B. This is because it would arrive 
     *   at any intermediate station with less or equal fuel than it had by driving there from A.
     *
     * Dynamic Pivot Strategy:
     * - We keep a running total of our fuel balance in `currentSurplus`. 
     * - If `currentSurplus` drops below zero at station `i`, it means the journey fails. We greedily set the next 
     *   station `i + 1` as our new candidate starting index and reset `currentSurplus` to 0.
     * - Thanks to Property 1, if the global sum condition is satisfied, the first candidate starting index that 
     *   completes the remaining linear pass without dropping below zero is guaranteed to be the correct answer.
     */
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalGas = 0;
        int totalCost = 0;
        
        // Sum up total gas and cost to verify global feasibility
        for (size_t i = 0; i < gas.size(); ++i) {
            totalGas += gas[i];
            totalCost += cost[i];
        }

        // Global Check: If total fuel is less than total consumption, no solution exists
        if (totalGas < totalCost) {
            return -1;
        }

        int startStationCandidate = 0;
        int currentSurplus = 0;
        size_t n = gas.size();

        // Perform a single-pass greedy traversal
        for (size_t i = 0; i < n; ++i) {
            currentSurplus += gas[i] - cost[i];

            // If the tank drops below zero, the current starting point fails
            if (currentSurplus < 0) {
                // Greedily shift the starting candidate past the failed segment region
                startStationCandidate = static_cast<int>(i + 1);
                currentSurplus = 0; // Reset active accumulator
            }
        }

        return startStationCandidate;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of gas stations. 
 *                  The logic performs a single unified forward pass across the array dataset, 
 *                  updating running tracking scalars in constant O(1) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization engine tracks only a few scalar index 
 *                   and value variables, using zero extra memory layouts on the runtime heap.
 */
int main() {
    int n;
    cout << "Enter the total number of gas stations: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The station pool count must be greater than 0." << endl;
        return 1;
    }

    vector<int> gas(static_cast<size_t>(n));
    cout << "Enter the gas amount available at each station separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> gas[static_cast<size_t>(i)];
    }

    vector<int> cost(static_cast<size_t>(n));
    cout << "Enter the cost amount required to reach the next station separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> cost[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting single-pass deficit tracking sweep..." << endl;
    int startingIndex = solver.canCompleteCircuit(gas, cost);

    if (startingIndex != -1) {
        cout << "Optimal starting station index location: " << startingIndex << endl;
    } else {
        cout << "Result: -1 (It is impossible to travel around the circuit once)" << endl;
    }

    return 0;
}