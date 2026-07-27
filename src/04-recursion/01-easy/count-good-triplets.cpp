#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/*
CORE LOGIC (RECURSIVE MULTI-LOOP BRANCHING STRUCTURES):
- The problem asks us to find the number of "good triplets" $(arr[i], arr[j], arr[k])$ such that:
  1. $0 \le i < j < k < \text{arr.size()}$
  2. $|arr[i] - arr[j]| \le a$
  3. $|arr[j] - arr[k]| \le b$
  4. $|arr[i] - arr[k]| \le c$
- **Simulating Nested Loops via Recursion**:
  - While this is traditionally solved using three nested loops, we can simulate the multi-loop mechanics using a recursive branching structure.
  - The recursive function `countTripletsHelper` maintains:
    - `currentIndex`: The array position currently under consideration.
    - `selectedIndices`: A list/vector tracking the elements chosen so far for the triplet.
- **Branching Decisions & Pruning**:
  - At each step, we have two branching choices:
    1. **Exclude**: Skip the current element and move to `currentIndex + 1`.
    2. **Include**: Choose the current element, add it to our tracking vector, and move to `currentIndex + 1`.
  - **Early Pruning Optimization**:
    - When selecting the second element ($j$), we can immediately check condition 2: $|arr[i] - arr[j]| \le a$. If this fails, we prune the branch early instead of wasting time looking for a third element ($k$).
  - **Base Case**:
    - If `selectedIndices` reaches a size of 3, we verify the remaining conditions (3 and 4). If they pass, we return 1 (a valid triplet found).
    - If `currentIndex` reaches the end of the array before we gather 3 elements, this branch is invalid $\rightarrow$ return 0.

TIME COMPLEXITY: O(N^3) worst-case - Bounded by the total number of unique index combinations ($\binom{N}{3}$). Thanks to our early pruning, the practical runtime is significantly faster.
SPACE COMPLEXITY: O(N) auxiliary - Bounded by the maximum recursion depth, which matches the size of the array.
*/

class Solution {
private:
    int maxA, maxB, maxC;

    int countTripletsHelper(const vector<int>& arr, size_t currentIndex, vector<size_t>& selectedIndices) {
        // Base Case: A triplet has been successfully collected
        if (selectedIndices.size() == 3) {
            int valI = arr[selectedIndices[0]];
            int valJ = arr[selectedIndices[1]];
            int valK = arr[selectedIndices[2]];

            // Check the remaining conditions for j -> k and i -> k
            if (abs(valJ - valK) <= maxB && abs(valI - valK) <= maxC) {
                return 1;
            }
            return 0;
        }

        // Base Case: Traversed the entire array without filling the triplet slots
        if (currentIndex >= arr.size()) {
            return 0;
        }

        // Option 1: Exclude the current element from the triplet selection
        int totalTriplets = countTripletsHelper(arr, currentIndex + 1, selectedIndices);

        // Option 2: Include the current element (subject to early pruning filters)
        bool canInclude = true;
        
        if (selectedIndices.size() == 1) {
            // Early Pruning Check: If we are choosing 'j', verify it aligns with 'i'
            if (abs(arr[selectedIndices[0]] - arr[currentIndex]) > maxA) {
                canInclude = false; // Prune this branch
            }
        }

        if (canInclude) {
            selectedIndices.push_back(currentIndex);
            totalTriplets += countTripletsHelper(arr, currentIndex + 1, selectedIndices);
            selectedIndices.pop_back(); // Backtracking state recovery step
        }

        return totalTriplets;
    }

public:
    int countGoodTriplets(const vector<int>& arr, int a, int b, int c) {
        maxA = a;
        maxB = b;
        maxC = c;
        vector<size_t> selectedIndices;
        return countTripletsHelper(arr, 0, selectedIndices);
    }
};

int main() {
    int n, a, b, c;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n < 3) {
        cout << "Invalid sizing bounds. Triplets require at least 3 elements." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter the array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    cout << "Enter constraint value 'a': ";
    cin >> a;
    cout << "Enter constraint value 'b': ";
    cin >> b;
    cout << "Enter constraint value 'c': ";
    cin >> c;

    Solution solver;
    cout << "\nExecuting recursive multi-loop branching search..." << endl;
    int result = solver.countGoodTriplets(arr, a, b, c);

    cout << "Total number of valid good triplets matches: " << result << endl;

    return 0;
}