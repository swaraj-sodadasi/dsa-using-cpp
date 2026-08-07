#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

class Solution {
private:
    /*
     * Private helper to hash 2D coordinates into a single unique 64-bit integer string key.
     * This avoids custom pair hashing overhead while preventing key collisions in our 
     * unordered_set up to large grid sizes (e.g., N = 10^9).
     */
    string hashCoordinate(int r, int c) {
        return to_string(r) + "," + to_string(c);
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ROW, COLUMN, AND DUAL-DIAGONAL MULTI-MAP FREQUENCY TRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to manage a grid system where lamps light up whole rows, columns, 
     * and diagonals, and query whether specific cells are illuminated. Queries can also 
     * turn off adjacent lamps.
     * 
     * 1. THE LARGE GRID SCALE CHALLENGE:
     *    With grid sizes up to N = 10^9, using a literal 2D matrix layout will crash due to 
     *    out-of-memory errors. We must use a sparse tracking approach.
     * 
     * 2. THE GEOMETRIC INVARIANT PROJECTIONS:
     *    A lamp placed at coordinate (r, c) illuminates four distinct geometric components:
     *      - Row Profile: Matched by row index `r`.
     *      - Column Profile: Matched by column index `c`.
     *      - Main Diagonal (\): Every cell shares the mathematical invariant property `r - c`.
     *      - Anti-Diagonal (/): Every cell shares the mathematical invariant property `r + c`.
     *    We maintain four independent `unordered_map<int, int>` tracking tables to count how many 
     *    active lamps contribute light to each projection line.
     * 
     * 3. UNIQUE DEDUPLICATION LAMPS REGISTRY:
     *    The input can contain duplicate lamp coordinates. To prevent multiple registrations 
     *    of the same lamp from distorting our illumination lines, we store unique lamp locations 
     *    in an `unordered_set<string> lampsRegistry` using custom coordinate hashing strings.
     * 
     * 4. QUERY RESPONSIVENESS AND THE 9-CELL LOCAL EVICTION SWEEP:
     *    For each query cell (r, c):
     *      - We check if its row, column, or either diagonal count is greater than 0. If yes, it's lit.
     *      - We then check all 9 cells in its immediate neighborhood (including itself). If an active 
     *        lamp is found in the registry, we erase it and decrement the matching projection map lines.
     */
    vector<int> gridIllumination(int n, vector<vector<int>>& lamps, vector<vector<int>>& queries) {
        unordered_map<int, int> rowFreq;
        unordered_map<int, int> colFreq;
        unordered_map<int, int> diagFreq;
        unordered_map<int, int> antiDiagFreq;
        unordered_set<string> lampsRegistry;

        // Step 3: Populate our registries and light up projection lines
        for (const auto& lamp : lamps) {
            int r = lamp[0];
            int c = lamp[1];
            string key = hashCoordinate(r, c);

            // Skip if a duplicate lamp is already active at this location
            if (lampsRegistry.find(key) == lampsRegistry.end()) {
                lampsRegistry.insert(key);
                rowFreq[r]++;
                colFreq[c]++;
                diagFreq[r - c]++;
                antiDiagFreq[r + c]++;
            }
        }

        vector<int> queryResults;
        queryResults.reserve(queries.size());

        // Step 4: Process illumination queries and handle neighbor turn-offs
        for (const auto& query : queries) {
            int qr = query[0];
            int qc = query[1];

            // A cell is illuminated if any of its structural line projections contain at least one lamp
            if (rowFreq[qr] > 0 || colFreq[qc] > 0 || diagFreq[qr - qc] > 0 || antiDiagFreq[qr + qc] > 0) {
                queryResults.push_back(1);
            } else {
                queryResults.push_back(0);
            }

            // Perform a local 3x3 sweep to turn off adjacent lamps
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = qr + dr;
                    int nc = qc + dc;

                    // Ensure neighbor coordinates stay inside valid grid boundaries
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                        string neighborKey = hashCoordinate(nr, nc);
                        auto it = lampsRegistry.find(neighborKey);
                        
                        if (it != lampsRegistry.end()) {
                            // Extinguish the lamp and update structural lines
                            lampsRegistry.erase(it);
                            rowFreq[nr]--;
                            colFreq[nc]--;
                            diagFreq[nr - nc]--;
                            antiDiagFreq[nr + nc]--;
                        }
                    }
                }
            }
        }

        return queryResults;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(L + Q) - Where L is the total number of lamps and Q is the total number 
 *                  of queries. Setting up the projection maps takes O(L) time. Evaluating 
 *                  each query takes O(1) time because the neighborhood sweep always checks exactly 
 *                  9 adjacent slots using constant-time hash map operations.
 *
 * SPACE COMPLEXITY: O(L) Auxiliary - The system allocates space proportional to the number of unique 
 *                   lamps to maintain the structural map frequencies and position coordinates.
 */
int main() {
    int n, lampCount, queryCount;
    cout << "Enter the grid dimension boundaries size (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid grid dimensions configured." << endl;
        return 1;
    }

    cout << "Enter the total number of lamps to position: ";
    if (!(cin >> lampCount) || lampCount < 0) {
        return 1;
    }

    vector<vector<int>> lamps(static_cast<size_t>(lampCount), vector<int>(2));
    cout << "Enter the lamp coordinates sequentially as space-separated pairs (Row Column):\n";
    for (int i = 0; i < lampCount; ++i) {
        cin >> lamps[static_cast<size_t>(i)][0] >> lamps[static_cast<size_t>(i)][1];
    }

    cout << "Enter the total number of target illumination queries: ";
    if (!(cin >> queryCount) || queryCount < 0) {
        return 1;
    }

    vector<vector<int>> queries(static_cast<size_t>(queryCount), vector<int>(2));
    cout << "Enter the query coordinates sequentially as space-separated pairs (Row Column):\n";
    for (int i = 0; i < queryCount; ++i) {
        cin >> queries[static_cast<size_t>(i)][0] >> queries[static_cast<size_t>(i)][1];
    }

    Solution solver;
    cout << "\nExecuting sparse multidimensional line frequency evaluations..." << endl;
    vector<int> outcome = solver.gridIllumination(n, lamps, queries);

    cout << "\nComputed Query Traversal Output Results:\n[ ";
    for (size_t i = 0; i < outcome.size(); ++i) {
        cout << outcome[i] << (i + 1 < outcome.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}