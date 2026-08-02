#include <iostream>
#include <vector>

using namespace std;

class Solution {
private:
    /*
     * FREQUENCY EVALUATION HELPER
     * ---------------------------
     * Helper method to count how many elements in the array are greater than 
     * or equal to a target candidate value `val`.
     */
    int countElementsGreaterThanOrEqual(const vector<int>& nums, int val) {
        int count = 0;
        for (int num : nums) {
            if (num >= val) {
                count++;
            }
        }
        return count;
    }

public:
    /*
     * BINARY SEARCH ON ANSWER RANGE ENGINE
     * ------------------------------------
     * The objective is to determine if an array is "special". An array is special 
     * if there exists a value `x` such that there are exactly `x` elements in the 
     * array that are greater than or equal to `x`. Note that `x` does not have to 
     * be an element of the array.
     *
     * Algorithmic & Search Strategy:
     * - The number of elements that can satisfy the condition is bounded by the size 
     *   of the array. Therefore, the search space for the answer `x` is strictly 
     *   bounded within the monotonic numerical range `[1, N]`, where N is `nums.size()`.
     * - Because this search space is sorted, we can use binary search to test candidate 
     *   values for `x` instead of doing a linear scan from 1 to N.
     *
     * Decision Criteria Protocol:
     * - We initialize the search boundaries: `left = 1` and `right = N`.
     * - Calculate the midpoint candidate securely: `mid = left + (right - left) / 2`.
     * - Compute `validCount`, which is the number of elements `>= mid`.
     * - Evaluation Steps:
     *   - If `validCount == mid`, we have found our exact target condition. Return `mid` immediately.
     *   - If `validCount > mid`, it means we have too many elements satisfying the criteria, 
     *     so our candidate `mid` is too small. We shift our lower search boundary: `left = mid + 1`.
     *   - If `validCount < mid`, it means there are not enough elements to meet or exceed the 
     *     current value of `mid`, so our candidate is too large. We contract our upper search 
     *     boundary: `right = mid - 1`.
     * - If the loop terminates without finding a match, the array cannot be special, and we return -1.
     */
    int specialArray(vector<int>& nums) {
        int left = 1;
        int right = static_cast<int>(nums.size());

        // Perform binary search across the potential answer range
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int validCount = countElementsGreaterThanOrEqual(nums, mid);

            if (validCount == mid) {
                return mid; // Found the precise value matching the structural constraint
            } else if (validCount > mid) {
                // Too many elements fit the criteria; increase the lower bound threshold
                left = mid + 1;
            } else {
                // Not enough elements fit the criteria; lower the upper bound target range
                right = mid - 1;
            }
        }

        return -1; // No value of x satisfies the special criteria conditions
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements inside the array. 
 *                  The binary search space spans a range of size N, requiring log(N) iterations. 
 *                  Inside each step, we perform a linear scan of size N to count the matching elements.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The optimization strategy operates entirely in-place 
 *                   using simple scalar tracking flags, requiring zero extra dynamic memory allocations.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (non-negative integers) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Elements must be non-negative integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting binary search on answer range sweep..." << endl;
    int result = solver.specialArray(nums);

    if (result != -1) {
        cout << "The array is special! The unique matching value X is: " << result << endl;
    } else {
        cout << "Result: -1 (The array does not satisfy the special parameter constraints)" << endl;
    }

    return 0;
}