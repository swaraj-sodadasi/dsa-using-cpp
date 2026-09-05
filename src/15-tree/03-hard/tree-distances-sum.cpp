#include <iostream>
#include <vector>

using namespace std;

/*
 * CORE LOGIC (Rerooting Tree DP / Two-Pass Bottom-Up Top-Down Counts):
 * 1. Pass 1 (Bottom-Up DFS):
 *    - Root the tree arbitrarily at node 0.
 *    - Compute subtree sizes `count[u]` for all nodes u:
 *      `count[u] = 1 + sum(count[v])` for all children v of u.
 *    - Compute initial distance sum for root 0:
 *      `ans[0] = sum(ans[v] + count[v])` for all children v of 0.
 * 2. Pass 2 (Top-Down Rerooting DFS):
 *    - When moving root focus from parent `u` to child `v`:
 *      * Nodes in subtree `v` get 1 step closer (subtract `count[v]`).
 *      * Nodes outside subtree `v` (total `n - count[v]`) get 1 step farther (add `n - count[v]`).
 *    - State transition:
 *      `ans[v] = ans[u] - count[v] + (n - count[v])`
 * 3. Recursively propagate this state down to all nodes to compute answer for every root.
 * 
 * TIME COMPLEXITY: O(n)
 * - Two full DFS traversals visit each node and edge exactly twice.
 * 
 * SPACE COMPLEXITY: O(n)
 * - Adjacency list and DP vectors (`count`, `ans`) take O(n) space.
 */

class Solution {
private:
    vector<vector<int>> adj;
    vector<int> count;
    vector<int> ans;
    int totalNodes;

    void dfsBottomUp(int node, int parent) {
        count[node] = 1;
        ans[node] = 0;

        for (int child : adj[node]) {
            if (child != parent) {
                dfsBottomUp(child, node);
                count[node] += count[child];
                ans[node] += ans[child] + count[child];
            }
        }
    }

    void dfsTopDown(int node, int parent) {
        for (int child : adj[node]) {
            if (child != parent) {
                // Rerooting DP transition equation
                ans[child] = ans[node] - count[child] + (totalNodes - count[child]);
                dfsTopDown(child, node);
            }
        }
    }

public:
    vector<int> sumOfDistancesInTree(int n, const vector<vector<int>>& edges) {
        totalNodes = n;
        adj.assign(n, vector<int>());
        count.assign(n, 0);
        ans.assign(n, 0);

        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        // Pass 1: Bottom-up DP to compute count array and ans[0]
        dfsBottomUp(0, -1);

        // Pass 2: Top-down rerooting DP to compute ans for all other nodes
        dfsTopDown(0, -1);

        return ans;
    }
};

int main() {
    cout << "--- Sum of Distances in Tree (Rerooting Tree DP) ---" << endl;

    int n;
    cout << "Enter total number of nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    vector<vector<int>> edges(n - 1, vector<int>(2));
    cout << "Enter " << n - 1 << " space-separated edge pairs (u v):" << endl;
    for (int i = 0; i < n - 1; ++i) {
        cin >> edges[i][0] >> edges[i][1];
    }

    Solution solver;
    vector<int> result = solver.sumOfDistancesInTree(n, edges);

    cout << "Sum of distances for each node: [ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : " ");
    }
    cout << "]" << endl;

    return 0;
}