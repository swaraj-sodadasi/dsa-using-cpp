#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Using a high value to represent infinity for unreachable node paths safely
const long long INF = 1e15;

/*
CORE LOGIC (CONTRIBUTION TO SUM TECHNIQUE VIA ALL-PAIRS SHORTEST PATHS):
- The problem asks us to compute a global sum of all-pairs shortest paths metrics or their variations 
  (e.g., adding up the shortest path values between all possible pairs of vertices `(u, v)` across the graph).
- Instead of computing paths in isolation, we use the Floyd-Warshall algorithm as our foundation to calculate 
  the complete all-pairs shortest path matrix `dist[u][v]`.
- **Contribution to Sum Technique**: 
  - A naive tabulation would re-traverse paths or accumulate states inefficiently. 
  - Instead, we evaluate how much each individual edge or intermediate route *contributes* to the overall graph connectivity. 
  - For problems involving the summation of paths, once `dist[u][v]` is finalized for all pairs, we iterate through 
    the matrix and add up all valid, finite path distances.
  - To prevent counting self-loops, path metrics where `u == v` are ignored, and if a pair `(u, v)` is completely 
    unreachable (`dist[u][v] == INF`), its contribution to the sum is exactly 0.
- **Floyd-Warshall Dynamic Relaxation**:
  We use three nested loops. In each phase, an intermediate vertex `k` is introduced. We check if routing from `u` to `v` 
  through `k` provides a shorter path than the currently recorded distance:
  $$ dist[u][v] = \min(dist[u][v], dist[u][k] + dist[k][v]) $$

TIME COMPLEXITY: O(V^3) - Where V is the number of vertices in the graph. The execution is bounded by the three nested loops.
SPACE COMPLEXITY: O(V^2) - The shortest path states are maintained in a 2D matrix structure of size V x V.
*/
long long computeAllPairsShortestPathSum(int n, const vector<vector<long long>>& adjacencyMatrix) {
    // Initialize the distance matrix with the input graph configuration
    vector<vector<long long>> dist(static_cast<size_t>(n), vector<long long>(static_cast<size_t>(n)));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[static_cast<size_t>(i)][static_cast<size_t>(j)] = adjacencyMatrix[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    // Core Floyd-Warshall dynamic relaxation sequence
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // If a path from i to k or from k to j is unreachable, we cannot relax through k
                if (dist[static_cast<size_t>(i)][static_cast<size_t>(k)] == INF || 
                    dist[static_cast<size_t>(k)][static_cast<size_t>(j)] == INF) {
                    continue;
                }
                
                // Relax the path distance
                dist[static_cast<size_t>(i)][static_cast<size_t>(j)] = min(
                    dist[static_cast<size_t>(i)][static_cast<size_t>(j)], 
                    dist[static_cast<size_t>(i)][static_cast<size_t>(k)] + dist[static_cast<size_t>(k)][static_cast<size_t>(j)]
                );
            }
        }
    }

    long long totalPathSum = 0;

    // Apply the Contribution to Sum Technique: Accumulate all finite distinct paths
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Skip self-distances and completely disconnected components
            if (i != j && dist[static_cast<size_t>(i)][static_cast<size_t>(j)] != INF) {
                totalPathSum += dist[static_cast<size_t>(i)][static_cast<size_t>(j)];
            }
        }
    }

    return totalPathSum;
}

int main() {
    int n, numEdges;
    cout << "Enter the number of vertices (V): ";
    cin >> n;
    cout << "Enter the number of edges (E): ";
    cin >> numEdges;

    if (n <= 0 || numEdges < 0) {
        cout << "Invalid graph configuration dimensions." << endl;
        return 1;
    }

    // Initialize the baseline adjacency grid configuration
    vector<vector<long long>> adjacencyMatrix(static_cast<size_t>(n), vector<long long>(static_cast<size_t>(n), INF));
    
    // Distances from any node to itself is always zero
    for (int i = 0; i < n; ++i) {
        adjacencyMatrix[static_cast<size_t>(i)][static_cast<size_t>(i)] = 0;
    }

    cout << "Enter the edges in format (u v weight) where 0 <= u, v < V:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        long long weight;
        cin >> u >> v >> weight;
        
        if (u >= 0 && u < n && v >= 0 && v < n) {
            // Track the smallest weight in case duplicate multi-edges are provided
            adjacencyMatrix[static_cast<size_t>(u)][static_cast<size_t>(v)] = min(
                adjacencyMatrix[static_cast<size_t>(u)][static_cast<size_t>(v)], 
                weight
            );
        }
    }

    long long totalSum = computeAllPairsShortestPathSum(n, adjacencyMatrix);
    cout << "\nThe total accumulated sum of all-pairs shortest paths is: " << totalSum << endl;

    return 0;
}