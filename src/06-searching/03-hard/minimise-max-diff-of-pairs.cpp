#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * GREEDY PAIRING VERIFICATION HELPER
     * ----------------------------------
     * Evaluates if it is possible to form at least `p` unique pairs from `nums` 
     * such that the absolute difference of each pair is less than or equal to `maxDiff`.
     * 
     * Greedy Pairing Mechanics:
     * - Since `nums` is sorted, the closest potential matches for any element are its immediate 
     *   neighbors. 
     * - We iterate through the array using a single-pass greedy validation sequence. 
     * - If `nums[i] - nums[i - 1] <= maxDiff`, it is always structurally optimal to form a pair 
     *   between `nums[i]` and `nums[i - 1]` immediately.
     * - When a pair is formed, we advance our pointer by 2 (`i += 2`) because each element can 
     *   only participate in one pair. If the constraint fails, we advance by 1 (`i++`) 
     *   to evaluate the next adjacent window segment.
     */
    bool isValidPairingThreshold(const vector<int>& nums, int p, int maxDiff) {
        int pairCount = 0;
        int n = static_cast<int>(nums.size());

        for (int i = 1; i < n; ++i) {
            if (nums[static_cast<size_t>(i)] - nums[static_cast<size_t>(i - 1)] <= maxDiff) {
                pairCount++;
                
                // Early pruning: return true as soon as the target number of pairs is met
                if (pairCount >= p) {
                    return true;
                }
                
                // Skip the next index since elements cannot be reused across multiple pairs
                i++; 
            }
        }
        return pairCount >= p;
    }

public:
    /*
     * GREEDY PAIRING VERIFICATION + BINARY SEARCH ENGINE
     * --------------------------------------------------
     * The objective is to select `p` pairs of elements from an integer array such that 
     * the maximum absolute difference among the selected pairs is minimized.
     *
     * Mathematical & Algorithmic Strategy:
     * - The search space represents the possible values for the maximum pair difference.
     * - This space exhibits monotonic properties: if we can form `p` pairs with a maximum difference 
     *   of `D`, then any maximum difference greater than `D` is also valid. If it's impossible 
     *   at `D`, it's guaranteed to be impossible for all values smaller than `D`.
     * - This monotonic truth enables a binary search on the answer range to find the exact minimax solution.
     *
     * Boundary Initialization Protocol:
     * - Lower Bound (`left`): 0 (The absolute minimal possible difference when identical elements form a pair).
     * - Upper Bound (`right`): The difference between the largest and smallest element in the sorted array.
     */
    int minimizeMax(vector<int>& nums, int p) {
        // Base case edge handling: if zero pairs are requested, the structural constraint defaults to 0.
        if (p == 0) return 0;

        // Step 1: Sort the collection to bring elements with minimal internal differences adjacent to each other.
        sort(nums.begin(), nums.end());

        int left = 0;
        int right = nums.back() - nums.front(); // Maximum absolute boundary difference span
        int optimizedMinimaxResult = right;

        // Step 2: Execute standard binary search across the monotonic target difference interval
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (isValidPairingThreshold(nums, p, mid)) {
                optimizedMinimaxResult = mid; // Register the valid difference candidate
                right = mid - 1;              // Compress search frame down to check for a lower constraint
            } else {
                left = mid + 1;               // Value is too restrictive; scale lower bound upwards
            }
        }

        return optimizedMinimaxResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N + N log(MaxDiff)) - Where N represents the total elements in nums. 
 *                  Sorting the array at the start takes O(N log N) time. The subsequent 
 *                  binary search operates across a domain range of size MaxDiff, executing a linear 
 *                  O(N) greedy sweep check during each iteration step.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The threshold pairing evaluation logic performs calculations completely 
 *                   in-place using basic index markers, using no dynamic data structures.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int p;
    cout << "Enter the target count of pairs to form (P must satisfy 2 * P <= N): ";
    if (!(cin >> p) || p < 0 || 2 * p > n) {
        cout << "Constraint Error: Invalid number of pairs requested." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting greedy pairing verification with binary search answer updates..." << endl;
    int optimalDifferenceResult = solver.minimizeMax(nums, p);

    cout << "Calculated minimized maximum absolute pair difference: " << optimalDifferenceResult << endl;

    return 0;
}