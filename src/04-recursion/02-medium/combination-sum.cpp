#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (UNBOUNDED KNAPSACK-STYLE RECURSIVE STATE SEARCH):
- The problem asks us to find all unique combinations of distinct candidates where the chosen numbers 
  sum up to a target value. The same number may be chosen from candidates an unlimited number of times.
- **Unbounded Selection Strategy**:
  - We can conceptualize this transition space similarly to the Unbounded Knapsack problem. 
  - At any given recursive state, defined by an active tracking index `candidateIndex` and a remaining `remainingTarget`, 
    we have two branching options to explore:
    1. **Skip**: Disregard the current candidate element entirely and advance forward to the next index slot: 
       `backtrack(candidateIndex + 1, remainingTarget)`.
    2. **Take**: Include the current candidate element in our working combination. Crucially, because selection is 
       unbounded, we do not advance the index pointer after choosing it. We instead remain at the same index slot 
       but subtract its value from the target: `backtrack(candidateIndex, remainingTarget - candidates[candidateIndex])`.
- **Base Cases & Early Pruning Filters**:
  - **Success Convergence**: If `remainingTarget == 0`, the numbers in our working path form a valid combination $\rightarrow$ save it.
  - **Pruning / Failure**: If `remainingTarget < 0` or `candidateIndex == candidates.size()`, the combination is invalid $\rightarrow$ terminate the branch.
  - Optimization: By sorting the `candidates` array in ascending order before starting, we can break out of the choice loops early the moment a single element exceeds `remainingTarget`.

TIME COMPLEXITY: O(2^T) worst-case - Where T is the target value. The exact search space depends heavily on the candidate values, but sorting and structural pruning minimize practical runtime.
SPACE COMPLEXITY: O(T) auxiliary - Bounded by the maximum recursion stack depth, which scales with the target value T when divided by the smallest candidate element.
*/

class Solution {
private:
    vector<vector<int>> allCombinations;

    void findCombinations(const vector<int>& candidates, size_t candidateIndex, int remainingTarget, vector<int>& currentCombination) {
        // Base Case: Target met perfectly, store the combination
        if (remainingTarget == 0) {
            allCombinations.push_back(currentCombination);
            return;
        }

        // Base Case: Target exceeded or out of candidates bounds
        if (candidateIndex >= candidates.size()) {
            return;
        }

        // Branching Decision 1: Skip the current candidate and move to the next index slot
        findCombinations(candidates, candidateIndex + 1, remainingTarget, currentCombination);

        // Branching Decision 2: Take the current candidate (if it doesn't overshoot the target)
        // Optimization: Since the array is sorted, if this candidate is too big, all subsequent ones will be too.
        if (remainingTarget >= candidates[candidateIndex]) {
            // Push the current element onto the working tracker path
            currentCombination.push_back(candidates[candidateIndex]);

            // Recurse on the SAME index since unbounded reuse is permitted
            findCombinations(candidates, candidateIndex, remainingTarget - candidates[candidateIndex], currentCombination);

            // Backtracking state recovery step
            currentCombination.pop_back();
        }
    }

public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        allCombinations.clear();
        
        // Sorting helps apply boundary checks efficiently during exploration
        sort(candidates.begin(), candidates.end());
        
        vector<int> currentCombination;
        findCombinations(candidates, 0, target, currentCombination);
        
        return allCombinations;
    }
};

int main() {
    int n, target;
    cout << "Enter the total number of distinct candidate elements: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid candidate size tracking parameter configuration." << endl;
        return 1;
    }

    vector<int> candidates(static_cast<size_t>(n));
    cout << "Enter the distinct candidates separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> candidates[static_cast<size_t>(i)];
    }

    cout << "Enter the target integer value sum: ";
    if (!(cin >> target) || target < 0) {
        cout << "Invalid target sum value constraint configuration." << endl;
        return 1;
    }

    Solution solver;
    vector<vector<int>> result = solver.combinationSum(candidates, target);

    cout << "\nAll unique combinations generated via unbounded knapsack-style search:\n";
    cout << "[\n";
    for (const auto& combination : result) {
        cout << "  [ ";
        for (int num : combination) {
            cout << num << " ";
        }
        cout << "]\n";
    }
    cout << "]\n";
    cout << "Total distinct combinations found: " << result.size() << endl;

    return 0;
}