#include <iostream>

using namespace std;

// Mock function representing the external API constraint provided by the system framework.
// For demonstration purposes, this value is defined globally and set dynamically inside main.
int globalFirstBadVersion = 0;
bool isBadVersion(int version) {
    return version >= globalFirstBadVersion;
}

class Solution {
public:
    /*
     * LEFT-BIASED BINARY DECISION BOUNDARY LOCATION ENGINE
     * ----------------------------------------------------
     * The objective is to locate the first "bad" version out of N consecutive versions, 
     * minimizing the total number of calls made to the `isBadVersion` API function.
     *
     * Algorithmic Strategy:
     * - The structural configuration forms a monotonically sorted boolean array layout 
     *   consisting of two distinct state groups: `[false, false, ..., true, true]`.
     * - We leverage a left-biased binary search engine to pinpoint the precise transition 
     *   boundary (the very first instance where the state flips to `true`).
     *
     * Boundary Maintenance Protocol:
     * - We initialize the tracking bounds `left = 1` and `right = n`.
     * - Midpoint Calculation: `mid = left + (right - left) / 2` is used to prevent integer overflow.
     * - Evaluation Steps:
     *   - If `isBadVersion(mid)` returns `true`, it means version `mid` is bad. The *first* bad 
     *     version is either `mid` itself or somewhere to its left. We contract the window 
     *     by moving the right pointer directly to the midpoint: `right = mid`.
     *   - If `isBadVersion(mid)` returns `false`, it means version `mid` is good. The first bad 
     *     version must lie strictly to the right. We update the left pointer to look beyond `mid`: 
     *     `left = mid + 1`.
     * - Loop Termination: The loop exits the moment `left == right`. At this convergence point, 
     *   the index is mathematically guaranteed to be the first bad version.
     */
    int firstBadVersion(int n) {
        int left = 1;
        int right = n;

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (isBadVersion(mid)) {
                // The current mid point is bad; the historical pivot point lies at or before mid
                right = mid;
            } else {
                // The current version is safe; the transition boundary must exist strictly after mid
                left = mid + 1;
            }
        }

        // At convergence, left and right meet to mark the boundary point location
        return left;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - The search space range is halved during each inspection step, 
 *                  ensuring the boundary is located within at most log_2(N) API invocations.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The search loop maintains state via scalar indexing variables, 
 *                   requiring no extra memory layout footprints on the stack framework.
 */
int main() {
    int n;
    cout << "Enter the total pool size of target versions (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Total versions count must be greater than 0." << endl;
        return 1;
    }

    cout << "Enter the hidden index of the first bad version (must fall within the range [1, " << n << "]): ";
    if (!(cin >> globalFirstBadVersion) || globalFirstBadVersion < 1 || globalFirstBadVersion > n) {
        cout << "Constraint Error: Invalid mock setup boundaries." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting left-biased binary decision boundary tracking sweep..." << endl;
    int firstBadIndexResult = solver.firstBadVersion(n);

    cout << "Identified first bad version transition index point: " << firstBadIndexResult << endl;
    cout << "Validation Check: " << (firstBadIndexResult == globalFirstBadVersion ? "PASSED" : "FAILED") << endl;

    return 0;
}