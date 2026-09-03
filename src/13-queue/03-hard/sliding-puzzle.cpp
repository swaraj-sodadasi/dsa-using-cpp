#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
private:
    // Predefined 1D index transitions mapping back to 2D movements on a 2x3 board
    const vector<vector<int>> neighbors = {
        {1, 3},       // Index 0 can swap with indices 1, 3
        {0, 2, 4},    // Index 1 can swap with indices 0, 2, 4
        {1, 5},       // Index 2 can swap with indices 1, 5
        {0, 4},       // Index 3 can swap with indices 0, 4
        {1, 3, 5},    // Index 4 can swap with indices 1, 3, 5
        {2, 4}        // Index 5 can swap with indices 2, 4
    };

    // Calculate Manhattan distance heuristic for A* state cost prioritization
    int getManhattanDistance(const string& state) {
        int distance = 0;
        for (int i = 0; i < 6; ++i) {
            char val = state[static_cast<size_t>(i)];
            if (val == '0') continue;
            
            int targetVal = val - '1'; // Target 1D index (0 for '1', 4 for '5')
            int currentR = i / 3, currentC = i % 3;
            int targetR = targetVal / 3, targetC = targetVal % 3;
            
            distance += abs(currentR - targetR) + abs(currentC - targetC);
        }
        return distance;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SLIDING PUZZLE STATE GRAPH BFS WITH A* HEURISTICS
     * -------------------------------------------------------------------------------------
     * The objective is to solve a 2x3 sliding puzzle in the minimum number of moves.
     * The target configuration state is serialized as "123450".
     * 
     * 1. STATE SERIALIZATION:
     *    We flatten the 2x3 grid into a 6-character string. This allows us to track 
     *    visited states efficiently using an unordered_set and simplifies state comparisons.
     * 
     * 2. A* HEURISTIC PRIORITIZATION:
     *    Standard BFS treats all paths equally. We optimize state-space exploration by 
     *    ranking options in a priority queue using A* Search.
     *    - Cost Function: f(n) = g(n) + h(n)
     *    - g(n): Actual move count from the start configuration.
     *    - h(n): Manhattan Distance calculation tracking the cost for each numbered tile 
     *      to reach its destination target position.
     * 
     * 3. GRAPH ADJACENCY TRANSITION:
     *    Swaps are performed strictly between the '0' tile and its valid adjacent slots 
     *    defined inside our neighbor-directed transition table.
     */
    int slidingPuzzle(vector<vector<int>>& board) {
        string startState = "";
        for (int r = 0; r < 2; ++r) {
            for (int c = 0; c < 3; ++c) {
                startState += to_string(board[static_cast<size_t>(r)][static_cast<size_t>(c)]);
            }
        }

        string targetState = "123450";
        if (startState == targetState) return 0;

        // Min-Priority Queue elements: tuple<f_score, g_cost, state_string>
        priority_queue<tuple<int, int, string>, vector<tuple<int, int, string>>, greater<tuple<int, int, string>>> pq;
        unordered_set<string> visited;

        int initialH = getManhattanDistance(startState);
        pq.push({initialH, 0, startState});
        visited.insert(startState);

        while (!pq.empty()) {
            auto [f, g, current] = pq.top();
            pq.pop();

            if (current == targetState) {
                return g;
            }

            int zeroIdx = static_cast<int>(current.find('0'));

            for (int neighbor : neighbors[static_cast<size_t>(zeroIdx)]) {
                string nextState = current;
                swap(nextState[static_cast<size_t>(zeroIdx)], nextState[static_cast<size_t>(neighbor)]);

                if (visited.find(nextState) == visited.end()) {
                    visited.insert(nextState);
                    int nextH = getManhattanDistance(nextState);
                    pq.push({g + 1 + nextH, g + 1, nextState});
                }
            }
        }

        return -1; // Target state is unreachable (insoluble board setup)
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(V * log(V)) - Where V represents the total number of permutations (6! / 2 = 360).
 *                  State sorting via the Priority Queue costs log(V) on inserts.
 *
 * SPACE COMPLEXITY: O(V) Auxiliary - Storing serialization sets and tracking the state queue.
 */
int main() {
    cout << "=== Sliding Puzzle 2x3 A* Solver ===\n";
    vector<vector<int>> board(2, vector<int>(3));
    cout << "Enter the 2x3 board elements sequentially (0 to 5):\n";
    for (int r = 0; r < 2; ++r) {
        cout << "Row " << r + 1 << ": ";
        for (int c = 0; c < 3; ++c) {
            cin >> board[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    Solution solver;
    cout << "\nResolving state-space transitions and routing path bounds..." << endl;
    int moves = solver.slidingPuzzle(board);

    if (moves != -1) {
        cout << "Minimum moves required to solve the puzzle: [" << moves << "]\n";
    } else {
        cout << "The puzzle configuration is insoluble. Output: [-1]\n";
    }

    return 0;
}