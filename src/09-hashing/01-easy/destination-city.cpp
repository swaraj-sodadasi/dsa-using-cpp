#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: HASH SET TRACKING FOR OUT-DEGREE ZERO NODES
     * -------------------------------------------------------------------------------------
     * The objective is to identify the final destination city from a list of directed paths.
     * The problem guarantees that the travel graph forms a unique line without cycles, meaning 
     * there is exactly one city that has no path leading out of it (out-degree == 0).
     * 
     * 1. TRACKING DEPARTURE CENTERS:
     *    Instead of building a full adjacency list or performing deep graph traversals, we isolate the
     *    property of departure. If a city appears as a starting point (`paths[i][0]`), its out-degree
     *    is at least 1. We collect all these departure cities into an `unordered_set` called `outgoingCities`.
     * 
     * 2. SINGLE-PASS DESTINATION QUERY:
     *    We loop through the paths a second time, inspecting each arrival city (`paths[i][1]`). 
     *    We check if the arrival city exists inside our `outgoingCities` departure registry:
     *      - If it is missing from the set: This city never serves as a starting point for any path.
     *        Its out-degree is exactly 0. Because a unique terminal city is guaranteed, this city 
     *        is our absolute destination.
     * 
     * 3. AMORTIZED CONSTANT EFFICIENCY:
     *    By converting graph connectivity lookups into simple set membership checks, the execution 
     *    runs in linear time with zero graph-traversal overhead.
     */
    string destCity(vector<vector<string>>& paths) {
        unordered_set<string> outgoingCities;

        // Step 1: Record all cities that have an outgoing path trajectory
        for (const auto& path : paths) {
            outgoingCities.insert(path[0]);
        }

        // Step 2: Identify the unique arrival city that never appears as a departure point
        for (const auto& path : paths) {
            const string& candidateDestination = path[1];
            if (outgoingCities.find(candidateDestination) == outgoingCities.end()) {
                return candidateDestination;
            }
        }

        return ""; // Fallback placeholder return statement
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of path pairs in the input vector. 
 *                  The algorithm sweeps through the paths list twice, executing hash set operations 
 *                  (insertions and lookups) in amortized O(1) constant time per step.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal `outgoingCities` set allocates space to store up 
 *                   to N unique departure city strings in the worst-case scenario.
 */
int main() {
    int totalPaths;
    cout << "Enter the total number of travel paths: ";
    if (!(cin >> totalPaths) || totalPaths <= 0) {
        cout << "Invalid path capacity count configured." << endl;
        return 1;
    }

    vector<vector<string>> paths(static_cast<size_t>(totalPaths), vector<string>(2));
    cout << "Enter each path as a space-separated pair (Departure_City Arrival_City):\n";
    for (int i = 0; i < totalPaths; ++i) {
        cout << "Path [" << i + 1 << "]: ";
        cin >> paths[static_cast<size_t>(i)][0] >> paths[static_cast<size_t>(i)][1];
    }

    Solution solver;
    cout << "\nExecuting out-degree zero hash set lookup evaluations..." << endl;
    string targetDestination = solver.destCity(paths);

    if (!targetDestination.empty()) {
        cout << "Successfully isolated terminal destination city: \"" << targetDestination << "\"\n";
    } else {
        cout << "Error: No valid destination city matching out-degree zero parameters identified." << endl;
    }

    return 0;
}