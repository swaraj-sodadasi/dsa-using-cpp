#include <iostream>

using namespace std;

// Mock variable representing the hidden target number to be guessed.
// For demonstration purposes, this value is defined globally and set dynamically inside main.
int globalTargetNumber = 0;

/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return -1 if num is higher than the picked number
 *          1 if num is lower than the picked number
 *          otherwise return 0
 */
int guess(int num) {
    if (num > globalTargetNumber) return -1;
    if (num < globalTargetNumber) return 1;
    return 0;
}

class Solution {
public:
    /*
     * INTERACTIVE BINARY SEARCH ENGINE
     * --------------------------------
     * The objective is to guess a pre-selected hidden target number within a range 
     * from 1 to N by making standard guess interactions via a pre-defined API.
     *
     * Algorithmic Strategy:
     * - The range of options represents a monotonically ordered numerical continuum.
     *   This allows us to leverage a standard binary search framework to locate the target.
     * - We maintain search space boundaries via two scalar bounds trackers: `left = 1` and `right = n`.
     *
     * Interactive Reduction Scheme:
     * - Calculate the midpoint securely to avoid integer overflow issues: `mid = left + (right - left) / 2`.
     * - Invoke `guess(mid)` to read the feedback signal:
     *   - Outcome `0`: The current midpoint `mid` matches the hidden number. The value is returned immediately.
     *   - Outcome `-1`: The guess is higher than the hidden number (`mid > target`). The target lies below `mid`.
     *     We restrict the search space by shifting the upper bound: `right = mid - 1`.
     *   - Outcome `1`: The guess is lower than the hidden number (`mid < target`). The target lies above `mid`.
     *     We adjust the lower bound to point beyond the current midpoint: `left = mid + 1`.
     * - The loop continues processing until the target is resolved or convergence occurs.
     */
    int guessNumber(int n) {
        int left = 1;
        int right = n;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            int responseCode = guess(mid);

            if (responseCode == 0) {
                return mid; // Target correctly identified
            } else if (responseCode == -1) {
                right = mid - 1; // Target lies strictly in the lower partition half
            } else {
                left = mid + 1;  // Target lies strictly in the upper partition half
            }
        }

        return left;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) - The search boundaries are halved at each iteration step, 
 *                  limiting the total number of API execution requests to at most log_2(N).
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The state machine operates using simple local index variables, 
 *                   requiring no extra dynamic allocations on the runtime stack framework.
 */
int main() {
    int n;
    cout << "Enter the upper bound limit factor of the number range (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The range size limit must be greater than 0." << endl;
        return 1;
    }

    cout << "Enter the hidden target number to be guessed (must fall within range [1, " << n << "]): ";
    if (!(cin >> globalTargetNumber) || globalTargetNumber < 1 || globalTargetNumber > n) {
        cout << "Constraint Error: The picked number must reside inside the defined search space boundaries." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting interactive binary search game engine loop..." << endl;
    int guessResult = solver.guessNumber(n);

    cout << "Identified hidden target number location value: " << guessResult << endl;
    cout << "Validation Check: " << (guessResult == globalTargetNumber ? "PASSED" : "FAILED") << endl;

    return 0;
}