#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (RECURSIVE IN-PLACE ELEMENT SWAPPING):
- The objective is to generate all unique permutations of a collection of distinct integers.
- **In-place Swapping Strategy**:
  - Instead of maintaining an auxiliary array or set to track which elements have been used, we can systematically partition the input array `nums` into two regions: the "fixed" prefix and the "active" suffix.
  - The recursive function `backtrack(startIndex)` processes the array by systematically locking in elements from left to right.
- **State Transition Strategy**:
  - **Base Case**: When `startIndex` equals the size of the array, all positions have been locked in, meaning a complete permutation has been formed. We save a copy of the current state of `nums`.
  - **Branching Step**: For each index `i` from `startIndex` to the end of the array:
    1. Swap the element at `startIndex` with the element at `i`. This sets `nums[i]` as the chosen element for the current position.
    2. Recurse deeper into the array by calling `backtrack(startIndex + 1)` to fill the remaining slots.
    3. Recover the original state by swapping `nums[startIndex]` and `nums[i]` back (Backtracking).

TIME COMPLEXITY: O(N * N!) - There are $N!$ unique permutations. For each permutation, we perform a copy operation taking $O(N)$ time.
SPACE COMPLEXITY: O(N) auxiliary - Bounded by the maximum depth of the recursion stack, which scales linearly with the size of the array.
*/

class Solution {
private:
    vector<vector<int>> allPermutations;

    void generatePermutations(vector<int>& nums, size_t startIndex) {
        // Base Case: All indices have been fixed, a valid permutation is complete
        if (startIndex == nums.size()) {
            allPermutations.push_back(nums);
            return;
        }

        for (size_t i = startIndex; i < nums.size(); ++i) {
            // Swap the element at the current index to the active prefix slot
            swap(nums[startIndex], nums[i]);

            // Recursively move forward to solve the remaining suffix sub-problem
            generatePermutations(nums, startIndex + 1);

            // Backtracking state recovery step
            swap(nums[startIndex], nums[i]);
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) {
        allPermutations.clear();
        generatePermutations(nums, 0);
        return allPermutations;
    }
};

int main() {
    int n;
    cout << "Enter the total number of distinct elements: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input sizing configuration parameter bounds." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter the distinct integer elements separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> nums[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    vector<vector<int>> result = solver.permute(nums);

    cout << "\nAll permutations generated via recursive in-place swapping:\n";
    cout << "[\n";
    for (const auto& permutation : result) {
        cout << "  [ ";
        for (int num : permutation) {
            cout << num << " ";
        }
        cout << "]\n";
    }
    cout << "]\n";
    cout << "Total permutations found: " << result.size() << endl;

    return 0;
}