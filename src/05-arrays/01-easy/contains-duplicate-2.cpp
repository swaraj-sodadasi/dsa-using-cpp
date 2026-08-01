#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * SLIDING WINDOW HASH SET OPTIMIZATION ENGINE
     * -------------------------------------------
     * The problem asks us to find if there are two distinct indices 'i' and 'j' in the array
     * such that nums[i] == nums[j] and the absolute difference between 'i' and 'j' is at most 'k'.
     *
     * Architectural Tracking Mechanics:
     * - A naive solution requires checking all pairs, resulting in a slow O(N * k) time footprint.
     * - We optimize this by using a sliding window combined with an unordered hash set. The hash set
     *   tracks the numbers present inside our active index window.
     * - The width of this sliding window is bounded by the constraint 'k'. At any given index position 'i',
     *   the hash set stores exactly the elements from `nums[i - k]` through `nums[i - 1]`.
     *
     * State Window Evaluation Sequence:
     * - We perform a single forward sweep through the array using an index tracker `i`.
     * - If the window grows larger than size 'k' (when `i > k`), the element that fell outside the 
     *   valid maximum distance window (`nums[i - k - 1]`) is evicted from the hash set.
     * - We then query the hash set to see if the current value `nums[i]` already exists within the active window.
     *   - If it exists, we have found a duplicate pair within distance 'k' -> Return true immediately.
     *   - If it does not exist, we insert `nums[i]` into the hash set and move the window forward.
     */
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_set<int> windowSet;
        size_t windowLimit = static_cast<size_t>(k);

        for (size_t i = 0; i < nums.size(); ++i) {
            // STEP 1: EVICTION RULE
            // If the distance between the current index and the oldest element in the window
            // exceeds k, remove that trailing element from the set.
            if (i > windowLimit) {
                windowSet.erase(nums[i - windowLimit - 1]);
            }

            // STEP 2: LOOKUP AND QUERY
            // Check if the current element is already present in the active sliding window.
            if (windowSet.count(nums[i])) {
                return true; // Match identified within the legal constraint boundaries
            }

            // STEP 3: INSERTION
            // Add the current element to the set to include it in the active window for the next step.
            windowSet.insert(nums[i]);
        }

        return false;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of items in the array.
 *                  The logic performs a single linear sweep over the input data. Hash set operations 
 *                  (lookup, insertion, eviction) run in average constant O(1) time.
 *
 * SPACE COMPLEXITY: O(min(N, k)) Auxiliary - The hash set allocation footprint scales linearly 
 *                   with the size of the window, storing at most k + 1 unique integers at any given time.
 */
int main() {
    int n, k;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the maximum allowed index distance constraint (k): ";
    if (!(cin >> k) || k < 0) {
        cout << "Invalid distance window parameter constraint." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting sliding window hash set optimization sweep..." << endl;
    
    if (solver.containsNearbyDuplicate(nums, k)) {
        cout << "Result: True! Found duplicate values within distance " << k << "." << endl;
    } else {
        cout << "Result: False! No matching duplicates found within distance " << k << "." << endl;
    }

    return 0;
}