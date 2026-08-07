#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: IN-PLACE TWO-POINTER SWAPPING
     * -------------------------------------------------------------------------------------
     * The objective is to reverse an array of characters in-place modifying the input 
     * array directly with O(1) extra memory.
     * 
     * 1. THE ARCHITECTURAL APPROACH TRADEOFF (RECURSION VS POINTERS):
     *    - Recursive Approach: While conceptually elegant, a recursive implementation creates
     *      a stack frame for every nested function call. For an array of size N, this results 
     *      in O(N) space complexity on the call stack, violating the strict constant auxiliary 
     *      space constraint.
     *    - Two-Pointer Approach (Chosen): We place one pointer `left` at the beginning of the 
     *      array (index 0) and another pointer `right` at the trailing boundary (`s.size() - 1`). 
     *      We swap the values at these positions and move the pointers closer together (`left++`, `right--`). 
     *      The loop terminates when the pointers meet or cross, achieving O(1) space efficiency.
     * 
     * 2. SYMMETRIC ELEMENT COLLISION SYSTEM:
     *    Using a simple `while (left < right)` loop ensures that we touch each index position exactly 
     *    once, executing precisely `N / 2` swap steps.
     */
    void reverseString(vector<char>& s) {
        if (s.empty()) return;

        int left = 0;
        int right = static_cast<int>(s.size()) - 1;

        // Converge pointers inward toward the center element
        while (left < right) {
            // Swap symmetric character memory blocks in-place
            swap(s[static_cast<size_t>(left)], s[static_cast<size_t>(right)]);
            
            left++;
            right--;
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of characters in the vector array. 
 *                  The loop processes exactly N / 2 swap cycles, resulting in a strictly linear execution.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The swap operation modifies elements directly within the existing 
 *                   vector structure without allocating secondary dynamic array containers.
 */
int main() {
    int charCount;
    cout << "Enter the total number of characters in the string array: ";
    if (!(cin >> charCount) || charCount < 0) {
        cout << "Invalid string size configured." << endl;
        return 1;
    }

    vector<char> s(static_cast<size_t>(charCount));
    if (charCount > 0) {
        cout << "Enter the characters consecutively (separated by spaces or returns):\n";
        for (int i = 0; i < charCount; ++i) {
            cin >> s[static_cast<size_t>(i)];
        }
    }

    cout << "\nOriginal Character Array Structure:\n[ ";
    for (size_t i = 0; i < s.size(); ++i) {
        cout << "'" << s[i] << "'" << (i + 1 < s.size() ? ", " : "");
    }
    cout << " ]\n";

    Solution solver;
    cout << "\nExecuting in-place dual two-pointer collision swapping..." << endl;
    solver.reverseString(s);

    cout << "\nReversed Character Array Result:\n[ ";
    for (size_t i = 0; i < s.size(); ++i) {
        cout << "'" << s[i] << "'" << (i + 1 < s.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}