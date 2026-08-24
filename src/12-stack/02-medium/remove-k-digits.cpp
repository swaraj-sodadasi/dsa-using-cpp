#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: GREEDY MONOTONIC INCREASING STACK PEAK DIGIT REMOVAL
     * -------------------------------------------------------------------------------------
     * The objective is to remove k digits from the string 'num' so that the new number 
     * is the smallest possible.
     * 
     * 1. THE GREEDY INTUITION:
     *    To minimize a number, we want its most significant digits (leftmost digits) to be 
     *    as small as possible. When scanning left-to-right, if we see a digit that is larger 
     *    than the next digit (a peak), dropping it immediately lowers the overall value.
     * 
     * 2. MONOTONIC STACK DEPLOYMENT:
     *    We treat a string as a dynamic stack container. For each character `digit`:
     *      - While k > 0, the stack is not empty, and the top element is larger than `digit`, 
     *        we greedily pop the peak element from the stack and decrement k.
     *      - Otherwise, we push `digit` onto the stack to build a monotonic increasing sequence.
     * 
     * 3. RESIDUAL CLEANUP & LEADING ZERO STRIPPING:
     *    If the sweep ends and we still have operations left (k > 0), we pop remaining digits 
     *    from the tail (since the stack is monotonic increasing, the tail holds the largest values).
     *    Finally, we strip out any leading zeros and handle the empty string fallback edge case.
     */
    string removeKdigits(string num, int k) {
        if (static_cast<int>(num.length()) <= k) {
            return "0";
        }

        string digitStack = "";

        for (char digit : num) {
            // Drop peak digits to keep the stack monotonic increasing
            while (k > 0 && !digitStack.empty() && digitStack.back() > digit) {
                digitStack.pop_back();
                k--;
            }
            digitStack.push_back(digit);
        }

        // If k operations are left, truncate the largest elements from the right end
        while (k > 0 && !digitStack.empty()) {
            digitStack.pop_back();
            k--;
        }

        // Strip leading zeros from the processed sequence buffer
        size_t zeroIndex = 0;
        while (zeroIndex < digitStack.length() && digitStack[zeroIndex] == '0') {
            zeroIndex++;
        }

        string result = digitStack.substr(zeroIndex);
        return result.empty() ? "0" : result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the input number string.
 *                  Every individual character digit is pushed and popped at most once.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The string container holds at most 
 *                   N digits during the dynamic stacking process.
 */
int main() {
    cout << "=== Greedy Monotonic Digit Deletion Engine ===\n";
    cout << "Enter the numeric string input digits: ";
    string num;
    if (!(cin >> num)) return 1;

    cout << "Enter the count of digits to discard (k): ";
    int k;
    if (!(cin >> k)) return 1;

    Solution solver;
    cout << "\nAnalyzing dynamic value slopes and discarding peaks..." << endl;
    string optimizedResult = solver.removeKdigits(num, k);

    cout << "Minimum Achievable Value Outcome: [" << optimizedResult << "]\n";

    return 0;
}