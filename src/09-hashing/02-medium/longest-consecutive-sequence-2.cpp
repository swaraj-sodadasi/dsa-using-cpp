#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: UNORDERED SET BOUNDARY EXTRACTION AND EXPANSION
     * -------------------------------------------------------------------------------------
     * The objective is to find the length of the longest consecutive elements sequence 
     * in an unsorted array of integers, running in strict O(N) time.
     * 
     * 1. HASH CONTAINER SEEDING:
     *    Sorting the array takes O(N log N) time, which violates the runtime constraint. 
     *    Instead, we populate an `unordered_set` with all numbers from the array. This 
     *    deduplicates the values and enables amortized O(1) membership lookups.
     * 
     * 2. SEQUENTIAL STREAK BOUNDARY DETECTION:
     *    To achieve linear time efficiency, we must avoid duplicate work. A naive search 
     *    might start tracking a streak from every number, resulting in O(N^2) worst-case performance. 
     *    We can optimize this by only initiating a streak analysis when we encounter the **absolute 
     *    left boundary** of a sequence. 
     *    For any element `num`, we query the set for `num - 1`:
     *      - If `num - 1` exists: `num` is part of a larger ongoing sequence. We skip it immediately.
     *      - If `num - 1` is missing: `num` is a sequence boundary. We freeze here and start counting.
     * 
     * 3. FORWARD ELEMENT STREAK EXPANSION:
     *    Once a boundary is isolated, we use a loop to check for consecutive numbers (`num + 1`, 
     *    `num + 2`, etc.) inside the hash set, tracking the streak length. We then update our 
     *    global maximum sequence record.
     */
    int longestConsecutive(vector<int>& nums) {
        // Step 1: Seed the hash container to establish constant-time lookups
        unordered_set<int> numRegistry(nums.begin(), nums.end());
        int longestStreakGlobal = 0;

        // Step 2: Iterate through the registry elements to extract streak boundaries
        for (int num : numRegistry) {
            // Check if a smaller adjacent value exists. If it does, skip to avoid duplicate work.
            if (numRegistry.find(num - 1) == numRegistry.end()) {
                int currentNum = num;
                int currentStreakLocal = 1;

                // Step 3: Expand forward sequentially as long as consecutive elements exist
                while (numRegistry.find(currentNum + 1) != numRegistry.end()) {
                    currentNum += 1;
                    currentStreakLocal += 1;
                }

                // Update the global maximum length record
                longestStreakGlobal = max(longestStreakGlobal, currentStreakLocal);
            }
        }

        return longestStreakGlobal;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  Building the initial hash set takes O(N) time. The main loop evaluates 
 *                  each number at most twice (once during the initial set iteration, and 
 *                  potentially once inside the forward expansion loop), executing set lookups 
 *                  in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal `numRegistry` hash set allocates memory space 
 *                   to hold up to N unique element integers in the worst-case scenario.
 */
int main() {
    int capacityCount;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> capacityCount) || capacityCount < 0) {
        cout << "Invalid sizing parameters configured." << endl;
        return 1;
    }

    if (capacityCount == 0) {
        cout << "Longest consecutive sequence length for empty matrix: 0" << endl;
        return 0;
    }

    vector<int> nums(static_cast<size_t>(capacityCount));
    cout << "Enter the array elements sequentially:\n";
    for (int i = 0; i < capacityCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting boundary extraction and linear streak expansion checks..." << endl;
    int resultingStreak = solver.longestConsecutive(nums);

    cout << "Length of the longest isolated consecutive sequence streak: " << resultingStreak << endl;

    return 0;
}