#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (RECURSIVE BACKTRACKING WITH FREQUENCY SORTING TO AVOID DUPLICATES):
- The problem asks us to return all possible subsets (the power set) of an integer array `nums` 
  that may contain duplicates. The final solution set must not contain duplicate subsets.
- **Sorting to Group Duplicates**:
  - The key to avoiding duplicate subsets without using an expensive set container is sorting. 
    By sorting `nums` in ascending order, all identical elements are placed next to each other.
- **Backtracking State Representation**:
  - We use a recursive function `backtrack(startIndex, currentSubset)` where:
    - `startIndex`: The array index where the current recursive depth begins looking for elements.
    - `currentSubset`: A dynamic array tracking the elements chosen for the current subset combination.
- **Duplicate Elimination Condition**:
  - At each step in our choice loop from `i = startIndex` to `nums.size() - 1`, we try adding `nums[i]` to our subset.
  - If `i > startIndex` and `nums[i] == nums[i - 1]`, it means we are in the same recursive layer and have already 
    explored a branch starting with this exact duplicate element. 
  - To prevent generating duplicate combinations, we skip (`continue`) this index selection entirely.

TIME COMPLEXITY: O(N * 2^N) - There are $2^N$ possible subsets. For each valid subset branch, copying it into our results array takes $O(N)$ time.
SPACE COMPLEXITY: O(N) auxiliary - The depth of the recursion stack scales linearly with $N$. The space required for `currentSubset` is also $O(N)$.
*/

class Solution {
private:
    vector<vector<int>> allSubsets;

    void findSubsets(const vector<int>& nums, size_t startIndex, vector<int>& currentSubset) {
        // Every state reached in the decision tree represents a valid subset combination
        allSubsets.push_back(currentSubset);

        for (size_t i = startIndex; i < nums.size(); ++i) {
            // Duplicate Skipping Step: Skip duplicate elements at the same recursive layer level
            if (i > startIndex && nums[i] == nums[i - 1]) {
                continue;
            }

            // Include the current element in the subset path
            currentSubset.push_back(nums[i]);

            // Recurse deeper into the array to build upon the current selection choice
            findSubsets(nums, i + 1, currentSubset);

            // Backtracking state recovery step
            currentSubset.pop_back();
        }
    }

public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        allSubsets.clear();
        
        // Critical Step: Sort the vector to cluster duplicate elements together
        sort(nums.begin(), nums.end());
        
        vector<int> currentSubset;
        findSubsets(nums, 0, currentSubset);
        
        return allSubsets;
    }
};

int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input sizing configuration parameter bounds." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter the elements (duplicates allowed) separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> nums[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    vector<vector<int>> result = solver.subsetsWithDup(nums);

    cout << "\nAll unique subsets generated via frequency grouping backtracking:\n";
    cout << "[\n";
    for (const auto& subset : result) {
        cout << "  [ ";
        for (int num : subset) {
            cout << num << " ";
        }
        cout << "]\n";
    }
    cout << "]\n";
    cout << "Total distinct subsets found: " << result.size() << endl;

    return 0;
}