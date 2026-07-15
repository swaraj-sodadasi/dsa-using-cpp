#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (GEOMETRIC COORDINATE GEOMETRY AND SIDE-LENGTH ANALYSIS):
- A valid square consists of 4 unique vertices that form 4 equal sides and 2 equal diagonals.
- If we calculate the squared Euclidean distances between all possible pairs of the 4 given points, we will generate exactly 6 distance values:
  - 4 values represent the sides of the square.
  - 2 values represent the diagonals of the square.
- To avoid working with floating-point inaccuracies introduced by square root operations, we calculate the squared distance: `(x1 - x2)^2 + (y1 - y2)^2`.
- After calculating all 6 distances, we sort them in ascending order. 
- For a valid square:
  1. The side length must be strictly greater than 0 (verifying the points are not all identical or collapsed: `distances[0] > 0`).
  2. The first 4 sorted distances (representing the sides) must be completely equal to each other: `distances[0] == distances[1] == distances[2] == distances[3]`.
  3. The final 2 sorted distances (representing the diagonals) must be completely equal to each other: `distances[4] == distances[5]`.
  4. By Pythagoras' theorem, the diagonal squared must equal twice the side squared: `distances[4] == 2 * distances[0]`.

TIME COMPLEXITY: O(1) - Calculating 6 pair distances and sorting an array of fixed size 6 takes constant time.
SPACE COMPLEXITY: O(1) - The analysis runs in-place using a fixed-size auxiliary array of 6 elements.
*/

// Helper function to calculate the squared distance between two points
long long getSquaredDistance(const vector<int>& p1, const vector<int>& p2) {
    return static_cast<long long>(p1[0] - p2[0]) * (p1[0] - p2[0]) + 
           static_cast<long long>(p1[1] - p2[1]) * (p1[1] - p2[1]);
}

bool validSquare(const vector<int>& p1, const vector<int>& p2, const vector<int>& p3, const vector<int>& p4) {
    // A square must have 6 distances between all pairs of 4 points
    vector<long long> distances = {
        getSquaredDistance(p1, p2),
        getSquaredDistance(p1, p3),
        getSquaredDistance(p1, p4),
        getSquaredDistance(p2, p3),
        getSquaredDistance(p2, p4),
        getSquaredDistance(p3, p4)
    };

    // Sort distances to group sides and diagonals together
    sort(distances.begin(), distances.end());

    // 1. Check if side length is non-zero (points are distinct)
    // 2. Check if all 4 sides are equal
    // 3. Check if both diagonals are equal
    // 4. Check Pythagoras' property (side^2 + side^2 == diagonal^2)
    return (distances[0] > 0 &&
            distances[0] == distances[1] &&
            distances[1] == distances[2] &&
            distances[2] == distances[3] &&
            distances[4] == distances[5] &&
            distances[4] == 2 * distances[0]);
}

int main() {
    vector<vector<int>> points(4, vector<int>(2));
    
    cout << "Enter the coordinates for 4 points (x y):\n";
    for (int i = 0; i < 4; ++i) {
        cout << "Point " << i + 1 << ": ";
        cin >> points[static_cast<size_t>(i)][0] >> points[static_cast<size_t>(i)][1];
    }

    if (validSquare(points[0], points[1], points[2], points[3])) {
        cout << "\nThe given points form a valid square!" << endl;
    } else {
        cout << "\nThe given points do NOT form a valid square." << endl;
    }

    return 0;
}