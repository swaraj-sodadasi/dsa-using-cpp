#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

/*
CORE LOGIC (GEOMETRIC MEDIAN / WEISZFELD'S ALGORITHM):
- The problem asks us to find a point (x, y) that minimizes the sum of Euclidean distances to a given set of customer positions. 
  This point is mathematically known as the **Geometric Median**.
- Unlike the mean (which minimizes squared distances) or the 1D median, the 2D Geometric Median does not have a closed-form algebraic formula. 
  Instead, we must approximate it using optimization algorithms.
- **Weiszfeld's Algorithm** is an iterative reweighted least squares method. It updates the estimate point $P = (x, y)$ using the formula:
  $$ P_{next} = \frac{\sum \frac{P_i}{d_i}}{\sum \frac{1}{d_i}} $$
  Where $P_i$ represents each input coordinate pair, and $d_i$ is the Euclidean distance from the current estimate $P$ to $P_i$.
- **Singularity Edge Case**: If the current estimate exactly hits one of the input points $P_i$, the distance $d_i$ becomes 0, causing a 
  division-by-zero error. To safely handle this, if any distance is extremely close to 0 (less than a threshold `EPS`), we substitute 
  the distance with a tiny value, or we check if shifting slightly yields a better total distance sum.
- **Alternative Optimization Strategy (Gradient Descent with Shrinking Steps)**: 
  A highly stable alternative that naturally bypasses the singularity point issue is to check the 4 primary directions (up, down, left, right) 
  from our current point using a changing step size. If a step decreases the total distance, we move there. Otherwise, we shrink the step size 
  by a fractional factor (e.g., `step /= 2`) and repeat until the step size is smaller than our target precision (e.g., 10^-7).
- Below, the implementation uses the direct step-search heuristic as it offers exceptional convergence stability without numerical instability.

TIME COMPLEXITY: O(N * log(Initial_Delta / Target_Precision)) - Where N is the number of coordinate pairs. The optimization loop runs a 
                 fixed number of times until the step size reaches the precision limit.
SPACE COMPLEXITY: O(1) auxiliary - The coordinates are processed directly out of the input storage configuration using scalar markers.
*/

// Helper function to calculate the total sum of Euclidean distances from a test point to all customer points
double getTotalDistance(double x, double y, const vector<vector<int>>& positions) {
    double totalDist = 0.0;
    for (const auto& pos : positions) {
        double dx = x - pos[0];
        double dy = y - pos[1];
        totalDist += sqrt(dx * dx + dy * dy);
    }
    return totalDist;
}

double getMinDistanceSum(const vector<vector<int>>& positions) {
    // 4 standard cardinal direction search offsets
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {1, -1, 0, 0};

    // Initialize the starting point to the centroid/mean of all points to get close to the optimal region
    double currentX = 0.0;
    double currentY = 0.0;
    for (const auto& pos : positions) {
        currentX += pos[0];
        currentY += pos[1];
    }
    currentX /= positions.size();
    currentY /= positions.size();

    double currentDist = getTotalDistance(currentX, currentY, positions);
    
    // Start with a large step size covering the standard grid coordinate range bounds
    double step = 100.0;
    double precisionLimit = 1e-7;

    // Iteratively adjust positions and shrink the search space radius
    while (step > precisionLimit) {
        bool foundBetterNeighbor = false;

        for (int i = 0; i < 4; ++i) {
            double nextX = currentX + dx[i] * step;
            double nextY = currentY + dy[i] * step;
            double nextDist = getTotalDistance(nextX, nextY, positions);

            // If a neighbor point offers a lower total distance sum, move to it instantly
            if (nextDist < currentDist) {
                currentDist = nextDist;
                currentX = nextX;
                currentY = nextY;
                foundBetterNeighbor = true;
                break; // Break loop early to keep moving in a beneficial direction
            }
        }

        // If no neighboring step produces a better configuration, shrink our search radius resolution
        if (!foundBetterNeighbor) {
            step /= 2.0;
        }
    }

    return currentDist;
}

int main() {
    int n;
    cout << "Enter the number of customer positions: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid customer location configuration size." << endl;
        return 1;
    }

    vector<vector<int>> positions(static_cast<size_t>(n), vector<int>(2));
    cout << "Enter coordinates for each location (x y):\n";
    for (int i = 0; i < n; ++i) {
        cout << "Location " << i + 1 << ": ";
        cin >> positions[static_cast<size_t>(i)][0] >> positions[static_cast<size_t>(i)][1];
    }

    double minSum = getMinDistanceSum(positions);
    
    // Format output to display 5 decimal places of floating-point precision
    cout << fixed << setprecision(5);
    cout << "\nThe minimum total Euclidean distance sum to the optimal service center position is: " << minSum << endl;

    return 0;
}