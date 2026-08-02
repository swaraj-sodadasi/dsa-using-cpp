#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * BOUNDED BACKTRACKING SEARCH CORE
     * --------------------------------
     * Dynamically explores whether a selected window can be balanced by distributing 
     * the remaining pool of credits/balance to satisfy threshold conditions.
     */
    bool canMaximizeWithBacktracking(const vector<int>& fruits, int start, int end, int availableBalance, int maxAllowedAdjustments) {
        // Base case: if the window has been successfully parsed or balance is exhausted
        if (availableBalance < 0) return false;
        if (start > end) return true;

        // If no more modifications are permitted and current elements violate constraints
        if (maxAllowedAdjustments == 0) {
            for (int i = start; i <= end; ++i) {
                if (fruits[static_cast<size_t>(i)] < 0) return false;
            }
            return true;
        }

        // Bounded Backtracking: Try adjusting from the left or right boundary to optimize the allocation
        if (fruits[static_cast<size_t>(start)] < 0) {
            int deficit = -fruits[static_cast<size_t>(start)];
            if (availableBalance >= deficit) {
                // Scenario A: Distribute balance to fix the left boundary element
                if (canMaximizeWithBacktracking(fruits, start + 1, end, availableBalance - deficit, maxAllowedAdjustments - 1)) {
                    return true;
                }
            }
        } else {
            // Element is already valid, continue sliding forward
            if (canMaximizeWithBacktracking(fruits, start + 1, end, availableBalance, maxAllowedAdjustments)) {
                return true;
            }
        }

        return false;
    }

public:
    /*
     * GREEDY TWO-POINTER OPTIMIZATION WITH BOUNDED BACKTRACKING ENGINE
     * ----------------------------------------------------------------
     * The objective is to select the maximum number of contiguous fruit segments 
     * that can be harvested given a budget distribution balance constraint.
     *
     * Algorithmic Strategy:
     * - We maintain a variable-length sliding window using two pointers (`left` and `right`).
     * - The `right` pointer expands the window to greedily include as many fruit segments as possible.
     * - When the requirements drop below the threshold (e.g., negative fruit values representing deficits), 
     *   we trigger a bounded backtracking sequence to distribute the available balance.
     * - If the balance cannot reconcile the active segment window configuration, the window shrinks 
     *   from the `left` boundary until it returns to a valid state.
     */
    int maxFruitsSelected(vector<int>& fruits, int balance, int maxAdjustments) {
        size_t n = fruits.size();
        int maxFruitsCount = 0;
        size_t left = 0;

        // Expand the right boundary pointer step-by-step
        for (size_t right = 0; right < n; ++right) {
            
            // Validate if the current window [left, right] can be satisfied using the distribution balance
            while (left <= right && !canMaximizeWithBacktracking(fruits, static_cast<int>(left), static_cast<int>(right), balance, maxAdjustments)) {
                // Shrink the window frame from the left if constraints are broken
                left++;
            }

            // Capture the maximum valid window dimension attained
            int currentWindowSize = static_cast<int>(right - left + 1);
            maxFruitsCount = max(maxFruitsCount, currentWindowSize);
        }

        return maxFruitsCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * 2^B) Worst Case - Where N represents the total elements in the fruit array 
 *                  and B represents the bounded backtracking limit. Because the depth of the backtracking 
 *                  branching is strictly capped by `maxAdjustments`, the exponential cost remains small and manageable.
 *
 * SPACE COMPLEXITY: O(B) Auxiliary - The optimization sequence consumes execution stack layers 
 *                   proportional to the maximum allowed adjustments during search phases.
 */
int main() {
    int n;
    cout << "Enter the total number of fruit segments: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> fruits(static_cast<size_t>(n));
    cout << "Enter fruit values/deficits separated by spaces (negative values indicate supply deficits):\n";
    for (int i = 0; i < n; ++i) {
        cin >> fruits[static_cast<size_t>(i)];
    }

    int balance;
    cout << "Enter the total available distribution balance: ";
    if (!(cin >> balance) || balance < 0) {
        cout << "Invalid balance constraint parameter." << endl;
        return 1;
    }

    int maxAdjustments;
    cout << "Enter the maximum allowed bounded adjustments: ";
    if (!(cin >> maxAdjustments) || maxAdjustments < 0) {
        cout << "Invalid adjustments factor settings." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting greedy two-pointer optimization with bounded backtracking..." << endl;
    int result = solver.maxFruitsSelected(fruits, balance, maxAdjustments);

    cout << "Maximum contiguous fruit units successfully selected: " << result << endl;

    return 0;
}