#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

class Solution {
public:
    /*
     * HASH SET SEARCH INTEGRATION ENGINE
     * ----------------------------------
     * The objective is to check if there exist two integers N and M such that N is the 
     * double of M (i.e., N = 2 * M) inside an unsorted integer array.
     *
     * Algorithmic Strategy:
     * - A naive approach uses nested loops to compare all pairs, taking O(N^2) quadratic time.
     * - We optimize this to O(N) linear time using a hash set (`unordered_set`) to achieve 
     *   constant-time O(1) lookups for complementary values.
     * - As we perform a single pass linear sweep through the array, for each element `num`, 
     *   we search our hash set for two potential matching conditions:
     *     1. `num * 2`: Checks if the current value is the half component of an existing value.
     *     2. `num / 2` (only if `num % 2 == 0`): Checks if the current value is the double component 
     *        of an existing value.
     * - If either complement is discovered in the hash set, the existential condition is satisfied, 
     *   and we return true immediately. Otherwise, we insert `num` into the set and continue.
     */
    bool checkIfExist(vector<int>& arr) {
        unordered_set<int> visitedElements;

        // Perform a single pass lookup and storage registration sequence
        for (int num : arr) {
            // Check condition 1: Has the double of the current element been registered?
            if (visitedElements.count(num * 2) > 0) {
                return true;
            }
            
            // Check condition 2: Is the current element even, and has its half been registered?
            if (num % 2 == 0 && visitedElements.count(num / 2) > 0) {
                return true;
            }

            // Register the current value into the lookup table context
            visitedElements.insert(num);
        }

        return false; // No valid matching double relation exists within the array
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements in the input array.
 *                  The algorithm reads each array element exactly once. Hash table lookup 
 *                  and insertion operations execute in average O(1) time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The lookup hash table structure scales dynamically, 
 *                   storing up to N elements in the absolute worst-case scenario.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting hash set search integration lookup sweep..." << endl;
    
    cout << boolalpha;
    bool outcomeResult = solver.checkIfExist(arr);

    cout << "\nDouble Existence Relation Outcome: " << outcomeResult << endl;
    if (outcomeResult) {
        cout << "Success: There exists an element N that is exactly twice the value of another element." << endl;
    } else {
        cout << "Fail: No element pair satisfies the N = 2 * M numerical constraint." << endl;
    }

    return 0;
}