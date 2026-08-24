#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: IN-PLACE VECTOR STACK CHARACTER ELIMINATION
     * -------------------------------------------------------------------------------------
     * The objective is to remove all adjacent duplicate characters from a string repeatedly.
     * 
     * 1. THE IN-PLACE VECTOR STACK CONSTRAINTS:
     *    Standard stack implementations (like std::stack<char>) copy characters into separate
     *    dynamic containers and require a secondary reverse stitching pass to format the string,
     *    resulting in O(N) auxiliary space overhead.
     *    Instead, we treat the original string input itself as a dynamic vector-backed stack.
     * 
     * 2. THE DUAL-CURSOR COLLAPSING ENGINE:
     *    We maintain a custom stack pointer cursor `topIndex` initialized to -1.
     *    We iterate through the input string with a forward scanning pointer `i`.
     *    For each character `s[i]`:
     *      - If `topIndex >= 0` and the character at `s[topIndex]` matches the current character `s[i]`,
     *        we have detected an adjacent duplicate. We pop the element by decrementing `topIndex`.
     *      - Otherwise, the character is distinct relative to the active top boundary. We push it
     *        onto our stack by incrementing `topIndex` and copying the character down: `s[++topIndex] = s[i]`.
     * 
     * 3. STRIP BOUNDARY RESIZING:
     *    Once the single-pass sweep concludes, all duplicate sets have collapsed completely.
     *    The valid, unique character elements remain locked between index 0 and `topIndex`.
     *    We resize the string down to exactly `topIndex + 1` characters in-place.
     */
    string removeDuplicates(string s) {
        int topIndex = -1;
        int stringLength = static_cast<int>(s.length());

        for (int i = 0; i < stringLength; ++i) {
            if (topIndex >= 0 && s[static_cast<size_t>(topIndex)] == s[static_cast<size_t>(i)]) {
                // Duplicate pair detected; pop the last element from the in-place stack framework
                topIndex--;
            } else {
                // Distinct boundary confirmed; push the element forward onto the top pointer slot
                topIndex++;
                s[static_cast<size_t>(topIndex)] = s[static_cast<size_t>(i)];
            }
        }

        // Resize the mutated character matrix down to match the final accumulated stack bounds
        s.resize(static_cast<size_t>(topIndex + 1));
        return s;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of the input string.
 *                  The collapsing logic visits each index slot exactly once in a single sweep.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The deletion mutations execute completely in-place inside 
 *                   the allocated character buffer memory, requiring zero extra space structures.
 */
int main() {
    cout << "=== In-Place Vector Stack Character Elimination Engine ===\n";
    cout << "Enter the string to check for adjacent duplicates: ";
    
    string inputString;
    if (!(cin >> inputString)) return 1;

    cout << "\nOriginal String Matrix: [" << inputString << "]\n";
    
    Solution solver;
    cout << "Running twin-pointer scans and collapsing adjacent character boundaries..." << endl;
    string processedOutcome = solver.removeDuplicates(inputString);

    cout << "Processed Outcome Result: [" << processedOutcome << "]\n";

    return 0;
}