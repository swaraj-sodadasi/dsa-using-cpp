#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (IN-PLACE 2D MATRIX TRANSPOSITION AND REFLECTION):
- Rotating a 2D matrix clockwise by 90 degrees can be broken down into two separate topological operations:
  1. Transpose the matrix: Convert all rows into columns. This swaps elements across the main diagonal, 
     meaning `matrix[i][j]` is swapped with `matrix[j][i]` for all `j > i`.
  2. Reflect the matrix horizontally: Reverse each individual row. This swaps elements symmetrical 
     to the middle column, moving the first column to the last, the second to the second-to-last, and so on.
- Performing both of these transformations in sequence results in a perfect 90-degree clockwise rotation 
  without requiring a secondary buffer matrix.

TIME COMPLEXITY: O(N^2) - Transposition visits half the elements (inner loop executes N*(N-1)/2 times). 
                 Reflection reverses N rows of length N, taking O(N^2) steps. Total time is proportional to total matrix cells.
SPACE COMPLEXITY: O(1) - All mutations occur entirely in-place inside the provided vector layer structure.
*/
void rotate(vector<vector<int>>& matrix) {
    size_t n = matrix.size();

    // 1. Transpose the matrix
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }

    // 2. Reflect horizontally (reverse each row)
    for (size_t i = 0; i < n; ++i) {
        reverse(matrix[i].begin(), matrix[i].end());
    }
}

int main() {
    int n;
    cout << "Enter the dimension size N for the square matrix (N x N): ";
    cin >> n;

    vector<vector<int>> matrix(n, vector<int>(n));
    cout << "Enter the matrix elements row by row (total " << n * n << " integers):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i][j];
        }
    }

    rotate(matrix);

    cout << "\nRotated matrix (90 degrees clockwise):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}