#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * CYCLE DECOMPOSITION ANALYSIS ENGINE
     * -----------------------------------
     * The objective is to calculate the minimum number of swaps required to sort an 
     * array of elements. This algorithm treats the sorting process as a directed graph 
     * permuted state mapping, leveraging the properties of Cycle Decomposition.
     *
     * Mathematical & Algorithmic Strategy:
     * - Any unsorted array can be modeled as a directed graph where an edge exists from the 
     *   current index of an element to its correct sorted index.
     * - This graph decomposes into a set of disjoint, isolated permutation cycles.
     *   - A cycle of length `L` elements requires exactly `L - 1` swaps to resolve all elements 
     *     back into their correct positions.
     * - Therefore, the total minimum swaps needed across the entire array is equal to the 
     *   sum of `(L_i - 1)` for every cycle `i`, which simplifies to:
     *   Total Swaps = Sum of all cycle lengths - Number of cycles
     *
     * Execution Protocol:
     * - Step 1: Pair each element of the array with its original index position, then sort this 
     *   pair matrix by element value. This maps out where each element needs to go.
     * - Step 2: Initialize a tracking array (`visited`) to mark nodes that have been processed.
     * - Step 3: Iterate through the array to discover cycles. For each unvisited element, trace its 
     *   path from pointer to pointer until the path returns to the starting node, counting the cycle length `L`.
     * - Step 4: Add `L - 1` to the total swap accumulator and mark all elements in that cycle as visited.
     */
    int minSwaps(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        
        // Step 1: Bind elements to their initial indexes and sort to find target configurations
        vector<pair<int, int>> elementPositions(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) {
            elementPositions[static_cast<size_t>(i)] = {nums[static_cast<size_t>(i)], i};
        }

        sort(elementPositions.begin(), elementPositions.end());

        // Step 2: Initialize node trackers for tracking cycle paths
        vector<bool> visited(static_cast<size_t>(n), false);
        int totalSwapsRequired = 0;

        // Step 3 & 4: Trace permutation graph cycles
        for (int i = 0; i < n; ++i) {
            // Skip the element if it is already visited or already in its correct sorted position
            if (visited[static_cast<size_t>(i)] || elementPositions[static_cast<size_t>(i)].second == i) {
                continue;
            }

            int cycleSize = 0;
            int currentIdx = i;

            // Traverse the cycle path until it loops back onto itself
            while (!visited[static_cast<size_t>(currentIdx)]) {
                visited[static_cast<size_t>(currentIdx)] = true;
                // Move to the next index in the cycle given by the element's original position key
                currentIdx = elementPositions[static_cast<size_t>(currentIdx)].second;
                cycleSize++;
            }

            // Accumulate required swaps for the extracted cycle block
            if (cycleSize > 1) {
                totalSwapsRequired += (cycleSize - 1);
            }
        }

        return totalSwapsRequired;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements inside the array. 
 *                  Sorting the indexed element collection dominates the execution timeframe. 
 *                  The subsequent cycle tracing pass runs in linear O(N) time since each 
 *                  node is visited at most twice.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization strategy instantiates an explicit 
 *                   index layout mapping copy array alongside a boolean vector tracking 
 *                   visited graph nodes.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive integer value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting cycle decomposition permutation analysis pass..." << endl;
    int swapsCount = solver.minSwaps(nums);

    cout << "Minimum structural element swaps required to fully sort the array: " << swapsCount << endl;

    return 0;
}