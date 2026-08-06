#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BASIC HASH SET MEMBERSHIP VERIFICATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine if any value appears at least twice in an integer array.
     * Return true if any value is duplicated, and false if every element is distinct.
     * 
     * 1. HASH TABLE MEMBERSHIP FOR LINEAR SPEED:
     *    A naive nested loop approach runs in O(N^2) time. Sorting the array takes O(N log N) 
     *    time but mutates the input. Instead, we use an `unordered_set` hash container to track 
     *    seen elements in a single pass.
     * 
     * 2. EARLY-TERMINATION SCAN:
     *    We loop through the array `nums` from left to right. For each element, we query the set:
     *      - If found: A duplicate exists. We immediately return true, terminating early.
     *      - If not found: We insert the current number into the set and proceed to the next element.
     * 
     * 3. INVARIANT PROTECTION:
     *    If the loop finishes without triggering a membership match, every element is distinct, 
     *    so we return false.
     */
    bool containsDuplicate(vector<int>& nums) {
        // Hash set container to store unique numbers seen so far
        unordered_set<int> seenNumbers;

        for (int num : nums) {
            // Step 2: Early-termination query check
            if (seenNumbers.find(num) != seenNumbers.end()) {
                return true; 
            }
            // Register the element in the tracking table
            seenNumbers.insert(num);
        }

        return false;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The loop parses each element at most once, performing set insertion and 
 *                  search queries in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - In the worst-case scenario where all elements are distinct, 
 *                   the hash set stores up to N records.
 */
int main() {
    int count;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> count) || count <= 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(count));
    cout << "Enter the array elements sequentially:\n";
    for (int i = 0; i < count; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting single-pass hash set duplicate verification..." << endl;
    bool hasDuplicate = solver.containsDuplicate(nums);

    if (hasDuplicate) {
        cout << "Outcome: Duplicate element found in the array. (Result = true)" << endl;
    } else {
        cout << "Outcome: All elements in the array are unique. (Result = false)" << endl;
    }

    return 0;
}