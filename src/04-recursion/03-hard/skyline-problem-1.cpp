#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * DIVIDE AND CONQUER COORDINATE MERGE ENGINE
     * ------------------------------------------
     * This helper function merges two independently calculated skylines into a single 
     * consolidated skyline layout, mirroring the merge step of the Merge Sort algorithm.
     *
     * State Parameters:
     *  - leftSky: The structured critical point layout of the left building group segment.
     *  - rightSky: The structured critical point layout of the right building group segment.
     *
     * Contour Tracking Mechanic:
     *  - We step through both skylines sequentially using two pointers (`i` and `j`).
     *  - At each step, we pick the point with the smaller X-coordinate (`x1` or `x2`) to advance.
     *  - We track the active running height of both segments independently (`leftHeight` and `rightHeight`).
     *  - The true height at any given X coordinate is the maximum of the two active heights: 
     *    `maxHeight = max(leftHeight, rightHeight)`.
     *  - To keep the skyline minimal, we only save a new contour point if `maxHeight` changes 
     *    from the previously stored height boundary.
     */
    vector<vector<int>> mergeSkylines(const vector<vector<int>>& leftSky, const vector<vector<int>>& rightSky) {
        vector<vector<int>> mergedResult;
        size_t i = 0, j = 0;
        int leftHeight = 0, rightHeight = 0;
        int currentX = 0, maxHeight = 0;

        while (i < leftSky.size() && j < rightSky.size()) {
            // Pick the smaller X coordinate to process the next step segment boundary
            if (leftSky[i][0] < rightSky[j][0]) {
                currentX = leftSky[i][0];
                leftHeight = leftSky[i][1]; // Update running left side height profile
                i++;
            } else if (leftSky[i][0] > rightSky[j][0]) {
                currentX = rightSky[j][0];
                rightHeight = rightSky[j][1]; // Update running right side height profile
                j++;
            } else {
                // If coordinates align perfectly, process both simultaneously
                currentX = leftSky[i][0];
                leftHeight = leftSky[i][1];
                rightHeight = rightSky[j][1];
                i++;
                j++;
            }

            // The effective visible height at this coordinate is the max of both profiles
            maxHeight = max(leftHeight, rightHeight);

            // Redundant point elimination: Only append if the height changes from the previous step
            if (mergedResult.empty() || mergedResult.back()[1] != maxHeight) {
                mergedResult.push_back({currentX, maxHeight});
            }
        }

        // Flush any remaining contour points from the left skyline segment
        while (i < leftSky.size()) {
            if (mergedResult.empty() || mergedResult.back()[1] != leftSky[i][1]) {
                mergedResult.push_back(leftSky[i]);
            }
            i++;
        }

        // Flush any remaining contour points from the right skyline segment
        while (j < rightSky.size()) {
            if (mergedResult.empty() || mergedResult.back()[1] != rightSky[j][1]) {
                mergedResult.push_back(rightSky[j]);
            }
            j++;
        }

        return mergedResult;
    }

    /*
     * DIVIDE AND CONQUER RECURSIVE SUBDIVISION FACTORY
     * ------------------------------------------------
     * Recursively splits the buildings input array into halves until individual
     * base case components are isolated, then merges them back up.
     */
    vector<vector<int>> getSkylineBounds(const vector<vector<int>>& buildings, size_t start, size_t end) {
        // BASE CASE: EMPTY INPUT RANGE CONVERGENCE
        if (start > end) {
            return {};
        }

        // BASE CASE: INDIVIDUAL BUILDING ISOLATION
        // A single isolated building maps to exactly two critical points:
        // 1. The top-left anchor start position: [Left, Height]
        // 2. The bottom-right termination drop point: [Right, 0]
        if (start == end) {
            return { {buildings[start][0], buildings[start][2]}, {buildings[start][1], 0} };
        }

        // Segment splitting window optimization
        size_t mid = start + (end - start) / 2;

        // Recursive Divide Step
        vector<vector<int>> leftSkyline = getSkylineBounds(buildings, start, mid);
        vector<vector<int>> rightSkyline = getSkylineBounds(buildings, mid + 1, end);

        // Conquer / Combine Step
        return mergeSkylines(leftSkyline, rightSkyline);
    }

public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        if (buildings.empty()) {
            return {};
        }
        return getSkylineBounds(buildings, 0, buildings.size() - 1);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total number of buildings. 
 *                  The system splits the input space down into a balanced binary tree layout of depth 
 *                  log N. At each level of the tree, merging coordinates takes linear time, O(N).
 *
 * SPACE COMPLEXITY: O(N log N) Auxiliary - Maximum allocation memory footprint utilized to hold intermediate 
 *                   result vector segments inside recursion layers, paired with log N call stack frames.
 */
int main() {
    int n;
    cout << "Enter the total number of buildings: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid building count parameter setting inputs." << endl;
        return 1;
    }

    vector<vector<int>> buildings(static_cast<size_t>(n), vector<int>(3));
    if (n > 0) {
        cout << "Enter the configurations parameters for each building (Left, Right, Height) separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> buildings[static_cast<size_t>(i)][0] 
                >> buildings[static_cast<size_t>(i)][1] 
                >> buildings[static_cast<size_t>(i)][2];
        }
    }

    Solution solver;
    cout << "\nExecuting divide and conquer coordinate boundary splitting..." << endl;
    vector<vector<int>> result = solver.getSkyline(buildings);

    cout << "\nGenerated Skyline Contour Critical Points Key:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "[" << result[i][0] << ", " << result[i][1] << "]";
        if (i + 1 < result.size()) cout << ", ";
    }
    cout << " ]\n";
    cout << "Total contour vertices generated: " << result.size() << endl;

    return 0;
}