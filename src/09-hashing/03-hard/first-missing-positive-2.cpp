#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: IN-PLACE INDEX INVERSION HASH SIMULATION
     * -------------------------------------------------------------------------------------
     * The objective is to identify the smallest missing positive integer from an unsorted 
     * array running in strict O(N) runtime and O(1) constant auxiliary space constraints.
     * 
     * 1. THE FINITE RANGE RESOLUTION PRINCIPLE:
     *    For an array containing N elements, the absolute worst-case scenario for the smallest 
     *    missing positive integer occurs when the array contains all consecutive integers 
     *    from 1 up to N. In this case, the answer is exactly N + 1. 
     *    Therefore, the first missing positive *must* fall within the closed range [1, N + 1]. 
     *    Any number out of this range (negative integers, zeroes, or values greater than N) 
     *    can be safely disregarded as they cannot be the first missing positive.
     * 
     * 2. THE CYCLIC SORT PLACEMENT SIMULATION:
     *    Instead of allocating an extra boolean tracking array or hash set—which would violate 
     *    the O(1) auxiliary space constraint—we swap values within the input array to treat 
     *    it as its own hash map.
     *    We want to place every valid integer `val` (where 1 <= val <= N) at its canonical 
     *    0-based index slot: `index = val - 1`. 
     * 
     *    We iterate through the array. For each element `nums[i]`, we enter a cyclic swap loop:
     *      - Condition to Swap: `nums[i]` is in the range [1, N] AND it is not already at its 
     *        correct position (`nums[i] != nums[nums[i] - 1]`).
     *      - We swap `nums[i]` with the element at its target index. This places at least one 
     *        number in its correct final spot per swap. We repeat until the condition fails.
     * 
     * 3. LINEAR EXTRACTOR SWEEP:
     *    After organizing the elements, we perform a second linear pass through the array. 
     *    The first index `i` where `nums[i] != i + 1` reveals the missing value: `i + 1`. 
     *    If all slots from index 0 to N-1 match perfectly, every value from 1 to N is present, 
     *    so the missing value is `N + 1`.
     */
    int firstMissingPositive(vector<int>& nums) {
        int n = static_cast<int>(nums.size());

        // Step 2: Cyclic Sort - Shuffle elements into their canonical index positions
        for (int i = 0; i < n; ++i) {
            // Continuously swap as long as the current number is a valid target value
            // and its destination slot does not already hold the correct number.
            while (nums[static_cast<size_t>(i)] > 0 && 
                   nums[static_cast<size_t>(i)] <= n && 
                   nums[static_cast<size_t>(i)] != nums[static_cast<size_t>(nums[static_cast<size_t>(i)] - 1)]) {
                
                swap(nums[static_cast<size_t>(i)], nums[static_cast<size_t>(nums[static_cast<size_t>(i)] - 1)]);
            }
        }

        // Step 3: Scan the array to locate the first index mismatch
        for (int i = 0; i < n; ++i) {
            if (nums[static_cast<size_t>(i)] != i + 1) {
                return i + 1; // Found the missing positive number
            }
        }

        // Base Fallback: The array contains all numbers from 1 to N perfectly
        return n + 1;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  Although the inner loop runs inside an outer loop, each swap places 
 *                  at least one element into its correct final position. Since an element 
 *                  can be swapped into its correct place at most once, the total number 
 *                  of swaps across the entire runtime is bounded by N, resulting in an 
 *                  overall amortized O(N) linear runtime performance.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The cyclic sort rearranges elements in place within 
 *                   the existing array boundaries without allocating extra data containers.
 */
int main() {
    int totalElementsCount;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> totalElementsCount) || totalElementsCount < 0) {
        cout << "Invalid sizing parameters configured." << endl;
        return 1;
    }

    if (totalElementsCount == 0) {
        cout << "Smallest missing positive integer on empty set: 1" << endl;
        return 0;
    }

    vector<int> nums(static_cast<size_t>(totalElementsCount));
    cout << "Enter the array elements sequentially (can include negative integers and zero):\n";
    for (int i = 0; i < totalElementsCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting in-place index inversion hash sorting..." << endl;
    int isolatedMissingInteger = solver.firstMissingPositive(nums);

    cout << "The isolated smallest missing positive integer is: " << isolatedMissingInteger << endl;

    return 0;
}