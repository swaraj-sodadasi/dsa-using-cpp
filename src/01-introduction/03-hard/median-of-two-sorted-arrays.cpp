#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

/*
CORE LOGIC (BINARY SEARCH ON SPLIT PARTITIONS):
- We seek a partition point in both arrays such that the left half contains the exact same number 
  of elements as the right half (or one extra element if the total size is odd).
- To optimize performance, we always perform the binary search on the smaller array (`A`). 
  Let `A` be the smaller array of size `m`, and `B` be the larger array of size `n`.
- We binary search for a partition index `i` in `A`. The corresponding partition index in `B` 
  is automatically determined as `j = (m + n + 1) / 2 - i`.
- A partition is correct when all elements in the left partition are less than or equal to 
  all elements in the right partition. Due to the sorted nature of `A` and `B`, we only need to verify:
    `maxLeftA <= minRightB`  and  `maxLeftB <= minRightA`
- If `maxLeftA > minRightB`, our partition `i` in `A` is too far to the right, so we move our binary search range left.
- If `maxLeftB > minRightA`, our partition `i` in `A` is too far to the left, so we move our binary search range right.
- Edge cases where partitions are empty are handled safely using `INT_MIN` and `INT_MAX`.

TIME COMPLEXITY: O(log(min(M, N))) - Binary search is executed strictly on the smaller of the two arrays.
SPACE COMPLEXITY: O(1) - The partition calculation runs completely in-place using scalar pointers.
*/
double findMedianSortedArrays(const vector<int>& A, const vector<int>& B) {
    // Ensure A is the smaller array
    if (A.size() > B.size()) {
        return findMedianSortedArrays(B, A);
    }

    int m = static_cast<int>(A.size());
    int n = static_cast<int>(B.size());
    int low = 0;
    int high = m;

    while (low <= high) {
        int i = low + (high - low) / 2;
        int j = (m + n + 1) / 2 - i;

        // Bounded boundaries check
        int maxLeftA = (i == 0) ? INT_MIN : A[static_cast<size_t>(i - 1)];
        int minRightA = (i == m) ? INT_MAX : A[static_cast<size_t>(i)];

        int maxLeftB = (j == 0) ? INT_MIN : B[static_cast<size_t>(j - 1)];
        int minRightB = (j == n) ? INT_MAX : B[static_cast<size_t>(j)];

        if (maxLeftA <= minRightB && maxLeftB <= minRightA) {
            // Found the correct partition point
            if ((m + n) % 2 == 1) {
                return max(maxLeftA, maxLeftB);
            } else {
                return (max(maxLeftA, maxLeftB) + min(minRightA, minRightB)) / 2.0;
            }
        } 
        else if (maxLeftA > minRightB) {
            high = i - 1; // Move left in A
        } 
        else {
            low = i + 1;  // Move right in A
        }
    }

    return 0.0; // Fallback condition for invalid input states
}

int main() {
    int m, n;
    cout << "Enter the number of elements in the first sorted array: ";
    cin >> m;
    vector<int> A(m);
    if (m > 0) {
        cout << "Enter " << m << " sorted integers separating each with a space or newline: ";
        for (int i = 0; i < m; ++i) cin >> A[i];
    }

    cout << "Enter the number of elements in the second sorted array: ";
    cin >> n;
    vector<int> B(n);
    if (n > 0) {
        cout << "Enter " << n << " sorted integers separating each with a space or newline: ";
        for (int i = 0; i < n; ++i) cin >> B[i];
    }

    double median = findMedianSortedArrays(A, B);
    cout << "\nThe combined median of the two sorted arrays is: " << median << endl;

    return 0;
}