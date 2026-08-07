#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CHARACTER ARRAY SUB-INSERTION REPLACEMENTS
     * -------------------------------------------------------------------------------------
     * The objective is to return a defanged version of a standard IPv4 address, where 
     * every period "." is replaced with "[.]".
     * 
     * 1. PRE-ALLOCATION OPTIMIZATION METHODOLOGY:
     *    A naive string construction method appends characters to an empty string one by one. 
     *    However, as the string grows, this triggers frequent, expensive memory reallocations 
     *    and element copying on the heap.
     *    An IPv4 address contains exactly 3 period characters. Replacing each `.` with `[.]` 
     *    adds exactly 2 extra characters per replacement, increasing the final string length 
     *    by exactly 6 characters. By allocating the exact required size (`address.length() + 6`) 
     *    upfront using `reserve()`, we completely eliminate runtime capacity adjustments.
     * 
     * 2. SINGLE-PASS CONDITIONAL PARSING:
     *    We traverse the original IP string sequentially character by character:
     *      - If the character is a period `'.'`, we append the three-character block `"[.]"`.
     *      - For any other character (digits `0`-`9`), we append it directly to our result.
     */
    string defangIPaddr(string address) {
        string defangedAddress = "";
        
        // Step 1: Pre-allocate capacity to completely prevent heap reallocation overhead
        defangedAddress.reserve(address.length() + 6);

        // Step 2: Traverse and append characters via a linear sweep
        for (char c : address) {
            if (c == '.') {
                defangedAddress.append("[.]");
            } else {
                defangedAddress.push_back(c);
            }
        }

        return defangedAddress;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input IP address string.
 *                  The single pass loop parses each character exactly once, running in strict 
 *                  linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Aside from the memory allocated for the final output string, 
 *                   the algorithm runs in true constant working space without temporary tables.
 */
int main() {
    string ipAddress;
    cout << "=== Defanging an IP Address Allocation Console ===\n";
    cout << "Enter a valid IPv4 address string (e.g., 1.1.1.1):\n";
    if (!(cin >> ipAddress)) return 1;

    // Basic validity assertion rule
    size_t periodCount = 0;
    for (char c : ipAddress) {
        if (c == '.') periodCount++;
    }
    if (periodCount != 3) {
        cout << "Constraint Warning: Standard IPv4 formatting requires exactly three period separators." << endl;
    }

    Solution solver;
    cout << "\nPre-calculating size parameters and executing string sub-insertions..." << endl;
    string outcomeResult = solver.defangIPaddr(ipAddress);

    cout << "Defanged network target string result: " << outcomeResult << "\n";

    return 0;
}