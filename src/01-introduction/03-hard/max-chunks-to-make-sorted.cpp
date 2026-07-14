#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (PREFIX MAX AND SUFFIX MIN MONOTONIC COMPARISONS):
- We want to split the array into the maximum number of chunks such that sorting each chunk individually results in a completely sorted array.
- A chunk boundary can be safely placed after index 'i' if and only if every element before or at 'i' is less than or equal to every element after 'i'.
- To verify this condition efficiently, we construct two tracking structures:
  1. A prefix maximum scan: `prefixMax[i]` stores the largest value from index 0 up to 'i'.
  2. A suffix minimum scan: `suffixMin[i]` stores the smallest value from index 'i' down to the last element.
- We iterate through the array from index 0 to 'size - 2'. If `prefixMax[i] <= suffixMin[i + 1]`, it means all elements in the current partition are smaller than or equal to all elements in the upcoming partitions, allowing us to safely slice a chunk.
- The total chunk count starts at 1 (since the entire array itself forms at least one final chunk).

TIME COMPLEXITY: O(N) - We perform a linear pass to build the suffix minimum tracking array and another single pass to count the partitions.
SPACE COMPLEXITY: O(N) - An auxiliary vector of size N is allocated to preserve the suffix minimum state.
*/
int maxChunksToSorted(const vector<int>& arr) {
    size_t n = arr.size();
    if (n == 0) return 0;

    vector<int> suffixMin(n);
    suffixMin[n - 1] = arr[n - 1];
    
    // Construct the suffix minimum tracker from right to left
    for (int i = static_cast<int>(n) - 2; i >= 0; --i) {
        suffixMin[static_cast<size_t>(i)] = min(arr[static_cast<size_t>(i)], suffixMin[static_cast<size_t>(i + 1)]);
    }

    int chunks = 0;
    int currentMax = arr[0];

    // Traverse the array comparing the running prefix max with the next suffix min
    for (size_t i = 0; i < n - 1; ++i) {
        currentMax = max(currentMax, arr[i]);
        if (currentMax <= suffixMin[i + 1]) {
            chunks++;
        }
    }

    // Add 1 for the final remaining chunk segment
    return chunks + 1;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " integers (can contain duplicates and negatives) separated by space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    int result = maxChunksToSorted(arr);
    cout << "\nThe maximum number of chunks that can be individually sorted is: " << result << endl;

    return 0;
}