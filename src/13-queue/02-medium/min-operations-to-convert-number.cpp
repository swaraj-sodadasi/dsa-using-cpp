#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: STATE SPACE BRANCHING BREADTH-FIRST SEARCH
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum number of operations to convert `start` to `goal`.
     * 
     * 1. STATE-SPACE BFS MODEL:
     *    We model the conversion process as an unweighted directed state-space graph where 
     *    each integer represents a state, and operations (+, -, ^) with numbers in `nums` 
     *    represent directed transitions. BFS is optimal for finding the shortest path.
     * 
     * 2. BRANCHING DOMAIN CONSTRAINT:
     *    According to the constraints, we can only perform operations on `x` if:
     *      0 <= x <= 1000
     *    If the result of an operation falls outside this range, it can still be our target 
     *    `goal` (yielding immediate termination), but we cannot branch further from it.
     * 
     * 3. VISITED TRACKER (O(1) RANGE REGISTERS):
     *    Because valid branching states are strictly bounded between 0 and 1000, we can use 
     *    a fast boolean look-up array of size 1001 to keep track of visited states and 
     *    prevent infinite cycle evaluations.
     */
    int minimumOperations(const vector<int>& nums, int start, int goal) {
        if (start == goal) return 0;

        // Queue holds pairs of {current_value, operations_count}
        queue<pair<int, int>> bfsQueue;
        bfsQueue.push({start, 0});

        // Visited array for fast index lookups within the valid bounds [0, 1000]
        vector<bool> visited(1001, false);
        if (start >= 0 && start <= 1000) {
            visited[static_cast<size_t>(start)] = true;
        }

        while (!bfsQueue.empty()) {
            auto [currentVal, steps] = bfsQueue.front();
            bfsQueue.pop();

            // Branch out using every number in the nums array
            for (int num : nums) {
                // Generate all 3 transition states
                int nextStates[3] = {
                    currentVal + num,
                    currentVal - num,
                    currentVal ^ num
                };

                for (int nextVal : nextStates) {
                    if (nextVal == goal) {
                        return steps + 1;
                    }

                    // Only branch further if the state falls within [0, 1000] and is unvisited
                    if (nextVal >= 0 && nextVal <= 1000) {
                        if (!visited[static_cast<size_t>(nextVal)]) {
                            visited[static_cast<size_t>(nextVal)] = true;
                            bfsQueue.push({nextVal, steps + 1});
                        }
                    }
                }
            }
        }

        return -1; // Goal unreachable
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(V * N) - Where V is the range of valid states (1001) and N is the 
 *                  number of elements in the `nums` array. In the worst case, we push 
 *                  at most 1001 states into the queue and branch each N times.
 *
 * SPACE COMPLEXITY: O(V) Auxiliary - The visited state array is locked to a fixed size 
 *                   of 1001, and the queue stores at most V elements.
 */
int main() {
    cout << "=== State Space Branching BFS Converter ===\n";
    cout << "Enter the number of integers in the transition array (nums): ";
    int numCount;
    if (!(cin >> numCount) || numCount <= 0) return 1;

    vector<int> nums(static_cast<size_t>(numCount));
    cout << "Enter the elements of nums sequentially:\n";
    for (int i = 0; i < numCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the start value: ";
    int start;
    cin >> start;

    cout << "Enter the goal target: ";
    int goal;
    cin >> goal;

    Solution solver;
    cout << "\nAnalyzing transition branching states..." << endl;
    int operations = solver.minimumOperations(nums, start, goal);

    if (operations != -1) {
        cout << "Minimum operations required to reach goal: [" << operations << "]\n";
    } else {
        cout << "The goal state is unreachable under current bounds. Output: [-1]\n";
    }

    return 0;
}