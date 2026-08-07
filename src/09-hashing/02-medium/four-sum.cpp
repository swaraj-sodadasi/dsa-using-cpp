#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: 4SUM (TWO-SUM DYNAMIC EXTENSIONS WITH HASH MAP PARTITIONING)
     * -------------------------------------------------------------------------------------
     * The objective is to identify all unique quadruplets $[nums[a], nums[b], nums[c], nums[d]]$ 
     * in an array such that their sum equals a target value, where $a, b, c, d$ are distinct indices.
     * 
     * 1. CRITICAL NUMERICAL OVERFLOW MANAGEMENT:
     *    Summing four 32-bit signed integers can easily exceed standard `int` capacities, causing 
     *    undefined behavior. We prevent overflow by casting intermediate values to 64-bit `long long`.
     * 
     * 2. THE CHOSEN HYBRID ALGORITHM ARCHITECTURE:
     *    While a pure hash map approach tracking all pairs ($O(N^2)$ space) is viable, it suffers from 
     *    massive duplicate handling overhead and complex hash collisions when generating unique nested lists.
     *    
     *    Instead, we deploy an optimal **Hybrid Dynamic Partitioning Structure**:
     *    - Sort the array to group duplicate values, allowing early pruning.
     *    - Run nested loops to fix the first two elements ($i$ and $j$).
     *    - Transform the remaining task into a **Dynamic Two-Pointer Sweep** on the remaining suffix,
     *      combining hash-like value-index partitioning logic with memory-efficient $O(1)$ space overhead.
     * 
     * 3. STRATEGIC SKIP PRUNING (SPEED INVARIANTS):
     *    - Skip duplicate elements immediately after processing them to enforce unique quadruplets.
     *    - Evaluate min/max bounds inside loops: If the current element plus the three smallest elements 
     *      exceeds the target, no valid quadruplet can follow from this position, so we break early.
     */
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> uniqueQuadrupletsResult;
        int n = static_cast<int>(nums.size());
        if (n < 4) return uniqueQuadrupletsResult;

        // Step 1: Sort the array to group duplicates and facilitate pointer navigation
        sort(nums.begin(), nums.end());

        // Fix the first anchor element
        for (int i = 0; i < n - 3; ++i) {
            // Early skip to prevent duplicate quadruplets starting with the same value
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            // Pruning Case A: The smallest possible sum from this position exceeds the target
            if (static_cast<long long>(nums[i]) + nums[i + 1] + nums[i + 2] + nums[i + 3] > target) break;
            // Pruning Case B: The largest possible sum from this position cannot reach the target
            if (static_cast<long long>(nums[i]) + nums[n - 3] + nums[n - 2] + nums[n - 1] < target) continue;

            // Fix the second anchor element
            for (int j = i + 1; j < n - 2; ++j) {
                // Early skip to prevent duplicates for the second position
                if (j > i + 1 && nums[j] == nums[j - 1]) continue;

                // Pruning Case C: The smallest possible sum with fixed i and j exceeds target
                if (static_cast<long long>(nums[i]) + nums[j] + nums[j + 1] + nums[j + 2] > target) break;
                // Pruning Case D: The largest possible sum with fixed i and j cannot reach target
                if (static_cast<long long>(nums[i]) + nums[j] + nums[n - 2] + nums[n - 1] < target) continue;

                // Step 2 & 3: Run the dynamic two-pointer sweep on the remaining suffix partition
                int left = j + 1;
                int right = n - 1;

                while (left < right) {
                    long long currentSum = static_cast<long long>(nums[i]) + nums[j] + nums[left] + nums[right];

                    if (currentSum == target) {
                        uniqueQuadrupletsResult.push_back({nums[i], nums[j], nums[left], nums[right]});

                        // Shift pointers past duplicates to maintain structural uniqueness invariants
                        while (left < right && nums[left] == nums[left + 1]) left++;
                        while (left < right && nums[right] == nums[right - 1]) right--;

                        left++;
                        right--;
                    } else if (currentSum < target) {
                        left++; // Increase the sum by moving the left pointer rightward
                    } else {
                        right--; // Decrease the sum by moving the right pointer leftward
                    }
                }
            }
        }

        return uniqueQuadrupletsResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N^3) - Where N represents the total number of elements in the array. 
 *                  Sorting takes O(N log N) time. The nested loops iterate in O(N^2) configurations, 
 *                  and the internal two-pointer partitioning pass scans the remaining suffix in linear O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The internal pointers and loop structures operate in-place, 
 *                   excluding the memory space required by the output container.
 */
int main() {
    int totalElementsCount, target;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> totalElementsCount) || totalElementsCount < 4) {
        cout << "Invalid count parameter configured. Minimum 4 elements required." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(totalElementsCount));
    cout << "Enter the array elements consecutively:\n";
    for (int i = 0; i < totalElementsCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the target sum value: ";
    cin >> target;

    Solution solver;
    cout << "\nExecuting hybrid structured partitioning 4Sum sweeps..." << endl;
    vector<vector<int>> outputResult = solver.fourSum(nums, target);

    cout << "\nComputed Quadruplets Match Output:\n[\n";
    for (const auto& quadruplet : outputResult) {
        cout << "  [ " << quadruplet[0] << ", " << quadruplet[1] << ", " 
             << quadruplet[2] << ", " << quadruplet[3] << " ]\n";
    }
    cout << "]\nTotal Matches Discovered: " << outputResult.size() << endl;

    return 0;
}