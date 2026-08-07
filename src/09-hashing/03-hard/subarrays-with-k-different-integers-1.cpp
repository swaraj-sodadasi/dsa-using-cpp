#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
private:
    /*
     * Private helper function to calculate the total number of subarrays containing 
     * AT MOST 'k' distinct integers using a standard sliding window approach.
     * 
     * A sliding window naturally expands by adding elements on the right. For a valid window 
     * stretching from index `left` to `right`, the total number of subarrays ending at 
     * `right` is exactly equal to the window's current width: `right - left + 1`.
     */
    int subarraysWithAtMostK(const vector<int>& nums, int k) {
        if (k <= 0) return 0;

        // Map stores: {Element_Value -> Active_Frequency_Count}
        unordered_map<int, int> frequencyMap;
        int left = 0;
        int totalSubarraysCount = 0;
        int distinctElementsCount = 0;

        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            // Include the incoming element from the right boundary
            if (frequencyMap[nums[static_cast<size_t>(right)]] == 0) {
                distinctElementsCount++;
            }
            frequencyMap[nums[static_cast<size_t>(right)]]++;

            // Shrink the left boundary if the number of distinct elements exceeds k
            while (distinctElementsCount > k) {
                frequencyMap[nums[static_cast<size_t>(left)]]--;
                if (frequencyMap[nums[static_cast<size_t>(left)]] == 0) {
                    distinctElementsCount--;
                }
                left++;
            }

            // Accumulate subarrays ending at 'right' within the valid window frame
            totalSubarraysCount += (right - left + 1);
        }

        return totalSubarraysCount;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SLIDING WINDOW FREQUENCY HASH MAP DIFFERENCE (AtMost(K) - AtMost(K-1))
     * -------------------------------------------------------------------------------------
     * The objective is to find the total number of continuous subarrays containing 
     * EXACTLY 'k' distinct integers.
     * 
     * 1. THE EXACTLY-K COMBINATORIAL PROBLEM:
     *    Directly counting subarrays with *exactly* k distinct elements using a sliding window 
     *    is highly complex. As the window expands, the number of distinct elements can fluctuate, 
     *    making it difficult to determine when to contract the left pointer without missing valid windows.
     * 
     * 2. THE SUBTRACTION REDUCTION FRAMEWORK:
     *    We can reframe the problem using a simple combinatorial identity:
     *    `Exact(K) = AtMost(K) - AtMost(K - 1)`
     *    
     *    - `AtMost(K)` calculates the total count of subarrays containing anywhere from 1 up to K 
     *      distinct elements.
     *    - `AtMost(K - 1)` calculates the total count of subarrays containing anywhere from 1 up to K-1 
     *      distinct elements.
     *    
     *    Subtracting these two results eliminates all subarrays with fewer than K distinct elements, 
     *    leaving exactly the count of subarrays with *exactly* K distinct elements.
     * 
     * 3. TWO INDEPENDENT LINEAR SWEEPS:
     *    By running our sliding window helper twice—once with parameter `k` and once with `k - 1`—we 
     *    solve the problem in optimal linear time with straightforward window conditions.
     */
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        return subarraysWithAtMostK(nums, k) - subarraysWithAtMostK(nums, k - 1);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The algorithm calls the helper function twice. Each call processes the array 
 *                  in a single pass, where the `left` and `right` pointers traverse the elements 
 *                  at most once. Hash map lookups and modifications complete in amortized O(1) time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal frequency maps allocate space proportional 
 *                   to the number of distinct elements, storing up to N entries in the worst case.
 */
int main() {
    int count, k;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> count) || count <= 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(count));
    cout << "Enter the array elements sequentially (Positive integers):\n";
    for (int i = 0; i < count; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Input values must be positive integers." << endl;
            return 1;
        }
    }

    cout << "Enter the exact distinct integers target threshold (k): ";
    if (!(cin >> k) || k <= 0 || k > count) {
        cout << "Invalid k configuration value." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting AtMost(K) - AtMost(K-1) linear window partitioning..." << endl;
    int perfectMatchesCount = solver.subarraysWithKDistinct(nums, k);

    cout << "Total subarrays discovered containing exactly " << k << " distinct integers: " << perfectMatchesCount << endl;

    return 0;
}