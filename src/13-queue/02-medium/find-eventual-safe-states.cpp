#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: KAHN'S TOPOLOGICAL SORT ALGORITHM VIA IN-DEGREE QUEUES
     * -------------------------------------------------------------------------------------
     * A node is "safe" if all paths starting from that node lead to a terminal node. 
     * Conversely, any node that is part of a cycle, or can reach a cycle, is unsafe.
     * 
     * 1. THE INVERSE GRAPH TRANSITION:
     *    Instead of tracking outgoing paths (which makes cycle detection difficult), we 
     *    reverse all edges of the graph: `u -> v` becomes `v -> u`.
     *    - Original terminal nodes (out-degree = 0) now become source nodes (in-degree = 0) 
     *      in the reversed graph.
     * 
     * 2. KAHN'S TOPOLOGICAL SWEEP:
     *    - We calculate the in-degrees of all nodes in the reversed graph (which matches 
     *      the original out-degrees of the nodes).
     *    - We enqueue all nodes with an in-degree of 0 (the terminal nodes).
     *    - We pop nodes from the queue one-by-one. These are verified safe states. For 
     *      each popped node, we traverse its neighbors in the reversed graph and decrement 
     *      their in-degrees. If a neighbor's in-degree drops to 0, it means all its paths 
     *      exclusively lead to safe nodes. We then enqueue it.
     * 
     * 3. BOUNDARY PROTECTION:
     *    - CRITICAL FIX: When iterating through the neighbors of node `u`, we must ensure 
     *      that each target node `v` is a valid index within the range [0, n - 1]. If a user 
     *      inputs an out-of-bounds neighbor index, ignoring or filtering it prevents 
     *      out-of-bounds vector assertions in `reverseGraph[v]` during dependency reversing.
     * 
     * 4. TERMINAL EVALUATION:
     *    Nodes visited during the BFS are safe. We collect, sort, and return them.
     */
    vector<int> eventualSafeNodes(const vector<vector<int>>& graph) {
        int n = static_cast<int>(graph.size());
        vector<vector<int>> reverseGraph(static_cast<size_t>(n));
        vector<int> inDegree(static_cast<size_t>(n), 0);

        // Step 1: Build the reversed graph and populate in-degree array
        for (int u = 0; u < n; ++u) {
            int validOutDegree = 0;
            for (int v : graph[static_cast<size_t>(u)]) {
                // Safeguard against out-of-bound target nodes (e.g., neighbor node index does not exist)
                if (v >= 0 && v < n) {
                    // Original edge: u -> v | Reversed edge: v -> u
                    reverseGraph[static_cast<size_t>(v)].push_back(u);
                    validOutDegree++;
                }
            }
            // In-degree in reversed graph equals original valid out-degree
            inDegree[static_cast<size_t>(u)] = validOutDegree;
        }

        queue<int> safeQueue;
        // Step 2: Enqueue all terminal nodes (in-degree of 0 in reverse graph)
        for (int i = 0; i < n; ++i) {
            if (inDegree[static_cast<size_t>(i)] == 0) {
                safeQueue.push(i);
            }
        }

        vector<bool> isSafe(static_cast<size_t>(n), false);

        // Step 3: Standard Kahn's BFS queue iteration
        while (!safeQueue.empty()) {
            int current = safeQueue.front();
            safeQueue.pop();
            isSafe[static_cast<size_t>(current)] = true;

            for (int neighbor : reverseGraph[static_cast<size_t>(current)]) {
                inDegree[static_cast<size_t>(neighbor)]--;
                if (inDegree[static_cast<size_t>(neighbor)] == 0) {
                    safeQueue.push(neighbor);
                }
            }
        }

        // Step 4: Collect all safe nodes in sorted order
        vector<int> safeNodes;
        for (int i = 0; i < n; ++i) {
            if (isSafe[static_cast<size_t>(i)]) {
                safeNodes.push_back(i);
            }
        }

        return safeNodes;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(V + E) - Where V represents the number of nodes (vertices) 
 *                  and E represents the total number of directed edges in the graph.
 *
 * SPACE COMPLEXITY: O(V + E) Auxiliary - To store the reversed graph adjacency list, 
 *                   in-degree lookup array, and BFS tracking structures.
 */
int main() {
    cout << "=== Eventual Safe States Kahn's Topological Sweep ===\n";
    cout << "Enter the total number of nodes (V): ";
    int numNodes;
    if (!(cin >> numNodes) || numNodes <= 0) return 1;

    vector<vector<int>> graph(static_cast<size_t>(numNodes));
    cout << "Enter the adjacency list for each node (enter count first, then neighbor list):\n";
    for (int i = 0; i < numNodes; ++i) {
        cout << "Node [" << i << "] neighbors count: ";
        int count;
        cin >> count;
        if (count > 0) {
            cout << "  Enter " << count << " neighbors: ";
            for (int j = 0; j < count; ++j) {
                int neighbor;
                cin >> neighbor;
                graph[static_cast<size_t>(i)].push_back(neighbor);
            }
        }
    }

    Solution solver;
    cout << "\nReversing dependency arcs and resolving terminal leaf nodes..." << endl;
    vector<int> safeStates = solver.eventualSafeNodes(graph);

    cout << "Identified Eventual Safe States Matrix:\n[";
    for (size_t i = 0; i < safeStates.size(); ++i) {
        cout << safeStates[i] << (i + 1 < safeStates.size() ? ", " : "");
    }
    cout << "]\n";

    return 0;
}