#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * RECURSIVE DFS/BFS GRID COLOR REASSIGNMENT ENGINE
     * ------------------------------------------------
     * The objective is to perform a flood fill on an image representation, starting 
     * from a given pixel location (sr, sc) and modifying the color of all adjacent 
     * connected pixels that match the starting position's original color.
     *
     * Algorithmic & Traversal Strategy:
     * - This task can be modeled as finding all reachable nodes within a connected component 
     *   on an unweighted 2D pixel coordinate graph.
     * - We implement a Breadth-First Search (BFS) model using an explicit queue block.
     * - Before initiating traversal, we must check if the original color of the target pixel 
     *   is already equal to the new color. If they match, the current grid configuration is 
     *   already fully resolved, and we return the grid immediately to prevent infinite loops.
     *
     * Execution Protocol Steps:
     * - Step 1: Capture the baseline target color tracking parameters from image[sr][sc].
     * - Step 2: Initialize a tracking coordinate queue (`coordinateQueue`) and push the initial indices.
     * - Step 3: Loop through the queue. Pop the coordinate pairs, rewrite their values with `color`, 
     *   and check all 4 orthogonal directional offsets (Up, Down, Left, Right).
     * - Step 4: If an adjacent neighbor lies within grid boundaries and contains the original baseline color, 
     *   enqueue it for upcoming modifications.
     */
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int rows = static_cast<int>(image.size());
        int cols = static_cast<int>(image[0].size());
        
        // Capture baseline structural threshold parameters
        int initialColor = image[static_cast<size_t>(sr)][static_cast<size_t>(sc)];
        
        // Guard step: If the target cell is already assigned the new color, terminate processing
        if (initialColor == color) {
            return image;
        }

        // Initialize FIFO queue layout to manage coordinates sequentially
        queue<pair<int, int>> coordinateQueue;
        coordinateQueue.push({sr, sc});
        
        // Reassign the root cell location color immediately
        image[static_cast<size_t>(sr)][static_cast<size_t>(sc)] = color;

        // Orthogonal directional tracking navigation matrices
        int delRow[] = {-1, 0, 1, 0};
        int delCol[] = {0, 1, 0, -1};

        // Step 3 & 4: Execute BFS coordinate validation loops
        while (!coordinateQueue.empty()) {
            auto [currRow, currCol] = coordinateQueue.front();
            coordinateQueue.pop();

            for (int i = 0; i < 4; ++i) {
                int neighborRow = currRow + delRow[i];
                int neighborCol = currCol + delCol[i];

                // Check boundary limitations and color matching profiles
                if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                    if (image[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] == initialColor) {
                        // Reassign pixel color state in-place to save structural memory tracking steps
                        image[static_cast<size_t>(neighborRow)][static_cast<size_t>(neighborCol)] = color;
                        coordinateQueue.push({neighborRow, neighborCol});
                    }
                }
            }
        }

        return image;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents the row capacity and C represents column capacity 
 *                  of the matrix image. In the worst-case scenario, the algorithm inspects and 
 *                  transforms every single pixel grid location exactly once.
 *
 * SPACE COMPLEXITY: O(R * C) Auxiliary - The implicit memory footprint consumed by the tracking queue 
 *                   during runtime checks scales proportionally to the boundary footprint perimeter.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid parameters configured." << endl;
        return 1;
    }

    vector<vector<int>> image(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix pixel color integer values row by row:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> image[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }

    int sr, sc, newColor;
    cout << "\nEnter seed coordinate indices (sr, sc) and target new color index value: ";
    cin >> sr >> sc >> newColor;

    if (sr < 0 || sr >= rows || sc < 0 || sc >= cols) {
        cout << "Boundary Error: Seed pixel coordinates fall outside the grid bounds." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting flood fill component exploration routines..." << endl;
    vector<vector<int>> resultGrid = solver.floodFill(image, sr, sc, newColor);

    cout << "\nPost-reassignment updated grid matrix configuration visualization:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cout << resultGrid[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}