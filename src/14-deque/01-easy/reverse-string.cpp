#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: REVERSE STRING II (SEGMENTED CHUNK-BASED INVERSIONS)
     * -------------------------------------------------------------------------------------
     * The objective is to reverse the first k characters for every 2k characters 
     * counting from the start of the string.
     * 
     * 1. CHUNKED ITERATION LAYOUT:
     *    We iterate through the string in strides of `2 * k` step sizes.
     * 
     * 2. BOUNDARY-CHECKED REVERSAL:
     *    For each chunk starting at index `i`:
     *      - The end index for the reversed segment is `min(i + k, n)`.
     *      - We reverse the sub-range `[i, min(i + k, n))` using in-place two-pointer swapping.
     */
    string reverseStr(string s, int k) {
        int n = static_cast<int>(s.length());

        for (int i = 0; i < n; i += 2 * k) {
            int left = i;
            int right = min(i + k, n) - 1;

            while (left < right) {
                swap(s[static_cast<size_t>(left)], s[static_cast<size_t>(right)]);
                left++;
                right--;
            }
        }

        return s;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of string s.
 *                  Every character is visited at most once during swapping operations.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Modifications performed in-place.
 */
int main() {
    cout << "=== Reverse String II Segmented Engine ===\n";
    cout << "Enter input string s: ";
    string s;
    if (!(cin >> s)) return 1;

    cout << "Enter value for k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    string result = solver.reverseStr(s, k);

    cout << "Resulting string after chunk reversals: " << result << "\n";

    return 0;
}