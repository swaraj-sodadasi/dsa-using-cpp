#include <iostream>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <string>

using namespace std;

/*
CORE LOGIC (GEOMETRIC SLOPE HASHING WITH GCD REDUCTION):
- The goal is to find the maximum number of points that lie on the same straight line.
- Any two distinct points define a unique line. If a third point forms the exact same slope with one 
  of the initial points, all three points must be collinear.
- Using standard floating-point numbers (`double`) to represent slopes (`dy / dx`) creates an accuracy risk due to 
  precision errors (e.g., small decimal differences can cause identical lines to hash incorrectly).
- To circumvent this issue, we represent the slope as a fully reduced fraction pair `(dy, dx)`. 
  We divide both components by their greatest common divisor: `g = std::gcd(dy, dx)`. 
  This normalizes the fraction layout (`dy /= g; dx /= g;`).
- Directional Normalization: To ensure opposite signs track to the exact same slope value, if `dx < 0`, we flip 
  the signs of both `dy` and `dx`. If `dx == 0` (a vertical line), we always force `dy = 1`.
- For each anchor point `i`, we iterate through all other points `j` and calculate their normalized slope fractions relative to `i`.
- We record these slope counts in a local hash map using a string key hash structure `"dy_dx"`.
- We track the peak count for each fixed anchor point `i` and maintain a global running maximum.

TIME COMPLEXITY: O(N^2 * log(maxCoord)) - We use a nested loop strategy to evaluate every point pair combo. 
                 The `std::gcd` calculation runs in logarithmic time proportional to the coordinate scale boundaries.
SPACE COMPLEXITY: O(N) - The hash map dynamically scales up to store at most N unique slope keys during each outer iteration.
*/

// Custom key builder function to turn reduced fractions into stable hash map lookup strings
string getSlopeKey(int dy, int dx) {
    if (dx == 0) return "1_0"; // Normalized indicator representing a vertical line
    if (dy == 0) return "0_1"; // Normalized indicator representing a horizontal line
    
    int g = std::gcd(dy, dx);
    dy /= g;
    dx /= g;
    
    // Maintain a consistent sign structure: force the denominator to be positive
    if (dx < 0) {
        dy = -dy;
        dx = -dx;
    }
    
    return to_string(dy) + "_" + to_string(dx);
}

int maxPoints(const vector<vector<int>>& points) {
    size_t n = points.size();
    if (n <= 2) return static_cast<int>(n);

    int globalMaxPoints = 0;

    for (size_t i = 0; i < n; ++i) {
        unordered_map<string, int> slopeMap;
        int localMax = 0;

        for (size_t j = i + 1; j < n; ++j) {
            int dy = points[j][1] - points[i][1];
            int dx = points[j][0] - points[i][0];

            string slopeKey = getSlopeKey(dy, dx);
            slopeMap[slopeKey]++;
            localMax = max(localMax, slopeMap[slopeKey]);
        }
        
        // Include the current anchor point 'i' in the collinear set calculation (+1)
        globalMaxPoints = max(globalMaxPoints, localMax + 1);
    }

    return globalMaxPoints;
}

int main() {
    int n;
    cout << "Enter the number of points: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid count boundary size." << endl;
        return 1;
    }

    vector<vector<int>> points(static_cast<size_t>(n), vector<int>(2));
    cout << "Enter the coordinates for each point (x y):\n";
    for (int i = 0; i < n; ++i) {
        cout << "Point " << i + 1 << ": ";
        cin >> points[static_cast<size_t>(i)][0] >> points[static_cast<size_t>(i)][1];
    }

    int result = maxPoints(points);
    cout << "\nThe maximum number of points lying on the same straight line is: " << result << endl;

    return 0;
}