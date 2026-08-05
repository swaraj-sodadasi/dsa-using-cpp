#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * SORTING AND GREEDY PAIR MINIMIZATION ENGINE
     * -------------------------------------------
     * The objective is to group 2n integers into n pairs (a1, b1), (a2, b2), ..., (an, bn) 
     * such that the sum of min(ai, bi) for all i is maximized.
     *
     * Algorithmic & Greedy Strategy:
     * - When we pair two numbers together, the larger number in the pair is completely discarded 
     *   by the min() operation, and the smaller number is added to our sum.
     * - To maximize the overall sum, we must minimize the "waste" of large numbers. For example, 
     *   if we pair the largest element with the smallest element, the largest element is wasted.
     * - The optimal greedy strategy is to pair numbers that are closest to each other on the number line. 
     *   This ensures that the penalty of dropping the larger element in each pair is minimized.
     *
     * Execution Protocol:
     * - Step 1: Sort the array in ascending order. This places optimal pair partners adjacent to one another.
     * - Step 2: Iterate through the sorted array, stepping by 2 indices at each hop (i = 0, 2, 4, ...). 
     *   In every adjacent pair (arr[i], arr[i+1]), arr[i] is guaranteed to be the minimum because the array 
     *   is sorted. 
     * - Step 3: Accumulate arr[i] into our total sum variable and return it.
     */
    int arrayPairSum(vector<int>& nums) {
        // Step 1: Sort the collection to align optimal greedy pairs next to each other
        sort(nums.begin(), nums.end());

        int maximizedSumResult = 0;
        size_t length = nums.size();

        // Step 2 & 3: Accumulate the lower boundary value of every adjacent pair segment
        for (size_t i = 0; i < length; i += 2) {
            maximizedSumResult += nums[i];
        }

        return maximizedSumResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements inside the collection array. 
 *                  Sorting the sequence dictates the time bottleneck at O(N log N). The subsequent 
 *                  greedy accumulation pass runs in linear O(N) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The sorting step operates completely in-place on the input 
 *                   container, requiring no additional heap allocations or dynamic structures.
 */
int main() {
    int n;
    cout << "Enter the count of pairs to form (N, meaning the array will have 2*N elements): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Pair count must be a positive integer." << endl;
        return 1;
    }

    int totalElements = 2 * n;
    vector<int> nums(static_cast<size_t>(totalElements));
    cout << "Enter " << totalElements << " integer array elements separated by spaces:\n";
    for (int i = 0; i < totalElements; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting sorting and greedy pairing element accumulations..." << endl;
    int optimalSum = solver.arrayPairSum(nums);

    cout << "Calculated maximized sum of pair minimums: " << optimalSum << endl;

    return 0;
}