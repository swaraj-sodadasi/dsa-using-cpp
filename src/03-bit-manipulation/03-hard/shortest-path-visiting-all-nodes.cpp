#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/*
CORE LOGIC (BITMASK BFS STATE SPACE EXPANSION):
- The problem asks us to find the shortest path that visits every node in an undirected graph. 
  We can start and end at any node, and we can revisit nodes and edges multiple times.
- **State Representation via Bitmasks**:
  - Since the number of nodes $N$ is small (typically $N \le 12$), we can represent the set of visited 
    nodes using an $N$-bit integer `mask`.
  - A bit value of 1 at the $i$-th position indicates that node $i$ has been visited.
  - The target state is reached when all nodes have been visited, meaning `mask == (1 << N) - 1`.
- **Breadth-First Search (BFS) Over State Space**:
  - A simple BFS tracking only the current node would fail because we are allowed to revisit nodes.
  - To properly detect cycles and avoid infinite loops, a single state must combine both the current physical position 
    and the collection of nodes visited so far: `(currentNode, visitedMask)`.
  - We initialize the BFS queue by pushing the starting configurations for *every* node: `(i, 1 << i)` with a distance of 0.
  - We use a 2D boolean array `visited[N][1 << N]` to ensure we never re-explore the exact same combination of node and visited mask.
- **State Transition Strategy**:
  - At each step, we pop the front state from the queue. If `mask` equals the target mask, we immediately return the current distance.
  - Otherwise, we look at all neighbors of `currentNode`. For each neighbor, the next state mask becomes `nextMask = mask | (1 << neighbor)`.
  - If `visited[neighbor][nextMask]` is false, we mark it true and push `(neighbor, nextMask)` into the queue with `distance + 1`.

TIME COMPLEXITY: O(N * 2^N) - There are at most $N \times 2^N$ unique states in the state space graph. A traditional BFS processes each state and its adjacent edge list exactly once.
SPACE COMPLEXITY: O(N * 2^N) auxiliary - Storage allocated to maintain the tracking framework inside the 2D boolean lookup table and the queue.
*/

struct State {
    int node;
    int mask;
    int dist;
};

int shortestPathLength(const vector<vector<int>>& graph) {
    size_t n = graph.size();
    if (n <= 1) return 0;

    int targetMask = (1 << n) - 1;
    queue<State> q;
    
    // visited[currentNode][visitedMask]
    vector<vector<bool>> visited(n, vector<bool>(1ULL << n, false));

    // Initialize BFS by considering every node as a potential starting point
    for (int i = 0; i < static_cast<int>(n); ++i) {
        int initialMask = 1 << i;
        q.push({i, initialMask, 0});
        visited[static_cast<size_t>(i)][static_cast<size_t>(initialMask)] = true;
    }

    while (!q.empty()) {
        State curr = q.front();
        q.pop();

        // If all nodes have been visited, return the distance immediately
        if (curr.mask == targetMask) {
            return curr.dist;
        }

        // Transition to all neighboring nodes
        for (int neighbor : graph[static_cast<size_t>(curr.node)]) {
            int nextMask = curr.mask | (1 << neighbor);
            
            // Push the new configuration into the queue if it hasn't been explored yet
            if (!visited[static_cast<size_t>(neighbor)][static_cast<size_t>(nextMask)]) {
                visited[static_cast<size_t>(neighbor)][static_cast<size_t>(nextMask)] = true;
                q.push({neighbor, nextMask, curr.dist + 1});
            }
        }
    }

    return -1; // Fallback configuration for disconnected graphs
}

int main() {
    int n;
    cout << "Enter the total number of nodes (maximum 12): ";
    if (!(cin >> n) || n <= 0 || n > 12) {
        cout << "Invalid sizing configuration input bounds." << endl;
        return 1;
    }

    vector<vector<int>> graph(static_cast<size_t>(n));
    cout << "Enter the adjacency list configuration rules.\n";
    for (int i = 0; i < n; ++i) {
        int edgesCount;
        cout << "Enter number of neighbors for node " << i << ": ";
        cin >> edgesCount;
        if (edgesCount > 0) {
            cout << "Enter the neighbor indices separated by spaces: ";
            for (int j = 0; j < edgesCount; ++j) {
                int neighbor;
                cin >> neighbor;
                graph[static_cast<size_t>(i)].push_back(neighbor);
            }
        }
    }

    int result = shortestPathLength(graph);
    cout << "\nThe shortest path length visiting all nodes is: " << result << endl;

    return 0;
}