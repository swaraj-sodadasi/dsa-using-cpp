#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

class Solution {
private:
    /*
     * GREEDY MONOTONIC SUBSEQUENCE SELECTION ENGINE
     * ---------------------------------------------
     * Extracts the lexicographically largest subsequence of a given length `k` 
     * from a single array while preserving the original relative order.
     * Uses a monotonic stack strategy implemented via a vector.
     */
    vector<int> maxSubarray(const vector<int>& nums, int k) {
        vector<int> result;
        int n = static_cast<int>(nums.size());
        int dropElementsCount = n - k; // Total elements we are allowed to discard

        for (int num : nums) {
            // Maintain a decreasing stack property: drop smaller items if we have remaining safety padding
            while (!result.empty() && result.back() < num && dropElementsCount > 0) {
                result.pop_back();
                dropElementsCount--;
            }
            result.push_back(num);
        }
        
        // Trim the vector if it collected more than k elements due to non-decreasing sequences
        result.resize(static_cast<size_t>(k));
        return result;
    }

    /*
     * LEXICOGRAPHICAL VECTOR COMPARISON GREEDY ENGINE
     * -----------------------------------------------
     * Evaluates which of the two remaining array segments forms a larger prefix 
     * configuration from their current starting pointers.
     */
    bool isGreater(const vector<int>& nums1, size_t i, const vector<int>& nums2, size_t j) {
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] > nums2[j]) return true;
            if (nums1[i] < nums2[j]) return false;
            i++;
            j++;
        }
        // If we reach the end of one vector, the one with remaining elements is larger
        return i < nums1.size();
    }

    /*
     * MONOTONIC INTERLEAVING MERGE ENGINE
     * -----------------------------------
     * Combines two single monotonic subsequences into the single largest 
     * lexicographical unified vector array of length `k`.
     */
    vector<int> merge(const vector<int>& nums1, const vector<int>& nums2, int k) {
        vector<int> mergedResult;
        mergedResult.reserve(static_cast<size_t>(k));
        size_t i = 0, j = 0;

        for (int step = 0; step < k; ++step) {
            // Greedily pick the element from the sequence that offers the greater lookahead layout
            if (isGreater(nums1, i, nums2, j)) {
                mergedResult.push_back(nums1[i++]);
            } else {
                mergedResult.push_back(nums2[j++]);
            }
        }
        return mergedResult;
    }

public:
    /*
     * GREEDY MONOTONIC SUBSEQUENCE SELECTION AND INTERLEAVING
     * -------------------------------------------------------
     * The objective is to create the maximum number of length k from digits of two arrays 
     * nums1 and nums2, preserving the relative order of digits within the same array.
     * 
     * Dynamic Partitioning Search Scheme:
     * - We distribute the total target length `k` across both arrays such that `k1 + k2 == k`.
     * - The range for `k1` (elements picked from `nums1`) is bounded by:
     *   `max(0, k - len(nums2)) <= k1 <= min(k, len(nums1))`
     * - For each valid `k1` partition tier:
     *   1. Extract the max monotonic subsequence of length `k1` from `nums1`.
     *   2. Extract the max monotonic subsequence of length `k2` from `nums2`.
     *   3. Merge the two subsequences lexicographically to build a candidate configuration.
     *   4. Track and store the absolute maximum layout found across all iterations.
     */
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<int> maxCombinationResult;
        int len1 = static_cast<int>(nums1.size());
        int len2 = static_cast<int>(nums2.size());

        int startK1 = max(0, k - len2);
        int endK1 = min(k, len1);

        // Scan across all valid combinations of length allocation partitions
        for (int k1 = startK1; k1 <= endK1; ++k1) {
            int k2 = k - k1;
            
            vector<int> sub1 = maxSubarray(nums1, k1);
            vector<int> sub2 = maxSubarray(nums2, k2);
            vector<int> currentCandidate = merge(sub1, sub2, k);

            // Retain the lexicographically largest sequence discovered
            if (maxCombinationResult.empty() || currentCandidate > maxCombinationResult) {
                maxCombinationResult = std::move(currentCandidate);
            }
        }

        return maxCombinationResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(k * (N + M + k^2)) - Where N and M represent the sizes of nums1 and nums2. 
 *                  There are at most k partition iteration tiers. In each tier, selecting the 
 *                  subsequence takes O(N + M) time, while the merge process takes O(k^2) due to 
 *                  the internal vector lookahead comparison loop.
 *
 * SPACE COMPLEXITY: O(k) Auxiliary - The solution constructs intermediate structural candidate sub-vectors 
 *                   and merged buffers proportional to the targeted constraint size k.
 */
int main() {
    string inputLine1, inputLine2;
    
    cout << "Enter the first array of single-digit integers (0-9) separated by spaces:\n";
    if (!getline(cin, inputLine1)) return 1;
    stringstream ss1(inputLine1);
    int digit;
    vector<int> nums1;
    while (ss1 >> digit) nums1.push_back(digit);

    cout << "Enter the second array of single-digit integers (0-9) separated by spaces:\n";
    if (!getline(cin, inputLine2)) return 1;
    stringstream ss2(inputLine2);
    vector<int> nums2;
    while (ss2 >> digit) nums2.push_back(digit);

    int k;
    cout << "Enter the targeted combined output sequence length constraint (k): ";
    if (!(cin >> k) || k <= 0 || k > static_cast<int>(nums1.size() + nums2.size())) {
        cout << "Invalid window selection constraint boundaries." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting greedy monotonic subsequence selection and interleaving search..." << endl;
    vector<int> result = solver.maxNumber(nums1, nums2, k);

    cout << "\nMaximum possible combined configuration constructed:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}