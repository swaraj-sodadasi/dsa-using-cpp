#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * Custom private helper function to calculate the Greatest Common Divisor (GCD).
     * We implement a clean, standard Euclidean algorithm loop to guarantee environment 
     * compatibility across distinct C++ dialect standards without standard library conflicts.
     */
    int calculateGcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: GEOMETRIC SLOPE HASHING VIA COPRIME FRACTION REDUCTIONS
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum number of points that lie on the same straight line.
     * 
     * 1. THE FLOATING-POINT INACCURACY PROBLEM:
     *    Calculating slopes using raw floating-point numbers ($\Delta y / \Delta x$) leads to precision 
     *    errors and rounding artifacts. Two lines with identical geometric trajectories could produce 
     *    slightly different `double` representations, leading to incorrect hash key lookups.
     * 
     * 2. COPRIME FRACTION REDUCTION RESOLUTION:
     *    Instead of storing floats, we represent the slope as a fully reduced fraction of coprime integers.
     *    For any two points $(x_1, y_1)$ and $(x_2, y_2)$, we compute:
     *      $\Delta x = x_2 - x_1$
     *      $\Delta y = y_2 - y_1$
     *    We divide both $\Delta x$ and $\Delta y$ by their Greatest Common Divisor (GCD). This normalizes 
     *    the fraction into its simplest form: $(\Delta x_{reduced}, \Delta y_{reduced})$. 
     *    We then format this coprime pair into a string key `"dy#dx"` for our hash map registry.
     * 
     * 3. SIGN CANONICALIZATION RULES:
     *    To prevent equivalent slopes from generating different keys due to negative signs (e.g., `-1/2` vs `1/-2`), 
     *    we enforce strict directional canonicalization rules:
     *      - If $\Delta x < 0$, we invert both $\Delta x$ and $\Delta y$.
     *      - If $\Delta x == 0$ (a perfectly vertical line), we force $\Delta y$ to $1$ and $\Delta x$ to $0$.
     *      - If $\Delta y == 0$ (a perfectly horizontal line), we force $\Delta y$ to $0$ and $\Delta x$ to $1$.
     * 
     * 4. ANCHOR EXPANSION LOOPS:
     *    We fix one anchor point `i` at a time and calculate the slopes to all subsequent points `j`. 
     *    A local hash map tracks slope frequencies for the current anchor. We update the global maximum 
     *    by adding $1$ to include the anchor point itself.
     */
    int maxPoints(vector<vector<int>>& points) {
        size_t n = points.size();
        if (n <= 2) {
            return static_cast<int>(n);
        }

        int globalMaxPoints = 0;

        // Step 4: Fix each point as the local coordinate origin anchor
        for (size_t i = 0; i < n; ++i) {
            // Local map stores: {"dy#dx_Coprime_String" -> Collinear_Points_Count}
            unordered_map<string, int> slopeFrequencyMap;
            int localMaxTrack = 0;

            for (size_t j = i + 1; j < n; ++j) {
                int deltaX = points[j][0] - points[i][0];
                int deltaY = points[j][1] - points[i][1];

                // Step 3: Enforce strict directional sign canonicalization rules
                if (deltaX == 0) {
                    deltaY = 1; // Standardized horizontal intercept representation
                } else if (deltaY == 0) {
                    deltaX = 1; // Standardized vertical intercept representation
                } else {
                    // Reduce coordinates down to their absolute coprime form
                    int commonGcd = calculateGcd(abs(deltaX), abs(deltaY));
                    deltaX /= commonGcd;
                    deltaY /= commonGcd;

                    // Ensure the negative sign is always anchored to the numerator/deltaX consistently
                    if (deltaX < 0) {
                        deltaX = -deltaX;
                        deltaY = -deltaY;
                    }
                }

                // Step 2: Formulate the explicit unique string hash map key lookup profile
                string slopeSignatureKey = to_string(deltaY) + "#" + to_string(deltaX);
                slopeFrequencyMap[slopeSignatureKey]++;
                
                localMaxTrack = max(localMaxTrack, slopeFrequencyMap[slopeSignatureKey]);
            }

            // Global update includes the local max count plus the initial fixed anchor node point (+1)
            globalMaxPoints = max(globalMaxPoints, localMaxTrack + 1);
        }

        return globalMaxPoints;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^2 * log(MaxCoordinate)) - Where N represents the total number of points. 
 *                  The outer nested loops check all coordinate pairs ($O(N^2)$ connections). For each 
 *                  pair, calculating the GCD takes logarithmic time proportional to the coordinate values, 
 *                  and hash map operations complete in amortized constant time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal local `slopeFrequencyMap` structure resets on each 
 *                   outer loop iteration, allocating space to store at most N unique slope records.
 */
int main() {
    int totalPointsCount;
    cout << "Enter the total number of coordinate points to evaluate: ";
    if (!(cin >> totalPointsCount) || totalPointsCount < 0) {
        cout << "Invalid points count parameter configured." << endl;
        return 1;
    }

    vector<vector<int>> points(static_cast<size_t>(totalPointsCount), vector<int>(2));
    cout << "Enter the coordinate elements sequentially as space-separated pairs (X Y):\n";
    for (int i = 0; i < totalPointsCount; ++i) {
        cout << "Point [" << i << "]: ";
        cin >> points[static_cast<size_t>(i)][0] >> points[static_cast<size_t>(i)][1];
    }

    Solution solver;
    cout << "\nExecuting fractional reductions and geometric slope hashing sweeps..." << endl;
    int maxCollinearCount = solver.maxPoints(points);

    cout << "Maximum number of collinear points discovered on a single line: " << maxCollinearCount << endl;

    return 0;
}