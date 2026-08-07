#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ROW-INDEXED MATRIX PROJECTION STRING ACCUMULATION
     * -------------------------------------------------------------------------------------
     * The objective is to write the string `s` in a zigzag pattern across a fixed number of rows 
     * (`numRows`), and then read it line by line to produce a newly structured string.
     */
    string convert(string s, int numRows) {
        // Base Boundary Condition: If numRows is 1 or greater than the string length, no zigzagging occurs
        if (numRows <= 1 || numRows >= static_cast<int>(s.length())) {
            return s;
        }

        // Step 1: Allocate compact, row-indexed vector strings to track parallel projections
        vector<string> rowBuffers(static_cast<size_t>(numRows));
        
        int currentRow = 0;
        bool goingDown = false;

        // Step 2: Route characters sequentially into row paths using wave trajectory bounces
        for (char c : s) {
            rowBuffers[static_cast<size_t>(currentRow)].push_back(c);

            // Change direction when hitting the absolute upper or lower row boundaries
            if (currentRow == 0 || currentRow == numRows - 1) {
                goingDown = !goingDown;
            }

            // Step the row index indicator forward or backward based on the active tracking state
            currentRow += goingDown ? 1 : -1;
        }

        // Step 3: Flatten row segments into a single contiguous cache-friendly buffer
        string result = "";
        result.reserve(s.length());
        
        for (const string& rowStr : rowBuffers) {
            result.append(rowStr);
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total character length of string s.
 * SPACE COMPLEXITY: O(N) Auxiliary - Packed row allocation structures.
 */
int main() {
    string sourceText;
    int configuredRowsCount;
    
    cout << "=== Zigzag Matrix Projection Conversion Engine ===\n";
    
    cout << "Enter the target number of rows: ";
    if (!(cin >> configuredRowsCount) || configuredRowsCount <= 0) {
        cout << "Invalid row height configuration." << endl;
        return 1;
    }

    // FIX: Clear the trailing newline character left behind in the buffer by 'cin >>'
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter the string to evaluate (spaces are safely accepted):\n";
    // FIX: Use getline so strings containing spaces or symbols are processed completely
    if (!getline(cin, sourceText)) {
        cout << "Failed to read input string." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nProjecting trajectory paths and compressing rows sequence arrays..." << endl;
    string transformedResult = solver.convert(sourceText, configuredRowsCount);

    cout << "\nZigzag Flattened Output String Outcome:\n\"" << transformedResult << "\"\n";

    return 0;
}