#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MAX VALUE OF EQUATION (MONOTONIC DEQUE POINT OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * Equation to maximize: y_i + y_j + |x_i - x_j| where |x_i - x_j| <= k and i < j.
     * 
     * Since points are sorted by x-coordinates, x_j > x_i, so |x_i - x_j| = x_j - x_i.
     * Rewriting equation:
     *   (y_j + x_j) + (y_i - x_i)
     * 
     * For a fixed point j (x_j, y_j), we want to maximize (y_i - x_i) for a previous point i 
     * satisfying: x_j - x_i <= k.
     * 
     * 1. MONOTONIC DEQUE STRUCTURE:
     *    - Store indices `i` in deque such that their `y_i - x_i` values are strictly decreasing.
     * 
     * 2. WINDOW BOUNDARY MAINTENANCE:
     *    - Remove indices `i` from front where `x_j - x_i > k`.
     * 
     * 3. MAXIMUM VALUE CALCULATION:
     *    - `dq.front()` yields the optimal index `i` maximizing `(y_i - x_i)`.
     *    - Update `maxVal = max(maxVal, (y_j + x_j) + (y_dq.front() - x_dq.front()))`.
     * 
     * 4. MAINTAINING DEQUE MONOTONICITY:
     *    - Before pushing `j`, pop indices from back whose `(y - x)` values are `<= (y_j - x_j)`.
     */
    int findMaxValueOfEquation(const vector<vector<int>>& points, int k) {
        int maxValue = INT_MIN;
        deque<int> dq; // Monotonically decreasing deque based on (y_i - x_i)

        for (int j = 0; j < static_cast<int>(points.size()); ++j) {
            int xj = points[static_cast<size_t>(j)][0];
            int yj = points[static_cast<size_t>(j)][1];

            // Remove points outside the x-distance boundary condition xj - xi <= k
            while (!dq.empty() && xj - points[static_cast<size_t>(dq.front())][0] > k) {
                dq.pop_front();
            }

            // Calculate max equation value with optimal point i from front of deque
            if (!dq.empty()) {
                int xi = points[static_cast<size_t>(dq.front())][0];
                int yi = points[static_cast<size_t>(dq.front())][1];
                maxValue = max(maxValue, yj + xj + yi - xi);
            }

            // Maintain monotonic decreasing order of (y_j - x_j) in deque
            int currentDiff = yj - xj;
            while (!dq.empty()) {
                int prevI = dq.back();
                int prevDiff = points[static_cast<size_t>(prevI)][1] - points[static_cast<size_t>(prevI)][0];
                if (prevDiff <= currentDiff) {
                    dq.pop_back();
                } else {
                    break;
                }
            }

            dq.push_back(j);
        }

        return maxValue;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the number of points.
 *                  Each point is pushed and popped from deque at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Deque space storing point indices.
 */
int main() {
    cout << "=== Max Value of Equation Monotonic Deque Engine ===\n";
    cout << "Enter total number of points: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<vector<int>> points(static_cast<size_t>(n), vector<int>(2));
    cout << "Enter points as [x y] pairs sequentially (sorted by x):\n";
    for (int i = 0; i < n; ++i) {
        cin >> points[static_cast<size_t>(i)][0] >> points[static_cast<size_t>(i)][1];
    }

    cout << "Enter maximum x-distance threshold k: ";
    int k;
    if (!(cin >> k) || k < 0) return 1;

    Solution solver;
    int result = solver.findMaxValueOfEquation(points, k);

    cout << "Maximum value of equation: [" << result << "]\n";

    return 0;
}