#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DELIMITED STRING SUBSTRING PARTITION ELIMINATION
     * -------------------------------------------------------------------------------------
     * The objective is to parse a delimited string, filter out target partition 
     * substrings, and reconstruct the result without empty partitions or orphaned delimiters.
     * 
     * 1. TWO-WAY WINDOW PARSING & SCANNING:
     *    We scan through the string maintaining start and end window pointers for each 
     *    delimited segment.
     * 
     * 2. PARTITION ELIMINATION & BUFFER RECONSTRUCTION:
     *    - Extract each partition bounded by the delimiter.
     *    - If the partition is non-empty and does not match the target partition to eliminate, 
     *      append it to the output buffer joined by the single delimiter.
     */
    string eliminatePartitions(const string& s, char delimiter, const string& eliminateTarget) {
        string result;
        result.reserve(s.length());

        size_t start = 0;
        size_t n = s.length();

        while (start < n) {
            // Find boundary of next partition
            size_t end = s.find(delimiter, start);
            if (end == string::npos) {
                end = n;
            }

            string token = s.substr(start, end - start);

            // Filter out empty tokens and matching target partitions
            if (!token.empty() && token != eliminateTarget) {
                if (!result.empty()) {
                    result += delimiter;
                }
                result += token;
            }

            start = end + 1;
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N is the length of string s.
 *                  Linear traversal across the input string.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Output string buffer construction.
 */
int main() {
    cout << "=== Delimited String Substring Partition Elimination Engine ===\n";
    cout << "Enter delimited input string: ";
    string s;
    if (!(cin >> s)) return 1;

    cout << "Enter single character delimiter: ";
    char delimiter;
    if (!(cin >> delimiter)) return 1;

    cout << "Enter target partition substring to eliminate: ";
    string target;
    cin >> target;

    Solution solver;
    string result = solver.eliminatePartitions(s, delimiter, target);

    cout << "Reconstructed string: " << result << "\n";

    return 0;
}