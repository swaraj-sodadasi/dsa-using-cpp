#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * SPEED CONSTRAINTS FEASIBILITY EVALUATOR
     * ----------------------------------------
     * Calculates the total hours required to consume all bananas in the piles 
     * given a fixed eating speed `speed` (bananas per hour). 
     * Returns true if Koko can finish all piles within the limit `h`.
     */
    bool canEatAllWithinTime(const vector<int>& piles, int h, int speed) {
        long long totalHoursSpent = 0;

        for (int bananas : piles) {
            // Calculate ceiling division: ceil(bananas / speed)
            // Using integer arithmetic to prevent floating-point precision issues
            totalHoursSpent += (bananas + speed - 1) / speed;

            // Early pruning: if accumulated hours breach the threshold, abort early
            if (totalHoursSpent > h) return false;
        }

        return totalHoursSpent <= h;
    }

public:
    /*
     * MONOTONIC SPEED CONSTRAINTS EVALUATION ENGINE
     * ---------------------------------------------
     * The objective is to determine the minimum integer eating speed `k` 
     * that allows Koko to eat all the bananas within `h` hours.
     *
     * Mathematical & Search Framework:
     * - The relationship between eating speed and completion time exhibits a monotonic behavior: 
     *   If Koko can finish all bananas at speed `S`, she can definitely finish them at any speed 
     *   greater than `S`. If she fails at speed `S`, any lower speed will also fail.
     * - This monotonic property allows us to execute a Binary Search on the Answer Range 
     *   to locate the optimal value of `k`.
     *
     * Boundary Initialization Protocol:
     * - Lower Bound (`left`): 1 banana per hour. This represents the absolute minimum possible speed.
     * - Upper Bound (`right`): The maximum value in `piles`. Eating faster than the largest pile 
     *   does not save any hours on that pile, as she can only finish one pile per hour.
     */
    int minEatingSpeed(vector<int>& piles, int h) {
        // Establish search range boundaries
        int left = 1;
        int right = *max_element(piles.begin(), piles.end());
        int optimalSpeed = right;

        // Execute binary search across the monotonic target speed spectrum
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (canEatAllWithinTime(piles, h, mid)) {
                optimalSpeed = mid; // Register the current valid speed candidate
                right = mid - 1;    // Attempt to discover a tighter, slower speed boundary
            } else {
                left = mid + 1;     // Speed is too slow; force shift the lower bound upward
            }
        }

        return optimalSpeed;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * log(MaxPile)) - Where N represents the total number of banana piles. 
 *                  The binary search space spans from 1 to the largest value in the piles array. 
 *                  Each step invokes an O(N) linear parsing verification loop.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The feasibility calculation engine runs completely in-place 
 *                   using local loop values, requiring zero extra dynamic frame space layouts.
 */
int main() {
    int n;
    cout << "Enter the total number of banana piles: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The collection size must be greater than 0." << endl;
        return 1;
    }

    vector<int> piles(static_cast<size_t>(n));
    cout << "Enter the banana quantities per pile separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> piles[static_cast<size_t>(i)];
        if (piles[static_cast<size_t>(i)] <= 0) {
            cout << "Constraint Error: Piles must contain a positive count of items." << endl;
            return 1;
        }
    }

    int h;
    cout << "Enter the maximum hours constraint parameter (H must be >= piles count): ";
    if (!(cin >> h) || h < n) {
        cout << "Constraint Error: Hours must be greater than or equal to the number of piles." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting monotonic speed constraints binary search tracking pass..." << endl;
    int optimalSpeedResult = solver.minEatingSpeed(piles, h);

    cout << "Minimum hourly eating speed factor discovered (K): " << optimalSpeedResult << " bananas/hour." << endl;

    return 0;
}