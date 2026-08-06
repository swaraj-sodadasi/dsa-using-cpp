#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: O(1) HASH MAP VALUE-INDEX MAPPING
     * -------------------------------------------------------------------------------------
     * The objective is to identify the indices of two numbers in an array that sum to a 
     * specific target value. Each input has exactly one solution, and elements cannot be reused.
     * 
     * 1. THE BRUTE-FORCE VS. COMPLEMENT GAP APPROACH:
     *    A naive nested loop approach checks all pairs, running in O(N^2) time. 
     *    Instead, we re-frame the problem: for any number `nums[i]`, we look for its exact 
     *    mathematical complement: `complement = target - nums[i]`.
     * 
     * 2. SINGLE-PASS O(1) HASH TABLE LOOKUP:
     *    We maintain an `unordered_map` that pairs array values to their index locations (`value -> index`).
     *    As we loop through the array, we check if the current number's complement already exists in the map:
     *      - If found: We have successfully isolated the matching pair and return their indices.
     *      - If not found: We insert the current number and its index into the map, saving it for downstream numbers.
     * 
     * 3. INVARIANT PROTECTION:
     *    Processing elements in a single pass prevents a number from matching with itself, fulfilling the uniqueness rule.
     */
    vector<int> twoSum(vector<int>& nums, int target) {
        // Map stores: {array_value -> element_index}
        unordered_map<int, int> valToIndexMap;

        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            int complement = target - nums[i];

            // Check if the required complement has already been registered
            if (valToIndexMap.find(complement) != valToIndexMap.end()) {
                return {valToIndexMap[complement], i};
            }

            // Register the current value and index position in the lookup table
            valToIndexMap[nums[static_cast<size_t>(i)]] = i;
        }

        return {}; // Fallback return statement if no pair matches the target constraints
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The loop parses each element exactly once, performing hash map insertions 
 *                  and search lookups in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - In the worst-case scenario, the hash map stores up to N 
 *                   distinct key-value pairings before finding the complementary match.
 */
int main() {
    int count, target;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> count) || count < 2) {
        cout << "Invalid count parameter configured. Minimum 2 elements required." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(count));
    cout << "Enter the array elements sequentially:\n";
    for (int i = 0; i < count; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the target sum value: ";
    cin >> target;

    Solution solver;
    cout << "\nExecuting single-pass hash map complement search..." << endl;
    vector<int> indices = solver.twoSum(nums, target);

    if (!indices.empty()) {
        cout << "Matching pair discovered at indices: [" << indices[0] << ", " << indices[1] << "]" << endl;
        cout << "Verification: " << nums[static_cast<size_t>(indices[0])] << " + " 
             << nums[static_cast<size_t>(indices[1])] << " = " << target << endl;
    } else {
        cout << "No matching pair satisfied the target constraints." << endl;
    }

    return 0;
}