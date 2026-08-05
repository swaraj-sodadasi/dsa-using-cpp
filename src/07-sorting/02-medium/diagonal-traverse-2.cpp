#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * BUCKET SORT INDEX-SUM COLLECTION MAPPING ENGINE
     * ------------------------------------------------
     * The objective is to return all elements of a 2D matrix list in diagonal order, 
     * starting from the top-left corner, moving bottom-left to top-right along each diagonal.
     * Note that the grid can be irregular (ragged), where each row has a different column length.
     *
     * Mathematical Alignment Rule:
     * - For any cell grid matrix coordinate position (row, col), all elements sharing the exact same 
     *   index sum value (row + col) belong strictly to the same diagonal line profile.
     * - The value of (row + col) increases monotonically as we move from the top-left diagonal 
     *   down towards the bottom-right diagonal.
     *
     * Algorithmic Strategy (Bucket Mapping Architecture):
     * - Instead of using sorting keys or flat trackers which complicate boundaries in ragged grids, 
     *   we use an index-sum bucket array system (`vector<vector<int>> diagonals`).
     * - The absolute maximum index sum value possible is bounded by the grid dimensions: 
     *   `MaxSum = totalRows + maxColumns`.
     *
     * Traversal Protocol:
     * - Step 1: Walk through the matrix cell by cell. Because elements along the same diagonal line 
     *   must be visited from bottom to top, we iterate through the rows backwards (from the bottom row 
     *   up to row 0) while moving from left to right across the columns.
     * - Step 2: For each cell, compute `indexSum = row + col`. Expand our `diagonals` bucket structure 
     *   dynamically if `indexSum` exceeds the current bucket capacity.
     * - Step 3: Append the cell value into its corresponding index-sum bucket: `diagonals[indexSum].push_back(matrix[row][col])`.
     * - Step 4: Flatten the bucket layers sequentially from bucket 0 up to the maximum index sum, 
     *   assembling them into our linear results vector.
     */
    vector<int> findDiagonalOrder(vector<vector<int>>& nums) {
        vector<vector<int>> diagonals;
        size_t totalElementsCount = 0;

        // Step 1 & 2 & 3: Traverse rows from bottom to top to preserve bottom-left to top-right diagonal order
        for (int r = static_cast<int>(nums.size()) - 1; r >= 0; --r) {
            size_t rowIdx = static_cast<size_t>(r);
            totalElementsCount += nums[rowIdx].size();

            for (size_t c = 0; c < nums[rowIdx].size(); ++c) {
                size_t indexSum = static_cast<size_t>(r) + c;

                // Dynamically resize the bucket container layers as larger index sums are encountered
                if (indexSum >= diagonals.size()) {
                    diagonals.resize(indexSum + 1);
                }

                diagonals[indexSum].push_back(nums[rowIdx][c]);
            }
        }

        // Step 4: Flatten the bucket system sequentially to form the final result array
        vector<int> flattenedResult;
        flattenedResult.reserve(totalElementsCount);

        for (const auto& diagonalBucket : diagonals) {
            for (int val : diagonalBucket) {
                flattenedResult.push_back(val);
            }
        }

        return flattenedResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the grand total number of elements inside the 2D matrix structure. 
 *                  Every cell coordinate is visited exactly once during the initial grid traversal pass, 
 *                  and exactly once during the final sequential bucket flattening step.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The bucket mapping strategy distributes elements into internal 
 *                   vector groups that collectively mirror the exact total size footprint of the input data.
 */
int main() {
    int rows;
    cout << "Enter the total number of rows in the matrix structure: ";
    if (!(cin >> rows) || rows <= 0) {
        cout << "Invalid parameter. Row count must be a positive integer value." << endl;
        return 1;
    }

    vector<vector<int>> nums(static_cast<size_t>(rows));
    cout << "\nSetting up input dimensions for " << rows << " rows (ragged columns allowed):" << endl;
    
    for (int i = 0; i < rows; ++i) {
        int cols;
        cout << "Enter the column element capacity count for row [" << i << "]: ";
        cin >> cols;
        
        if (cols < 0) {
            cout << "Constraint Error: Column counts cannot be negative." << endl;
            return 1;
        }

        if (cols > 0) {
            nums[static_cast<size_t>(i)].resize(static_cast<size_t>(cols));
            cout << "Enter " << cols << " row elements separated by spaces: ";
            for (int j = 0; j < cols; ++j) {
                cin >> nums[static_cast<size_t>(i)][static_cast<size_t>(j)];
            }
        }
    }

    Solution solver;
    cout << "\nExecuting index-sum bucket distribution mapping and flattening sweeps..." << endl;
    vector<int> orderResult = solver.findDiagonalOrder(nums);

    cout << "Diagonal traverse tracking order sequence output:\n[ ";
    for (size_t i = 0; i < orderResult.size(); ++i) {
        cout << orderResult[i] << (i + 1 < orderResult.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}