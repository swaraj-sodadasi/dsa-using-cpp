#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MONOTONIC STACK INDEX DISTANCE CALCULATIONS
     * -------------------------------------------------------------------------------------
     * The objective is to compute the number of days you have to wait until a warmer 
     * temperature for each daily recording.
     * 
     * 1. THE INDEX DISTANCE INVERSE TRACKING:
     *    Instead of storing value tokens, our stack tracks array index offsets (`i`). 
     *    This design lets us calculate wait durations on the fly by computing the 
     *    distance between indices: `distance = currentWarmerDayIndex - colderDayIndex`.
     * 
     * 2. THE LIQUIDATION LAYER:
     *    We scan the array from left to right. The active temperature `temperatures[i]` 
     *    acts as a potential breakthrough candidate for past unresolved cold days. 
     *    As long as the stack is not empty and `temperatures[i]` is strictly greater than 
     *    the temperature at our stack's top index, we have found the *next warmer day*. 
     *    We pop the colder day's index, compute the distance (`i - colderIdx`), write it 
     *    to the output array, and repeat.
     * 
     * 3. AUXILIARY PRE-INITIALIZATION MATRICES:
     *    The output vector is pre-initialized with zeros. Elements that have no warmer days 
     *    downstream remain in the stack throughout the sweep. Their corresponding indices 
     *    retain the default value of 0, matching the problem specifications perfectly.
     */
    vector<int> dailyTemperatures(const vector<int>& temperatures) {
        int totalDays = static_cast<int>(temperatures.size());
        vector<int> waitDurations(static_cast<size_t>(totalDays), 0);
        stack<int> unresolvedDaysStack;

        for (int i = 0; i < totalDays; ++i) {
            // Liquidate indices from the stack whose temperatures are colder than the current day
            while (!unresolvedDaysStack.empty() && temperatures[static_cast<size_t>(i)] > temperatures[static_cast<size_t>(unresolvedDaysStack.top())]) {
                int colderDayIdx = unresolvedDaysStack.top();
                unresolvedDaysStack.pop();
                waitDurations[static_cast<size_t>(colderDayIdx)] = i - colderDayIdx;
            }
            // Register the current day's index to await a warmer downstream breakthrough
            unresolvedDaysStack.push(i);
        }

        return waitDurations;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements inside the temperatures vector.
 *                  Each daily index is pushed onto the stack exactly once and popped at most once.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The implicit stack memory contains at most 
 *                   N data frames under strictly decreasing temperature tracks (e.g., [80, 75, 70, 60]).
 */
int main() {
    cout << "=== Monotonic Stack Daily Temperatures Wait Engine ===\n";
    cout << "Enter the total number of days recorded: ";
    int daysCount;
    if (!(cin >> daysCount) || daysCount < 0) return 1;

    vector<int> temperaturesList(static_cast<size_t>(daysCount));
    if (daysCount > 0) {
        cout << "Enter the daily integer temperatures sequentially:\n";
        for (int i = 0; i < daysCount; ++i) {
            cin >> temperaturesList[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nScanning structural intervals and tracking monotonic index gaps..." << endl;
    vector<int> customWaitMatrix = solver.dailyTemperatures(temperaturesList);

    cout << "Calculated Wait Days Result Vector: [ ";
    for (int duration : customWaitMatrix) cout << duration << " ";
    cout << "]\n";

    return 0;
}