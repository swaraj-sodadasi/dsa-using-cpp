#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BOUNDARY SIMULATION AND TRAVERSAL):
- We define four boundaries to track our layers: 'top' (0), 'bottom' (rows - 1), 'left' (0), and 'right' (cols - 1).
- We simulate the spiral path by constantly iterating along the edges in a clockwise order and shrinking the boundaries:
  1. Traverse from left to right across the 'top' row, then increment 'top'.
  2. Traverse from top to bottom down the 'right' column, then decrement 'right'.
  3. Traverse from right to left across the 'bottom' row (checking if 'top <= bottom' remains true), then decrement 'bottom'.
  4. Traverse from bottom to top up the 'left' column (checking if 'left <= right' remains true), then increment 'left'.
- The loop continues until the 'top' and 'bottom' boundaries or the 'left' and 'right' boundaries cross each other.

TIME COMPLEXITY: O(R * C) - Every element in the matrix of R rows and C columns is visited exactly once.
SPACE COMPLEXITY: O(1) auxiliary - No extra heap space is allocated beyond the vector used to return the result sequence.
*/
vector<int> spiralOrder(const vector<vector<int>>& matrix) {
    vector<int> result;
    if (matrix.empty() || matrix[0].empty()) return result;

    int top = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left = 0;
    int right = static_cast<int>(matrix[0].size()) - 1;

    while (top <= bottom && left <= right) {
        // 1. Move Left to Right along Top Boundary
        for (int i = left; i <= right; ++i) {
            result.push_back(matrix[static_cast<size_t>(top)][static_cast<size_t>(i)]);
        }
        top++;

        // 2. Move Top to Bottom along Right Boundary
        for (int i = top; i <= bottom; ++i) {
            result.push_back(matrix[static_cast<size_t>(i)][static_cast<size_t>(right)]);
        }
        right--;

        // 3. Move Right to Left along Bottom Boundary
        if (top <= bottom) {
            for (int i = right; i >= left; --i) {
                result.push_back(matrix[static_cast<size_t>(bottom)][static_cast<size_t>(i)]);
            }
            bottom--;
        }

        // 4. Move Bottom to Top along Left Boundary
        if (left <= right) {
            for (int i = bottom; i >= top; --i) {
                result.push_back(matrix[static_cast<size_t>(i)][static_cast<size_t>(left)]);
            }
            left++;
        }
    }

    return result;
}

int main() {
    int rows, cols;
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;

    vector<vector<int>> matrix(rows, vector<int>(cols));
    cout << "Enter the matrix elements row by row (total " << rows * cols << " integers):\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> matrix[i][j];
        }
    }

    vector<int> result = spiralOrder(matrix);

    cout << "\nSpiral order traversal: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}