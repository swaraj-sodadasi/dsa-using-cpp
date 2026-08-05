#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * CYCLE TRAVERSAL COST EVALUATION ENGINE (MATRIX MATRIX / FLATTENED PERMUTATION)
     * -----------------------------------------------------------------------------
     * The objective is to calculate the minimum cost required to sort a 2D matrix 
     * (processed as a flattened array) into strict ascending order.
     * 
     * Cost Function Definition:
     * - The cost of swapping any two elements X and Y is defined as (X + Y).
     *
     * Algorithmic Strategy (Graph Permutation Cycle Analysis):
     * - Any unsorted sequence forms a directed graph mapping each current element value 
     *   to its target sorted position matrix index.
     * - This graph naturally decomposes into independent, disjoint permutation cycles.
     * - To resolve a single cycle of size `K`, we must execute exactly `K - 1` structural swaps.
     *
     * Optimal Choice Dual-Pathway Verification Protocol:
     * - For each independent cycle, there are two competing strategies to minimize swap values:
     * 
     *   Pathway 1: Local Minimum Exploitation
     *     - Use the smallest value existing within the active cycle (`localMin`) to swap 
     *       with every other element in the cycle sequentially.
     *     - Formula: Cost1 = Sum(all elements in cycle except localMin) + localMin * (K - 1)
     * 
     *   Pathway 2: Global Minimum Borrowing & Execution
     *     - Swap the absolute smallest element of the entire global array (`globalMin`) into the cycle.
     *     - Perform the resolution using `globalMin`, then swap the original `localMin` back out.
     *     - Formula: Cost2 = Sum(all elements in cycle except localMin) + globalMin * (K - 1) + 2 * (localMin + globalMin)
     * 
     * - For each isolated component, we take the minimum of Cost1 and Cost2 and accumulate it into our total cost.
     */
    long long minCostToSortMatrix(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;

        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());
        int totalElements = rows * cols;

        // Flatten the 2D matrix structure into a single sequence vector
        vector<int> nums;
        nums.reserve(static_cast<size_t>(totalElements));
        int globalMin = INT_MAX;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int val = matrix[static_cast<size_t>(r)][static_cast<size_t>(c)];
                nums.push_back(val);
                globalMin = min(globalMin, val); // Track the global minimal anchor element
            }
        }

        // Pair up values with their current flattened location coordinates
        vector<pair<int, int>> sortedPositions(static_cast<size_t>(totalElements));
        for (int i = 0; i < totalElements; ++i) {
            sortedPositions[static_cast<size_t>(i)] = {nums[static_cast<size_t>(i)], i};
        }

        // Sort the tracking pairs by value to capture the target destination index matrix
        sort(sortedPositions.begin(), sortedPositions.end());

        // Track values to their final target index coordinates
        vector<int> targetIndexMap(static_cast<size_t>(totalElements));
        for (int i = 0; i < totalElements; ++i) {
            targetIndexMap[static_cast<size_t>(sortedPositions[static_cast<size_t>(i)].second)] = i;
        }

        vector<bool> visited(static_cast<size_t>(totalElements), false); //
        long long totalMinimumCost = 0;

        // Traverse all elements to parse disjoint structural permutation loops
        for (int i = 0; i < totalElements; ++i) {
            // Skip processing if the cell is visited or already resting at its target sorted location
            if (visited[static_cast<size_t>(i)] || targetIndexMap[static_cast<size_t>(i)] == i) {
                continue;
            }

            long long cycleSum = 0;
            int localMin = INT_MAX;
            int cycleSize = 0;
            int currentIdx = i;

            // Follow the node permutation mapping links until the cycle loops back onto itself
            while (!visited[static_cast<size_t>(currentIdx)]) {
                visited[static_cast<size_t>(currentIdx)] = true;
                int val = nums[static_cast<size_t>(currentIdx)];
                
                cycleSum += val;
                localMin = min(localMin, val); //
                cycleSize++;

                currentIdx = targetIndexMap[static_cast<size_t>(currentIdx)]; //
            }

            // Exclude local minimum from standard sum accumulation to comply with pathway algebraic rules
            long long sumWithoutLocalMin = cycleSum - localMin;

            // Evaluate Pathway 1: Solve relying purely on internal local minimum elements
            long long costLocalMinOnly = sumWithoutLocalMin + (static_cast<long long>(localMin) * (cycleSize - 1)); //

            // Evaluate Pathway 2: Solve by temporarily borrowing global minimum anchor element
            long long costWithGlobalMin = sumWithoutLocalMin + (static_cast<long long>(globalMin) * (cycleSize - 1)) 
                                         + (2 * (static_cast<long long>(localMin) + globalMin)); //

            // Greedily commit to whichever pathway produces the lowest evaluation metric
            totalMinimumCost += min(costLocalMinOnly, costWithGlobalMin); //
        }

        return totalMinimumCost;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M * log(N * M)) - Where N represents the total row count and M represents 
 *                  the total column count of the input matrix. Flattening and sorting the array elements 
 *                  to build the ideal index target matrix acts as the key primary algorithmic cost. 
 *                  The subsequent cycle tracing pass evaluates each cell index linearly in O(N * M) time.
 *
 * SPACE COMPLEXITY: O(N * M) Auxiliary - The graph tracking layout framework requires storage maps, 
 *                   flattening lists, and a boolean validation vector scaling linearly with matrix size.
 */
int main() {
    int rows, cols;
    cout << "Enter the row matrix size capacity (M): ";
    if (!(cin >> rows) || rows <= 0) return 1;

    cout << "Enter the column matrix size capacity (N): ";
    if (!(cin >> cols) || cols <= 0) return 1;

    vector<vector<int>> matrix(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "\nEnter matrix cell item values row by row:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> matrix[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    Solution solver;
    cout << "\nExecuting graph cycle decomposition matrix cost evaluation metrics..." << endl;
    long long minimumSortingCost = solver.minCostToSortMatrix(matrix);

    cout << "Minimum computed swap cost required to sort the matrix structure: " << minimumSortingCost << endl;

    return 0;
}