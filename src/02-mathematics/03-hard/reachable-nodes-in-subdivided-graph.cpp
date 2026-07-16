#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Constant value to represent infinite distance paths safely
const int INF = 1e9;

/*
CORE LOGIC (DIJKSTRA EXTENDED WITH EDGE REMAINDER MATH):
- The problem asks us to determine the total number of reachable nodes (both original backbone nodes and intermediate subdivision nodes) 
  from node 0 using a maximum step count constraint `maxMoves`.
- **Step 1: Shortest Path Detection (Dijkstra's Algorithm)**:
  We treat the base nodes as a traditional weighted graph. Moving across an edge containing `cnt` sub-nodes requires exactly `cnt + 1` steps.
  We run Dijkstra's algorithm from source node 0 using a min-priority queue to find the minimum distance (`dist[i]`) to every base node.
- **Step 2: Counting Reachable Base Nodes**:
  Any base node `i` whose minimum distance `dist[i]` is less than or equal to `maxMoves` is fully reachable, adding 1 to our running total.
- **Step 3: Calculating Subdivision Edge Remainders**:
  For each original undirected edge connecting node `u` and node `v` embedded with `cnt` sub-nodes:
  - The number of sub-nodes reachable starting from node `u`'s side is `max(0, maxMoves - dist[u])`.
  - The number of sub-nodes reachable starting from node `v`'s side is `max(0, maxMoves - dist[v])`.
  - The total sub-nodes claimed along this edge is the sum of the reachability limits from both endpoints. However, because both directions 
    can overlap or over-consume the edge, the final addition is strictly capped at the physical capacity of the edge: `min(cnt, reach_u + reach_v)`.

TIME COMPLEXITY: O(E * log V) - Where E is the number of edges and V is the number of vertices. The performance bottleneck is 
                 the priority-queue-driven edge relaxation sequence.
SPACE COMPLEXITY: O(V + E) - Auxiliary memory is utilized entirely by the graph adjacency structures and min-distance arrays.
*/
int reachableNodes(const vector<vector<int>>& edges, int maxMoves, int n) {
    // Safety check: ensure graph can contain at least the source node 0
    if (n <= 0) return 0;

    // Build adjacency list: adj[u] stores pairs of {neighbor, edge_weight}
    vector<vector<pair<int, int>>> adj(static_cast<size_t>(n));
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        int cnt = edge[2];
        
        // Robust boundary validation: ignore or handle invalid edge declarations out-of-bounds
        if (u < 0 || u >= n || v < 0 || v >= n) {
            continue; 
        }
        
        adj[static_cast<size_t>(u)].push_back({v, cnt + 1});
        adj[static_cast<size_t>(v)].push_back({u, cnt + 1});
    }

    // Min-priority queue storing pairs of {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(static_cast<size_t>(n), INF);

    // Initialize the source node 0 execution path
    dist[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // If a shorter path to u has already been processed, discard this stale record
        if (d > dist[static_cast<size_t>(u)]) continue;

        for (const auto& [v, weight] : adj[static_cast<size_t>(u)]) {
            if (dist[static_cast<size_t>(u)] + weight < dist[static_cast<size_t>(v)]) {
                dist[static_cast<size_t>(v)] = dist[static_cast<size_t>(u)] + weight;
                pq.push({dist[static_cast<size_t>(v)], v});
            }
        }
    }

    int totalReachable = 0;

    // Step 2: Accumulate all base graph nodes within reach of maxMoves
    for (int i = 0; i < n; ++i) {
        if (dist[static_cast<size_t>(i)] <= maxMoves) {
            totalReachable++;
        }
    }

    // Step 3: Compute sub-node coverages across individual edges with remainder math
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        int cnt = edge[2];

        // Skip calculations for edges outside validation parameters
        if (u < 0 || u >= n || v < 0 || v >= n) continue;

        // Evaluate remaining step ranges extending out from endpoints
        int reach_u = (dist[static_cast<size_t>(u)] <= maxMoves) ? (maxMoves - dist[static_cast<size_t>(u)]) : 0;
        int reach_v = (dist[static_cast<size_t>(v)] <= maxMoves) ? (maxMoves - dist[static_cast<size_t>(v)]) : 0;

        // Cap intermediate edge node metrics to the maximum subdivision allocation limit
        totalReachable += min(cnt, reach_u + reach_v);
    }

    return totalReachable;
}

int main() {
    int n, numEdges, maxMoves;
    cout << "Enter number of original graph vertices (n): ";
    cin >> n;
    cout << "Enter total number of edges: ";
    cin >> numEdges;
    cout << "Enter maximum moves limit: ";
    cin >> maxMoves;

    if (n <= 0 || numEdges < 0 || maxMoves < 0) {
        cout << "Invalid parameter initializations detected." << endl;
        return 1;
    }

    vector<vector<int>> edges;
    cout << "Enter edges format as (u v cnt) where 'cnt' is the number of sub-nodes:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v, cnt;
        cin >> u >> v >> cnt;
        
        // Prevent manual typing failures from pushing invalid indexes to downstream vectors
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cout << "Warning: Edge " << u << " -> " << v << " skipped because node labels exceed vertex count n=" << n << "\n";
            continue;
        }
        edges.push_back({u, v, cnt});
    }

    int result = reachableNodes(edges, maxMoves, n);
    cout << "\nTotal reachable nodes (original + sub-nodes) from node 0: " << result << endl;

    return 0;
}