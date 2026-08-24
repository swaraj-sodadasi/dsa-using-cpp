#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MONOTONIC STACK RANGE BOUNDARIES CONTRIBUTION FACTORIZATION
     * -------------------------------------------------------------------------------------
     * The objective is to find the sum of min(b) for all subarrays b of an array A.
     * 
     * 1. THE MATHEMATICAL CONTRIBUTION PRINCIPLE:
     *    Instead of computing the minimum for every subarray explicitly, we determine for 
     *    each element arr[i] how many subarrays have arr[i] as their strict minimum.
     *    Let:
     *      - PLE[i] = Index of the Previous Less Element to the left of i.
     *      - NLE[i] = Index of the Next Less Element to the right of i.
     *    The number of valid subarrays containing arr[i] as the minimum is:
     *      Count = (i - PLE[i]) * (NLE[i] - i)
     *    Total contribution of arr[i] = arr[i] * Count.
     * 
     * 2. THE STRICT VS. NON-STRICT DUPLICATE GUARD:
     *    To avoid double-counting subarrays when identical duplicate values exist in the array, 
     *    we handle one boundary strictly and the other non-strictly. In this pipeline, we search 
     *    for strictly less elements on the left, and less-than-or-equal elements on the right.
     * 
     * 3. SINGLE-PASS INTEGRATED RESOLUTION:
     *    We maintain a monotonic increasing stack tracking indices. We iterate from 0 to N.
     *    A virtual terminal pad element (value 0) flushes the remaining items out of the stack 
     *    at index `i == n`, completing calculation bounds dynamically.
     */
    int sumSubarrayMins(const vector<int>& arr) {
        int n = static_cast<int>(arr.size());
        stack<int> s;
        long long totalSum = 0;
        const long long MODULO_BASE = 1000000007;

        for (int i = 0; i <= n; ++i) {
            // Virtual element 0 at index n flushes out all remaining nodes in the stack
            int currentVal = (i == n) ? 0 : arr[static_cast<size_t>(i)];

            // Maintain monotonic increasing property by liquidating larger entries
            while (!s.empty() && arr[static_cast<size_t>(s.top())] > currentVal) {
                int midIdx = s.top();
                s.pop();

                long long leftBoundaryIdx = s.empty() ? -1 : s.top();
                long long rightBoundaryIdx = i;

                // Calculate the left and right continuous segment spans
                long long leftSpanCount = midIdx - leftBoundaryIdx;
                long long rightSpanCount = rightBoundaryIdx - midIdx;

                long long subarrayPermutations = (leftSpanCount * rightSpanCount) % MODULO_BASE;
                long long segmentContribution = (arr[static_cast<size_t>(midIdx)] * subarrayPermutations) % MODULO_BASE;

                totalSum = (totalSum + segmentContribution) % MODULO_BASE;
            }
            s.push(i);
        }

        return static_cast<int>(totalSum);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements in the source array. 
 *                  Every individual array index coordinate is pushed and popped at most once.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal monotonic stack tracks up to N records under 
 *                   monotonically increasing value allocations (e.g., [10, 20, 30, 40]).
 */
int main() {
    cout << "=== Sum of Subarray Minimums Range Contribution Engine ===\n";
    cout << "Enter the total size of the integer array: ";
    int length;
    if (!(cin >> length) || length < 0) return 1;

    vector<int> numbersList(static_cast<size_t>(length));
    if (length > 0) {
        cout << "Enter the raw integer components sequentially:\n";
        for (int i = 0; i < length; ++i) {
            cin >> numbersList[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nAnalyzing range boundaries and factoring subsegment permutations..." << endl;
    int calculatedTotalSum = solver.sumSubarrayMins(numbersList);

    cout << "Sum of All Structural Subarray Minimums: [" << calculatedTotalSum << "]\n";

    return 0;
}