#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * BINARY SEARCH ON FRACTIONAL FLOAT RANGE WITH TWO-POINTER COUNTING ENGINE
     * ------------------------------------------------------------------------
     * The objective is to find the k-th smallest fraction form p/q such that p and q 
     * are elements of a strictly ascending sorted array of prime numbers, where p < q.
     *
     * Algorithmic & Mathematical Strategy:
     * - A brute force generation of all pairs runs in O(N^2 log(N^2)) time. 
     *   We optimize this to O(N log(MaxVal)) by operating a binary search on the fractional value 
     *   range `[0.0, 1.0]`.
     * - Because the fraction space is monotonic (increasing the numerator or decreasing the denominator 
     *   changes values predictably), we can count fractions less than or equal to a value `mid` in 
     *   linear O(N) time using a sliding window/two-pointer scan.
     *
     * Two-Pointer Counting & Target Mapping Scheme:
     * - Initialize `count = 0`, a numerator index tracker `i = 0`, and tracking values for the 
     *   best fraction encountered so far (`bestP = 0`, `bestQ = 1`).
     * - For each denominator pointer `j` from 1 to N-1:
     *   - Advance the numerator pointer `i` forward while the value condition matches: `arr[i] / arr[j] <= mid`.
     *   - Since `arr` is sorted, if `arr[i]/arr[j] <= mid`, then all numerators from `0` to `i-1` 
     *     paired with `arr[j]` are also less than or equal to `mid`. We add this to our total: `count += i`.
     *   - While doing this, we also track the absolute maximum fraction (`arr[i-1]/arr[j]`) that stayed 
     *     within the `mid` threshold. This keeps track of the boundary value if we hit exactly `k`.
     * - If the total matching fraction count equals `k`, the tracked boundary fraction `[bestP, bestQ]` 
     *   is our exact answer.
     * - If `count < k`, our guess `mid` is too small, so we shift our lower boundary: `left = mid`.
     * - If `count > k`, our guess `mid` is too large, so we contract our upper boundary: `right = mid`.
     */
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        int n = static_cast<int>(arr.size());
        double left = 0.0;
        double right = 1.0;

        // Perform real-number fractional binary search space reductions
        while (left < right) {
            double mid = left + (right - left) / 2.0;

            int count = 0;
            int i = 0;
            int bestNumeratorIdx = 0;
            int bestDenominatorIdx = 0;
            double maxFractionSeen = 0.0;

            // Compute structural fraction frequency distribution matrix across the array using two-pointers
            for (int j = 1; j < n; ++j) {
                while (i < j && arr[static_cast<size_t>(i)] <= mid * arr[static_cast<size_t>(j)]) {
                    i++;
                }
                
                // All numerators from index 0 to i-1 form fractions <= mid with arr[j]
                count += i;

                // Track the largest fraction that is less than or equal to mid
                if (i > 0) {
                    double currentFraction = static_cast<double>(arr[static_cast<size_t>(i - 1)]) / arr[static_cast<size_t>(j)];
                    if (currentFraction > maxFractionSeen) {
                        maxFractionSeen = currentFraction;
                        bestNumeratorIdx = i - 1;
                        bestDenominatorIdx = j;
                    }
                }
            }

            if (count == k) {
                return {arr[static_cast<size_t>(bestNumeratorIdx)], arr[static_cast<size_t>(bestDenominatorIdx)]};
            } else if (count < k) {
                left = mid;  // Target fraction is larger; scale lower bound upward
            } else {
                right = mid; // Target fraction is smaller; compress upper bound downward
            }
        }

        return {};
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * log(1 / Precision)) - Where N represents the number of elements inside 
 *                  the array. The search requires a fixed iteration count to satisfy floating precision 
 *                  limits. Each refinement pass uses a linear O(N) two-pointer scan.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The fractional calculation model executes completely in-place 
 *                   using simple index markers, requiring zero extra dynamic table slots.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the prime array: ";
    if (!(cin >> n) || n < 2) {
        cout << "Invalid parameter. The array size must be at least 2." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter prime array numbers in strictly ascending sorted order separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    // Input verification check to ensure items conform to ascending requirements
    if (!is_sorted(arr.begin(), arr.end())) {
        cout << "Constraint Error: Elements must be entered in sorted order." << endl;
        return 1;
    }

    int k;
    cout << "Enter the target fractional element rank value to locate (K): ";
    int maxK = (n * (n - 1)) / 2;
    if (!(cin >> k) || k < 1 || k > maxK) {
        cout << "Constraint Error: K must be within the valid fractional range [1, " << maxK << "]." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting fractional value binary search with two-pointer edge counting..." << endl;
    vector<int> fractionResult = solver.kthSmallestPrimeFraction(arr, k);

    if (!fractionResult.empty()) {
        cout << "The " << k << "-th smallest prime fraction is: " 
             << fractionResult[0] << " / " << fractionResult[1] 
             << " (Value: " << static_cast<double>(fractionResult[0]) / fractionResult[1] << ")" << endl;
    } else {
        cout << "Error: Target item could not be resolved within standard precision metrics." << endl;
    }

    return 0;
}