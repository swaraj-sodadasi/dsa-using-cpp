#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * FREQUENCY ESTIMATOR MATRIX HELPER
     * ---------------------------------
     * Counts how many elements in the matrix are less than or equal to a target candidate value.
     * Uses a linear time staircase traversal starting from the top-right corner.
     */
    int countLessOrEqual(const vector<vector<int>>& matrix, int midValue) {
        int n = static_cast<int>(matrix.size());
        int count = 0;
        int row = 0;
        int col = n - 1;

        while (row < n && col >= 0) {
            if (matrix[static_cast<size_t>(row)][static_cast<size_t>(col)] <= midValue) {
                // If the element is <= midValue, all elements to its left in this row are also <= midValue
                count += (col + 1);
                // Move down to evaluate the next row segment
                row++;
            } else {
                // If the element is too large, move left to decrease the values examined
                col--;
            }
        }
        return count;
    }

public:
    /*
     * VARIANT A: BINARY SEARCH ON VALUE RANGE ENGINE
     * ----------------------------------------------
     * Strategy: Instead of searching indexes, search the numerical range between the matrix's 
     * minimum element (top-left) and maximum element (bottom-right). We narrow down this value 
     * range using a staircase counting method until we isolate the kth smallest element.
     */
    int kthSmallestBinarySearch(vector<vector<int>>& matrix, int k) {
        int n = static_cast<int>(matrix.size());
        int left = matrix[0][0];
        int right = matrix[static_cast<size_t>(n - 1)][static_cast<size_t>(n - 1)];
        int result = left;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            // If the count of elements <= mid is at least k, mid is a candidate answer
            if (countLessOrEqual(matrix, mid) >= k) {
                result = mid;
                right = mid - 1; // Try to find a smaller valid value in the left range
            } else {
                left = mid + 1;  // Not enough smaller elements; search the right range
            }
        }
        return result;
    }

    /*
     * STRUCTURAL ELEMENT MATRIX WRAPPER
     * ---------------------------------
     * Tracks value properties along with cell coordinates for heap node allocations.
     */
    struct MatrixNode {
        int val;
        int row;
        int col;
        
        // Reverse comparator for min-heap arrangement properties
        bool operator>(const MatrixNode& other) const {
            return val > other.val;
        }
    };

    /*
     * VARIANT B: MIN-HEAP MULTI-POINTER EXTRACTOR ENGINE
     * --------------------------------------------------
     * Strategy: Treat the sorted matrix as N sorted lists. We load the first element of each row 
     * into a min-heap. We then extract the minimum element k times, replacing each popped element 
     * with the next element from its respective row.
     */
    int kthSmallestMinHeap(vector<vector<int>>& matrix, int k) {
        int n = static_cast<int>(matrix.size());
        priority_queue<MatrixNode, vector<MatrixNode>, greater<MatrixNode>> minHeap;

        // Initialize the heap by pushing the first element of each row
        // Optimization constraint: push at most min(n, k) items
        int rowsToPush = min(n, k);
        for (int r = 0; r < rowsToPush; ++r) {
            minHeap.push({matrix[static_cast<size_t>(r)][0], r, 0});
        }

        int poppedCount = 0;
        int resultValue = 0;

        // Extract the minimum element k times
        while (!minHeap.empty()) {
            MatrixNode current = minHeap.top();
            minHeap.pop();
            
            poppedCount++;
            resultValue = current.val;

            if (poppedCount == k) {
                break;
            }

            // Insert the next element from the same row into the heap, if available
            if (current.col + 1 < n) {
                minHeap.push({matrix[static_cast<size_t>(current.row)][static_cast<size_t>(current.col + 1)], current.row, current.col + 1});
            }
        }
        return resultValue;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * VARIANT A (Binary Search on Value Range):
 *   - Time Complexity: O(N * log(MaxVal - MinVal)) - Where N is the matrix row/col count. 
 *     The counting function runs in O(N) time at each step of the binary search loop.
 *   - Space Complexity: O(1) Auxiliary - Operates in-place with zero memory allocation.
 *
 * VARIANT B (Min-Heap Strategy):
 *   - Time Complexity: O(K * log(min(N, K))) - We pop and push elements up to K times, 
 *     with each heap adjustment taking logarithmic time.
 *   - Space Complexity: O(min(N, K)) Auxiliary - The heap stores at most N elements simultaneously.
 */
int main() {
    int n;
    cout << "Enter the matrix size grid dimension (N for an N x N matrix): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The dimension size must be greater than 0." << endl;
        return 1;
    }

    vector<vector<int>> matrix(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n)));
    cout << "Enter matrix cells row by row (each row and column must be sorted in ascending order):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[static_cast<size_t>(i)][static_cast<size_t>(j)];
        }
    }

    int k;
    cout << "Enter the targeted index element rank to locate (K): ";
    if (!(cin >> k) || k < 1 || k > n * n) {
        cout << "Constraint Error: K must be within the range [1, N * N]." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting matrix evaluation algorithms..." << endl;

    int binarySearchResult = solver.kthSmallestBinarySearch(matrix, k);
    int minHeapResult = solver.kthSmallestMinHeap(matrix, k);

    cout << "Result via Binary Search on Value Range: " << binarySearchResult << endl;
    cout << "Result via Min-Heap Multi-Pointer Extraction: " << minHeapResult << endl;

    return 0;
}