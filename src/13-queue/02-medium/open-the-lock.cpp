#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: STATE-SPACE COMBO GRAPH BFS OPTIMIZATION
     * -------------------------------------------------------------------------------------
     * The problem asks for the minimum number of turns to reach a target combination from 
     * "0000" without hitting any "deadends".
     * 
     * 1. SHORT-PATH BFS FOR STATE-SPACE EXPLORATION:
     *    The lock combinations represent nodes in an undirected graph. Each state has 8 
     *    adjacent nodes (turning each of the 4 wheels either up or down). A Breadth-First 
     *    Search (BFS) is optimal here since it guarantees finding the shortest path first 
     *    in an unweighted graph.
     * 
     * 2. COMBINATORIAL STEPPING LAWS:
     *    For each digit at index `i` (from 0 to 3), turning the wheel creates two directions:
     *      - Step Up: `(currentDigit - '0' + 1) % 10`
     *      - Step Down: `(currentDigit - '0' - 1 + 10) % 10`
     * 
     * 3. VISITED AND DEADEND O(1) LOCKOUTS:
     *    Deadends are mapped to a hash set for constant-time lookup. This set also serves as 
     *    the visited tracker. If the starting point "0000" is already in the deadend set, 
     *    no traversal can begin, returning -1 immediately.
     */
    int openLock(const vector<string>& deadends, const string& target) {
        unordered_set<string> visited(deadends.begin(), deadends.end());
        string startState = "0000";

        if (visited.count(startState)) {
            return -1;
        }
        if (startState == target) {
            return 0;
        }

        queue<string> stateQueue;
        stateQueue.push(startState);
        visited.insert(startState);

        int depthTurns = 0;

        while (!stateQueue.empty()) {
            int levelSize = static_cast<int>(stateQueue.size());

            for (int i = 0; i < levelSize; ++i) {
                string currentCombo = stateQueue.front();
                stateQueue.pop();

                if (currentCombo == target) {
                    return depthTurns;
                }

                // Generate all 8 adjacent combination options
                for (int digitIdx = 0; digitIdx < 4; ++digitIdx) {
                    char originalChar = currentCombo[static_cast<size_t>(digitIdx)];

                    // Compute wheel rotations
                    char charUp = static_cast<char>(((originalChar - '0' + 1) % 10) + '0');
                    char charDown = static_cast<char>(((originalChar - '0' - 1 + 10) % 10) + '0');

                    // Option 1: Turn Wheel Up
                    currentCombo[static_cast<size_t>(digitIdx)] = charUp;
                    if (!visited.count(currentCombo)) {
                        visited.insert(currentCombo);
                        stateQueue.push(currentCombo);
                    }

                    // Option 2: Turn Wheel Down
                    currentCombo[static_cast<size_t>(digitIdx)] = charDown;
                    if (!visited.count(currentCombo)) {
                        visited.insert(currentCombo);
                        stateQueue.push(currentCombo);
                    }

                    // Restore the digit index to its original state for the next wheels
                    currentCombo[static_cast<size_t>(digitIdx)] = originalChar;
                }
            }
            depthTurns++;
        }

        return -1; // Unreachable target combination
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(D * (10^N) + M) - Where N represents the number of wheels (4), 
 *                  10^N is the maximum possible combination states (10000), D represents 
 *                  the number of digits per state (4), and M is the size of the deadends list.
 *
 * SPACE COMPLEXITY: O(10^N + M) Auxiliary - To hold visited combination states and the deadends set.
 */
int main() {
    cout << "=== Open the Lock BFS Combo Optimizer ===\n";
    cout << "Enter the number of deadends: ";
    int deadendCount;
    if (!(cin >> deadendCount) || deadendCount < 0) return 1;

    vector<string> deadends;
    if (deadendCount > 0) {
        cout << "Enter the deadend combinations (e.g., '8888'):\n";
        for (int i = 0; i < deadendCount; ++i) {
            string temp;
            cin >> temp;
            deadends.push_back(temp);
        }
    }

    cout << "Enter the target combination (4 digits): ";
    string target;
    cin >> target;

    Solution solver;
    cout << "\nAnalyzing lock state graph patterns and running BFS..." << endl;
    int turns = solver.openLock(deadends, target);

    if (turns != -1) {
        cout << "Minimum lock turn operations required: [" << turns << "]\n";
    } else {
        cout << "The lock is deadlocked. Target unreachable. Output: [-1]\n";
    }

    return 0;
}