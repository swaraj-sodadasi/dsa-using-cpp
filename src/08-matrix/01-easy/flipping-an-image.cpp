#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * HORIZONTAL INVERSION FOLLOWED BY BITWISE NEGATION ENGINE
     * --------------------------------------------------------
     * The objective is to flip a binary matrix horizontally, then invert it 
     * bitwise (change 0 to 1, and 1 to 0).
     *
     * Algorithmic & Optimization Strategy:
     * - We can optimize this process into a single pass per row using a two-pointer 
     *   approach (`left` and `right`) moving toward the center.
     * - For any pair of items being mirrored horizontally at indices `left` and `right`:
     *   - Case A: If `image[r][left] == image[r][right]`, their values will remain at 
     *             these positions after the horizontal flip. However, both must be inverted 
     *             bitwise. Thus, we toggle both values in place.
     *   - Case B: If `image[r][left] != image[r][right]`, swapping them changes their 
     *             positions, but performing a bitwise inversion on the swapped values 
     *             cancels out the change (e.g., swapping 1 and 0 gives 0 and 1, then inverting 
     *             gives 1 and 0, which matches the starting state). Therefore, their values 
     *             remain completely unchanged, and we can skip modifying them entirely.
     * - For the center element when the row length is odd (`left == right`), we simply invert 
     *   the single bit value in place.
     *
     * Execution Protocol:
     * - Step 1: Iterate through each row `r` of the matrix.
     * - Step 2: Initialize two pointer trackers, `left = 0` and `right = cols - 1`.
     * - Step 3: Loop while `left <= right`. Apply the optimized parity inversion rules, 
     *   then update the pointer bounds.
     */
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& image) {
        if (image.empty() || image[0].empty()) return image;

        size_t rows = image.size();
        size_t cols = image[0].size();

        // Step 1: Process each row independently
        for (size_t r = 0; r < rows; ++r) {
            size_t left = 0;
            size_t right = cols - 1;

            // Step 2 & 3: Run the optimized two-pointer approach toward the center
            while (left <= right) {
                if (left == right) {
                    // Center element in an odd-length row: invert the bit value
                    image[r][left] ^= 1;
                }
                else if (image[r][left] == image[r][right]) {
                    // Mirror values match: toggle both bits in place
                    image[r][left] ^= 1;
                    image[r][right] ^= 1;
                }
                // If values do not match, they remain unchanged after the combined flip and invert operations

                left++;
                if (right > 0) right--; // Guard against underflow with size_t tracking indices
            }
        }

        return image;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(R * C) - Where R represents row capacity and C represents column capacity 
 *                  of the input grid layout. The two-pointer traversal updates indices in a single pass, 
 *                  evaluating each cell exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The image transformation executes completely in place, 
 *                   modifying matrix values without allocating extra dynamic vectors or storage buffers.
 */
int main() {
    int rows, cols;
    cout << "Enter grid matrix dimensions (Rows followed by Columns): ";
    if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        cout << "Invalid boundary parameter choices configured." << endl;
        return 1;
    }

    vector<vector<int>> image(static_cast<size_t>(rows), vector<int>(static_cast<size_t>(cols)));
    cout << "Enter matrix binary cell values row by row (Only 0s and 1s are allowed):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cin >> image[static_cast<size_t>(r)][static_cast<size_t>(c)];
            
            // Constraint boundary validation check
            if (image[static_cast<size_t>(r)][static_cast<size_t>(c)] != 0 && 
                image[static_cast<size_t>(r)][static_cast<size_t>(c)] != 1) {
                cout << "Constraint Error: Grid element value must be strictly 0 or 1." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting horizontal inversion and bitwise negation pipelines..." << endl;
    vector<vector<int>> resultMatrix = solver.flipAndInvertImage(image);

    cout << "\nProcessed output binary matrix visualization:\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cout << resultMatrix[static_cast<size_t>(r)][static_cast<size_t>(c)] << " ";
        }
        cout << "\n";
    }

    return 0;
}