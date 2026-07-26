#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BITMASK DP WITH BACKTRACKING PRUNING):
- The problem asks whether we can satisfy a set of customer quantity requests (`quantity`) using an array of integers (`nums`). 
  The constraint is that all integers allocated to a single customer must be identical.
- **Frequency Compression**:
  - The actual values in `nums` do not matter; only their frequency distributions do. 
  - We compress `nums` by counting the frequencies of each unique integer and storing these counts in a vector `counts`.
- **State Representation via Bitmasks**:
  - The number of customers $M$ is very small (typically $M \le 10$). This allows us to use an $M$-bit integer `mask` to track the subset of satisfied customers.
  - A bit value of 1 at the $i$-th position indicates that `quantity[i]` has been fully satisfied.
- **Submask Precomputation**:
  - For every possible `mask` from $0$ to $2^M - 1$, we calculate the total quantity required to satisfy that subset of customers: `submaskSums[mask]`.
- **Dynamic Programming with Backtracking State Transition**:
  - We define a recursive function `canDistributeFromIndex(countIndex, satisfiedMask)` with memoization.
  - `countIndex` tracks which unique number frequency bucket from our `counts` vector we are currently allocating.
  - `satisfiedMask` tracks the current subset of satisfied customers.
  - At each step, we look at the remaining unsatisfied customers (`unsatMask = (totalStates - 1) ^ satisfiedMask`).
  - Instead of trying every individual customer, we iterate through all **mathematical submasks** of `unsatMask` using the bitwise optimization: `submask = (submask - 1) & unsatMask`.
  - If the available frequency `counts[countIndex]` is greater than or equal to the total quantity demanded by this submask (`submaskSums[submask]`), we can fulfill this entire batch of customers at once. 
  - We transition to the next unique value index: `canDistributeFromIndex(countIndex + 1, satisfiedMask | submask)`.
  - If any path yields a complete satisfaction mask (`(1 << M) - 1`), we return `true`.

TIME COMPLEXITY: O(N * 3^M) worst-case, where N is the number of unique integer frequencies and M is the number of customers. The submask enumeration over all combinations of masks naturally scales bound by $3^M$, which executes extremely fast for $M \le 10$.
SPACE COMPLEXITY: O(N * 2^M) auxiliary - Storage allocated for the memoization table mapping unique value index iterations against subset configurations.
*/

class Solution {
private:
    vector<int> uniqueCounts;
    vector<int> quantity;
    vector<int> submaskSums;
    vector<vector<int>> memo;
    size_t totalStates;

    bool canDistributeFromIndex(size_t countIndex, int satisfiedMask) {
        // Base Case: All customers have been successfully satisfied
        if (satisfiedMask == static_cast<int>(totalStates - 1)) {
            return true;
        }
        
        // Base Case: Ran out of unique integer frequency pools before satisfying all customers
        if (countIndex >= uniqueCounts.size()) {
            return false;
        }

        // Return cached result if this state combination has been explored before
        if (memo[countIndex][static_cast<size_t>(satisfiedMask)] != -1) {
            return memo[countIndex][static_cast<size_t>(satisfiedMask)] == 1;
        }

        // Identify the inverse mask containing only the unsatisfied customers
        int unsatMask = static_cast<int>(totalStates - 1) ^ satisfiedMask;
        
        // Iterate through all submasks of unsatisfied customers
        // We start with the full unsatMask and work down to 0
        int submask = unsatMask;
        while (submask > 0) {
            // Pruning: Check if the current unique number pool can fulfill this subset batch requirement
            if (uniqueCounts[countIndex] >= submaskSums[static_cast<size_t>(submask)]) {
                // Try satisfying this submask batch and move to the next frequency bucket
                if (canDistributeFromIndex(countIndex + 1, satisfiedMask | submask)) {
                    memo[countIndex][static_cast<size_t>(satisfiedMask)] = 1;
                    return true;
                }
            }
            // Bitwise degradation to obtain the next mathematical submask sequence
            submask = (submask - 1) & unsatMask;
        }

        // Option to skip the current unique number frequency bucket entirely without allocating it to anyone
        if (canDistributeFromIndex(countIndex + 1, satisfiedMask)) {
            memo[countIndex][static_cast<size_t>(satisfiedMask)] = 1;
            return true;
        }

        memo[countIndex][static_cast<size_t>(satisfiedMask)] = 0;
        return false;
    }

public:
    bool canDistribute(const vector<int>& nums, const vector<int>& customerQuantities) {
        quantity = customerQuantities;
        size_t m = quantity.size();
        totalStates = 1ULL << m;

        // Step 1: Compress nums into frequency buckets
        unordered_map<int, int> frequencyMap;
        for (int num : nums) {
            frequencyMap[num]++;
        }

        uniqueCounts.clear();
        for (const auto& pair : frequencyMap) {
            uniqueCounts.push_back(pair.second);
        }

        // Optimization: Sort frequencies in descending order to trigger backtracking pruning early
        sort(uniqueCounts.rbegin(), uniqueCounts.rend());
        
        // Optimization: Sort customer quantities descending so larger constraints are tested first
        sort(quantity.rbegin(), quantity.rend());

        // Step 2: Precompute total required quantity sums for every possible subset mask configuration
        submaskSums.assign(totalStates, 0);
        for (size_t mask = 0; mask < totalStates; ++mask) {
            int currentSum = 0;
            for (size_t i = 0; i < m; ++i) {
                if ((mask & (1ULL << i)) != 0) {
                    currentSum += quantity[i];
                }
            }
            submaskSums[mask] = currentSum;
        }

        // Initialize memoization table with -1 (representing unvisited state pathways)
        memo.assign(uniqueCounts.size(), vector<int>(totalStates, -1));

        return canDistributeFromIndex(0, 0);
    }
};

int main() {
    int n, m;
    cout << "Enter the total number of items in the integer pool array (nums): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid pool array size initialization parameters." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the elements of the array separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the total number of customers (maximum 10): ";
    if (!(cin >> m) || m <= 0 || m > 10) {
        cout << "Invalid constraint bounds. Customer group size must be between 1 and 10." << endl;
        return 1;
    }

    vector<int> quantity(static_cast<size_t>(m));
    cout << "Enter the quantity requested by each customer separated by spaces:\n";
    for (int i = 0; i < m; ++i) {
        cin >> quantity[static_cast<size_t>(i)];
    }

    Solution solver;
    if (solver.canDistribute(nums, quantity)) {
        cout << "\nSUCCESS: All repeating integer quantity combinations can be successfully distributed!" << endl;
    } else {
        cout << "\nFAILURE: The repeating integer distribution constraints cannot be fulfilled." << endl;
    }

    return 0;
}