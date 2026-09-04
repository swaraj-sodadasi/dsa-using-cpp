#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * 1. GREEDY MONOTONIC STACK SUB-ARRAY SELECTION:
     *    Extracts the lexicographically largest sub-array of length k from nums.
     */
    vector<int> maxArray(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        vector<int> stack;
        stack.reserve(static_cast<size_t>(k));
        int drop = n - k;

        for (int num : nums) {
            while (drop > 0 && !stack.empty() && stack.back() < num) {
                stack.pop_back();
                drop--;
            }
            stack.push_back(num);
        }

        stack.resize(static_cast<size_t>(k));
        return stack;
    }

    /*
     * 2. GREEDY LEXICOGRAPHICAL COMPARISON:
     *    Compares sub-arrays starting at i and j to determine which produces 
     *    the larger sequence from the current position.
     */
    bool greater(const vector<int>& nums1, int i, const vector<int>& nums2, int j) {
        int n1 = static_cast<int>(nums1.size());
        int n2 = static_cast<int>(nums2.size());

        while (i < n1 && j < n2) {
            if (nums1[static_cast<size_t>(i)] != nums2[static_cast<size_t>(j)]) {
                return nums1[static_cast<size_t>(i)] > nums2[static_cast<size_t>(j)];
            }
            i++;
            j++;
        }
        return (n1 - i) > (n2 - j);
    }

    /*
     * 3. SUB-LIST MERGING:
     *    Merges two sub-arrays into a single maximum possible sequence of length k.
     */
    vector<int> merge(const vector<int>& nums1, const vector<int>& nums2, int k) {
        vector<int> result(static_cast<size_t>(k));
        int i = 0, j = 0;

        for (int r = 0; r < k; ++r) {
            if (greater(nums1, i, nums2, j)) {
                result[static_cast<size_t>(r)] = nums1[static_cast<size_t>(i++)];
            } else {
                result[static_cast<size_t>(r)] = nums2[static_cast<size_t>(j++)];
            }
        }
        return result;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CREATE MAXIMUM NUMBER (GREEDY MONOTONIC STACK & MERGE)
     * -------------------------------------------------------------------------------------
     * The objective is to create the maximum number of length k from two arrays nums1 and nums2,
     * maintaining relative order within each array.
     * 
     * We iterate through all valid splits i and j (where i + j = k), generating the max sequence 
     * from nums1 of length i, and from nums2 of length j, and merging them lexicographically.
     */
    vector<int> maxNumber(const vector<int>& nums1, const vector<int>& nums2, int k) {
        int m = static_cast<int>(nums1.size());
        int n = static_cast<int>(nums2.size());
        vector<int> maxResult;

        int start = max(0, k - n);
        int end = min(k, m);

        for (int i = start; i <= end; ++i) {
            vector<int> seq1 = maxArray(nums1, i);
            vector<int> seq2 = maxArray(nums2, k - i);
            vector<int> candidate = merge(seq1, seq2, k);

            if (maxResult.empty() || greater(candidate, 0, maxResult, 0)) {
                maxResult = candidate;
            }
        }

        return maxResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(K * (M + N)^2) - Where M, N are lengths of nums1, nums2, and K is output size.
 *                  Generating sub-arrays takes O(M + N), merging takes O(K * (M + N)).
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Buffer space allocated for merged and stack arrays.
 */
int main() {
    cout << "=== Create Maximum Number Engine ===\n";
    cout << "Enter size of first array nums1: ";
    int m;
    if (!(cin >> m) || m < 0) return 1;

    vector<int> nums1(static_cast<size_t>(m));
    if (m > 0) {
        cout << "Enter elements for nums1:\n";
        for (int i = 0; i < m; ++i) {
            cin >> nums1[static_cast<size_t>(i)];
        }
    }

    cout << "Enter size of second array nums2: ";
    int n;
    if (!(cin >> n) || n < 0) return 1;

    vector<int> nums2(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter elements for nums2:\n";
        for (int i = 0; i < n; ++i) {
            cin >> nums2[static_cast<size_t>(i)];
        }
    }

    cout << "Enter target length k: ";
    int k;
    if (!(cin >> k) || k <= 0 || k > m + n) return 1;

    Solution solver;
    vector<int> result = solver.maxNumber(nums1, nums2, k);

    cout << "Maximum sequence created: [ ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << "]\n";

    return 0;
}