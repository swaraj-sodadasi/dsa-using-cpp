#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: GREEDY SORTING / PRIORITY DECREMENTING
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the minimum number of character deletions needed to 
     * make all character frequencies in a string unique.
     * 
     * 1. FIXED-CAPACITY INTENSITY PROFILES:
     *    Instead of allocating deep, complex hash dynamic objects, we create a fixed-size 
     *    vector array `frequencies` of size 26. This maps the standard lowercase English 
     *    alphabet characters ('a' through 'z') in true O(1) constant auxiliary space.
     * 
     * 2. THE GREEDY SORTING CONTRACT:
     *    After calculating the base frequencies, we sort the array in descending order. 
     *    This arranges the highest frequency values at the front, letting us enforce uniqueness 
     *    from highest to lowest. We establish an upper limit parameter `maxAllowedFrequency` 
     *    initialized to the highest frequency present.
     * 
     * 3. PRIORITY DECREMENTING PIPELINE:
     *    We iterate down through our sorted frequencies:
     *      - If the current frequency exceeds `maxAllowedFrequency`, it must be reduced to avoid 
     *        a collision. The difference `frequencies[i] - maxAllowedFrequency` is added to our 
     *        deletion count.
     *      - We then calculate the next valid threshold for the following element: 
     *        `maxAllowedFrequency = max(0, maxAllowedFrequency - 1)`. This ensures frequencies 
     *        never drop below zero, even under heavy deletion cascades.
     */
    int minDeletions(string s) {
        // Step 1: Count initial character frequencies using a fixed 26-element array
        vector<int> frequencies(26, 0);
        for (char c : s) {
            frequencies[static_cast<size_t>(c - 'a')]++;
        }

        // Step 2: Sort frequencies in descending order to apply the greedy scan pattern
        sort(frequencies.rbegin(), frequencies.rend());

        int deletionCount = 0;
        // Initialize the upper ceiling value to the largest frequency found
        int maxAllowedFrequency = frequencies[0];

        // Step 3: Run the priority decrementing check down the sorted array
        for (int i = 0; i < 26; ++i) {
            if (frequencies[static_cast<size_t>(i)] == 0) {
                break; // Remaining character profiles do not exist in the source text
            }

            // If the current frequency is too high, cut it down to match the max allowed ceiling
            if (frequencies[static_cast<size_t>(i)] > maxAllowedFrequency) {
                deletionCount += (frequencies[static_cast<size_t>(i)] - maxAllowedFrequency);
            } else {
                // Adjust the ceiling downward to match the current safe frequency value
                maxAllowedFrequency = frequencies[static_cast<size_t>(i)];
            }

            // Decrement the ceiling threshold for the next step, clamping at zero
            maxAllowedFrequency = max(0, maxAllowedFrequency - 1);
        }

        return deletionCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input string s. We perform a single 
 *                  pass over the string to populate our frequency log. Sorting the array takes O(1) constant 
 *                  time because the size of the array is fixed at 26 elements.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The frequency list and processing variables maintain a locked capacity 
 *                   of 26 units, remaining completely independent of the size of the input text string.
 */
int main() {
    string inputText;
    cout << "=== Minimum Deletions to Make Character Frequencies Unique Console ===\n";
    cout << "Enter a string consisting of lowercase English letters exclusively:\n";
    if (!(cin >> inputText)) return 1;

    // Validate character bounds early
    for (char c : inputText) {
        if (c < 'a' || c > 'z') {
            cout << "Constraint Error: Input text contains invalid out-of-bounds characters." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nLaunching character grid counts and running greedy cascade reductions..." << endl;
    int minimumDeletionsComputed = solver.minDeletions(inputText);

    cout << "\nMinimum character deletion cycles executed: " << minimumDeletionsComputed << " operations.\n";

    return 0;
}