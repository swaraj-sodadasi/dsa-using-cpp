#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MONOTONIC HASH LOOKUP STRATEGY FOR SUBSET ARYS
     * -------------------------------------------------------------------------------------
     * The objective is to find the next greater element for each item of nums1 in nums2,
     * where nums1 is a subset of nums2.
     * 
     * 1. THE RIGHT-TO-LEFT MONOTONIC SCAN:
     *    Instead of brute-forcing pairs across arrays which yields O(N1 * N2), we precalculate
     *    the answers for all items inside the container nums2. We scan nums2 from right to left
     *    maintaining a strictly decreasing stack of elements.
     * 
     * 2. THE PIPELINE LIQUIDATION MATRIX:
     *    For each element nums2[i], we pop elements from the stack that are smaller than or
     *    equal to it. If the stack is not empty after cleansing, the top of the stack is the
     *    next greater element. If the stack is empty, no such element exists (-1).
     * 
     * 3. THE INSTANT HASH INDEX DIRECTORY:
     *    We store the mapping of {element -> next_greater} inside an unordered_map.
     *    Once the linear pass over nums2 finishes, we build the final output vector for nums1
     *    by looking up each element in the hash map in constant O(1) time.
     */
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> nextGreaterMap;
        stack<int> s;
        int n2 = static_cast<int>(nums2.size());

        // Process nums2 from right to left to populate the next greater element matrix
        for (int i = n2 - 1; i >= 0; --i) {
            int currentElement = nums2[static_cast<size_t>(i)];

            // Clear stack items that are less than or equal to the current element
            while (!s.empty() && s.top() <= currentElement) {
                s.pop();
            }

            // Map the current item to its next greater counterpart
            nextGreaterMap[currentElement] = s.empty() ? -1 : s.top();

            // Push the current item to serve as a target candidate for previous elements
            s.push(currentElement);
        }

        // Build the target result matrix by indexing items via hash lookups
        vector<int> result(nums1.size());
        for (size_t i = 0; i < nums1.size(); ++i) {
            result[i] = nextGreaterMap[nums1[i]];
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N1 + N2) - Where N1 and N2 are lengths of nums1 and nums2 respectively.
 *                  Every item in nums2 is pushed and popped at most once. The results for nums1
 *                  are fetched in constant time.
 *
 * SPACE COMPLEXITY: O(N2) Auxiliary - The unordered hash map and internal stack structure store
 *                   at most N2 elements during execution workflows.
 */
int main() {
    cout << "=== Monotonic Stack Subset Hash Lookup Engine ===\n";
    
    int n1Count, n2Count;
    cout << "Enter size of subset array (nums1): ";
    if (!(cin >> n1Count) || n1Count < 0) return 1;
    vector<int> nums1(static_cast<size_t>(n1Count));
    cout << "Enter elements for nums1:\n";
    for (int i = 0; i < n1Count; ++i) cin >> nums1[static_cast<size_t>(i)];

    cout << "Enter size of main array (nums2): ";
    if (!(cin >> n2Count) || n2Count < 0) return 1;
    vector<int> nums2(static_cast<size_t>(n2Count));
    cout << "Enter elements for nums2:\n";
    for (int i = 0; i < n2Count; ++i) cin >> nums2[static_cast<size_t>(i)];

    Solution solver;
    cout << "\nExecuting monotonic loops and assembling hash directory maps..." << endl;
    vector<int> outcome = solver.nextGreaterElement(nums1, nums2);

    cout << "Next Greater Elements Result: [ ";
    for (int val : outcome) cout << val << " ";
    cout << "]\n";

    return 0;
}