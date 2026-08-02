#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * BACKTRACKING DEPTH-FIRST SEARCH ENGINE WITH STATE PRUNING
     * ---------------------------------------------------------
     * Recursively evaluates suffix partitions to check if they can form a valid descending
     * consecutive sequence where each subsequent numeric conversion is exactly 1 less
     * than the preceding baseline value.
     */
    bool checkSubstrings(const string& s, size_t index, unsigned long long previousValue, int segmentCount) {
        // Base Case Success Anchor: We reached the end of the string and have formed at least 2 segments
        if (index == s.length()) {
            return segmentCount >= 2;
        }

        unsigned long long currentNumericValue = 0;
        size_t n = s.length();

        // Branching Execution: Explore every valid substring slice starting from the current index anchor
        for (size_t i = index; i < n; ++i) {
            // Step 1: Accumulate digits dynamically inline to compute numerical value
            int currentDigit = s[i] - '0';
            
            // Critical Overflow Pruning Invariant: Prevent tracking variables from breaking bounds
            // 1844674407370955161ULL is roughly (ULLONG_MAX - 9) / 10
            if (currentNumericValue > 1844674407370955161ULL) {
                break; 
            }
            currentNumericValue = currentNumericValue * 10 + currentDigit;

            // Step 2: Evaluate Parity Rules or Structural Sequencing Dependencies
            if (segmentCount == 0) {
                // For the initial baseline segment, we can choose any number as long as it leaves 
                // room for at least one subsequent character to form the descending chain.
                if (i == n - 1) break; // Cannot consume the entire string on the first segment step
                
                if (checkSubstrings(s, i + 1, currentNumericValue, 1)) {
                    return true;
                }
            } else {
                // CRITICAL PRUNING INVARIANT: The value must be exactly equal to previousValue - 1
                if (currentNumericValue == previousValue - 1) {
                    if (checkSubstrings(s, i + 1, currentNumericValue, segmentCount + 1)) {
                        return true;
                    }
                    // Since it matches exactly, any further extension of this partition will only 
                    // grow the value, breaking the strict decreasing chain constraint. We prune early.
                    break; 
                }
                
                // If the value grows strictly greater than the target state, stop searching down this path
                if (currentNumericValue >= previousValue) {
                    break;
                }
            }
        }

        return false;
    }

public:
    /*
     * SPLITTING A STRING INTO DESCENDING CONSECUTIVE VALUES
     * -----------------------------------------------------
     * The objective is to determine whether we can split a numeric string 's' into 
     * two or more non-empty substrings such that the numerical values of the substrings 
     * form a descending consecutive sequence (e.g., "050043" -> [5, 4, 3]).
     *
     * Algorithmic Optimization Strategy:
     * - A naive tracking approach generates all possible combinations of string splits, which 
     *   incurs exponential runtime hazards.
     * - We optimize this by applying a backtracking depth-first search combined with 
     *   strict pruning constraints. 
     * - Because `currentNumericValue` can grow rapidly, we use `unsigned long long` to prevent 
     *   integer overflow bugs, and we explicitly prune branches the moment our accumulated value 
     *   meets or exceeds the `previousValue`.
     */
    bool splitString(string s) {
        if (s.empty()) return false;
        return checkSubstrings(s, 0, 0, 0);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(2^N) Worst Case - Where N represents the total length of the input string.
 *                  Although the theoretical worst-case bound is exponential, the aggressive 
 *                  state pruning criteria (`currentVal >= previousValue`) eliminates the vast majority 
 *                  of invalid search paths instantly, resulting in near-linear performance in practice.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization engine operates entirely in place on the input string, 
 *                   requiring stack memory layout proportional to N to track frame recursion depth.
 */
int main() {
    string inputStr;
    cout << "Enter a numeric digits string sequence (e.g., 050043, 908070): ";
    if (!(cin >> inputStr)) {
        return 1;
    }

    // Constraint Verification: Ensure string contains only numeric characters
    if (any_of(inputStr.begin(), inputStr.end(), [](char c) { return !isdigit(c); })) {
        cout << "Constraint Error: String must contain only numeric characters." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting backtracking state pruning validation sweep..." << endl;
    
    cout << boolalpha;
    bool outcomeResult = solver.splitString(inputStr);

    cout << "\nValidation Split Evaluation Outcome: " << outcomeResult << endl;
    if (outcomeResult) {
        cout << "Success: The string can be broken into a valid descending consecutive sequence framework." << endl;
    } else {
        cout << "Fail: No valid split partition combination satisfies the structural constraints." << endl;
    }

    return 0;
}