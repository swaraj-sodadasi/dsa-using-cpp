#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * FREQUENCY-BASED EQUIVALENCY CHECK ENGINE
     * ----------------------------------------
     * The objective is to determine if we can make the `arr` array equal to the `target` 
     * array by reversing any number of non-empty sub-arrays.
     *
     * Mathematical & Algorithmic Strategy:
     * - A core mathematical property of sub-array reversals is that they allow you to sort 
     *   or permute the array into any arbitrary order (similar to bubble sort mechanics, where 
     *   reversing adjacent pairs of elements allows any permutation).
     * - Therefore, `arr` can be transformed into `target` if and only if both arrays contain 
     *   the exact same elements with the exact same frequencies. The actual order of elements 
     *   does not matter.
     *
     * Verification Protocol:
     * - Step 1: Verify if both arrays have identical lengths. If their sizes differ, they cannot 
     *   be made equal. Return `false` immediately.
     * - Step 2: Use an efficient frequency hash map (`unordered_map<int, int>`) to count the frequency 
     *   of each element in `target`.
     * - Step 3: Iterate through `arr` and decrement the frequency counts in the hash map. If an element 
     *   in `arr` is missing from the map or its count drops below 0, the arrays are not equivalent. 
     *   Return `false`.
     * - If the loop completes successfully, the arrays are structurally equivalent. Return `true`.
     */
    bool canBeEqual(vector<int>& target, vector<int>& arr) {
        // Step 1: Base length verification check
        if (target.size() != arr.size()) {
            return false;
        }

        // Step 2: Populate the frequency mapping tracker using the target sequence profile
        unordered_map<int, int> elementCounts;
        for (int num : target) {
            elementCounts[num]++;
        }

        // Step 3: Deplete frequency counts using elements from the candidate array
        for (int num : arr) {
            // If the element does not exist or has already been exhausted, equivalency fails
            if (elementCounts.find(num) == elementCounts.end() || elementCounts[num] == 0) {
                return false;
            }
            elementCounts[num]--;
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the number of elements in the arrays. The algorithm 
 *                  performs a linear pass to record frequencies followed by another linear pass to 
 *                  verify and consume them, running in optimal O(N) average time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization strategy requires an internal hash map 
 *                   container to store the frequency profile, which scales proportionally with 
 *                   the number of unique elements inside the array.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the arrays: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> target(static_cast<size_t>(n));
    cout << "Enter elements of the 'target' array separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> target[static_cast<size_t>(i)];
    }

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter elements of the 'arr' array separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting frequency-based structural equivalency scan..." << endl;
    
    cout << boolalpha;
    bool result = solver.canBeEqual(target, arr);

    cout << "Sub-array reversal equivalency target result: " << result << endl;

    return 0;
}