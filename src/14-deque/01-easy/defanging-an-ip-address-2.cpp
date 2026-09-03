#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DOUBLE-ENDED STRING ARRAY EXPANSION LAYOUT
     * -------------------------------------------------------------------------------------
     * The objective is to replace every period "." in an IPv4 address with "[.]".
     * 
     * 1. PRE-ALLOCATION / SPACE OPTIMIZATION:
     *    An IPv4 address has at most 3 periods. Replacing each '.' with '[.]' expands 
     *    the length by 2 characters per period (total addition of 6 characters).
     *    By reserving memory upfront via `reserve()`, we avoid multiple heap reallocations.
     * 
     * 2. SINGLE-PASS EXPANSION TRAVERSAL:
     *    We iterate through the input string character-by-character and append the 
     *    defanged representation directly into the output buffer.
     */
    string defangIPaddr(const string& address) {
        string defanged;
        // Pre-allocate memory: original size + 6 extra chars for 3 expanded dots
        defanged.reserve(address.length() + 6);

        for (char ch : address) {
            if (ch == '.') {
                defanged += "[.]";
            } else {
                defanged += ch;
            }
        }

        return defanged;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of the string (fixed <= 15 for IPv4).
 *                  Single linear traversal over input characters.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Output buffer allocated with exact required size.
 */
int main() {
    cout << "=== IPv4 Address Defanging Engine ===\n";
    cout << "Enter valid IPv4 address (e.g., 1.1.1.1): ";
    string ip;
    if (!(cin >> ip)) return 1;

    Solution solver;
    string result = solver.defangIPaddr(ip);

    cout << "Defanged IP Address: " << result << "\n";

    return 0;
}