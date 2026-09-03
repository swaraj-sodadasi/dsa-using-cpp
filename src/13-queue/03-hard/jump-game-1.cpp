#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-VALUE INDEX SUBLIST HASH MAP NODE AGGREGATION BFS
     * -------------------------------------------------------------------------------------
     * The objective is to find the minimum number of steps to reach the last index of the array.
     * From an index `i`, we can jump to:
     *   1. `i + 1` (where `i + 1 < arr.length`)
     *   2. `i - 1` (where `i - 1 >= 0`)
     *   3. `j` (where `arr[i] == arr[j]` and `i != j`)
     * 
     * 1. HASH MAP GRAPH AGGREGATION:
     *    To handle jumps of type 3 in constant time, we build an adjacency list representation 
     *    using a hash map: `std::unordered_map<int, std::vector<int>>`. This maps each unique 
     *    value in the array to a sublist of all index positions containing that value.
     * 
     * 2. SHORTEST-PATH BFS EXPANSION:
     *    Standard BFS naturally processes nodes in increasing order of distance. The first time 
     *    we reach the final index `n - 1`, we are guaranteed to have found the shortest path.
     * 
     * 3. CRITICAL REDUNDANCY PRUNING:
     *    Once we expand jumps of type 3 for a given value, we clear its entry inside our 
     *    hash map (`map.erase(value)` or `.clear()`). If we visit another node with the 
     *    same value later, we don't need to check these indices again. This prevents 
     *    O(N^2) worst-case performance on arrays with many duplicate elements (e.g., [7,7,7...]).
     */
    int minJumps(const vector<int>& arr) {
        int n = static_cast<int>(arr.size());
        if (n <= 1) return 0;

        // Step 1: Build the value-to-index list mapping representation
        unordered_map<int, vector<int>> valueIndices;
        for (int i = 0; i < n; ++i) {
            valueIndices[arr[static_cast<size_t>(i)]].push_back(i);
        }

        queue<int> bfsQueue;
        vector<bool> visited(static_cast<size_t>(n), false);

        bfsQueue.push(0);
        visited[0] = true;

        int steps = 0;

        // Step 2: Standard BFS traversal
        while (!bfsQueue.empty()) {
            int levelSize = static_cast<int>(bfsQueue.size());

            for (int i = 0; i < levelSize; ++i) {
                int curr = bfsQueue.front();
                bfsQueue.pop();

                // Target index reached
                if (curr == n - 1) {
                    return steps;
                }

                int val = arr[static_cast<size_t>(curr)];

                // Direction option 1: Step Right (curr + 1)
                if (curr + 1 < n && !visited[static_cast<size_t>(curr + 1)]) {
                    visited[static_cast<size_t>(curr + 1)] = true;
                    bfsQueue.push(curr + 1);
                }

                // Direction option 2: Step Left (curr - 1)
                if (curr - 1 >= 0 && !visited[static_cast<size_t>(curr - 1)]) {
                    visited[static_cast<size_t>(curr - 1)] = true;
                    bfsQueue.push(curr - 1);
                }

                // Direction option 3: Jump to other indices with the same value
                if (valueIndices.find(val) != valueIndices.end()) {
                    for (int sameValueIdx : valueIndices[val]) {
                        if (sameValueIdx != curr && !visited[static_cast<size_t>(sameValueIdx)]) {
                            visited[static_cast<size_t>(sameValueIdx)] = true;
                            bfsQueue.push(sameValueIdx);
                        }
                    }
                    // CRITICAL STEP: Clear the list of indices for this value to avoid redundant O(N) scans
                    valueIndices.erase(val);
                }
            }
            steps++;
        }

        return -1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of elements in the array. 
 *                  Every node and edge in the graph is visited at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - To hold the indices hash map, visited lookup 
 *                   array, and queue allocations.
 */
int main() {
    cout << "=== Jump Game IV BFS Path Finder ===\n";
    cout << "Enter the number of elements in the array: ";
    int numCount;
    if (!(cin >> numCount) || numCount <= 0) return 1;

    vector<int> arr(static_cast<size_t>(numCount));
    cout << "Enter the elements of the array sequentially:\n";
    for (int i = 0; i < numCount; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nScanning path layers and executing multi-value index jumps..." << endl;
    int jumps = solver.minJumps(arr);

    cout << "Minimum jump operations required to reach the end: [" << jumps << "]\n";
    return 0;
}