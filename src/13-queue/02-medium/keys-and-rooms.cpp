#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: KEYS AND ROOMS (ITERATIVE GRAPH TRAVERSAL VIA FIFO VERIFICATION SETS)
     * -------------------------------------------------------------------------------------
     * The objective is to determine if we can visit all rooms, starting from room 0.
     * Each room contains a set of keys, where each key unlocks another room.
     * 
     * 1. STATE-SPACE TRAVERSAL:
     *    We model the rooms and keys as a directed graph where rooms are nodes and keys 
     *    represent directed edges to other rooms. We use an iterative Breadth-First Search 
     *    (BFS) using a FIFO queue to discover rooms.
     * 
     * 2. VERIFICATION AND BOUNDARY PROTECTION:
     *    - We maintain a 'visited' set (or lookup array) to keep track of rooms we have already 
     *      entered. This prevents redundant processing of rooms and infinite loops.
     *    - CRITICAL FIX: Before enqueuing a key, we must check if it points to a valid room 
     *      index within the boundaries [0, n - 1]. Invalid keys (e.g., out-of-bounds indices) 
     *      must be ignored to prevent segmentation faults or out-of-bound vector assertions.
     * 
     * 3. TERMINAL EVALUATION:
     *    Once the queue is empty, if the number of unique visited rooms equals the total 
     *    number of rooms, we return true; otherwise, we return false.
     */
    bool canVisitAllRooms(const vector<vector<int>>& rooms) {
        int n = static_cast<int>(rooms.size());
        if (n <= 1) return true;

        unordered_set<int> visited;
        queue<int> bfsQueue;

        // Begin traversal at Room 0
        bfsQueue.push(0);
        visited.insert(0);

        while (!bfsQueue.empty()) {
            int currentRoom = bfsQueue.front();
            bfsQueue.pop();

            // Examine all keys inside the current room
            for (int key : rooms[static_cast<size_t>(currentRoom)]) {
                // Safeguard against out-of-bounds keys (e.g., room contains a key for a non-existent room)
                if (key >= 0 && key < n) {
                    if (visited.find(key) == visited.end()) {
                        visited.insert(key);
                        bfsQueue.push(key);
                    }
                }
            }
        }

        // If the size of the visited set equals the total rooms, all rooms are reachable
        return static_cast<int>(visited.size()) == n;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + K) - Where N represents the total number of rooms and K 
 *                  represents the total number of keys (edges) across all rooms.
 *                  Every room and key is processed at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - To hold the visited lookup set and the BFS 
 *                   traversal queue, which can grow up to size N.
 */
int main() {
    cout << "=== Keys and Rooms Iterative BFS Solver ===\n";
    cout << "Enter the total number of rooms (N): ";
    int numRooms;
    if (!(cin >> numRooms) || numRooms <= 0) return 1;

    vector<vector<int>> rooms(static_cast<size_t>(numRooms));
    cout << "Enter the keys inside each room (enter count first, then key list):\n";
    for (int i = 0; i < numRooms; ++i) {
        cout << "Room [" << i << "] keys count: ";
        int count;
        cin >> count;
        if (count > 0) {
            cout << "  Enter " << count << " keys (room indices): ";
            for (int j = 0; j < count; ++j) {
                int key;
                cin >> key;
                rooms[static_cast<size_t>(i)].push_back(key);
            }
        }
    }

    Solution solver;
    cout << "\nAnalyzing reachability paths and verifying room keys..." << endl;
    bool status = solver.canVisitAllRooms(rooms);

    if (status) {
        cout << "All rooms are successfully reachable! Output: [true]\n";
    } else {
        cout << "Some rooms remain locked and unreachable. Output: [false]\n";
    }

    return 0;
}