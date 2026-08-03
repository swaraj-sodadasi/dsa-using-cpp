#include <iostream>
#include <string>
#include <cmath>
#include <climits>

using namespace std;

class Solution {
public:
    /*
     * MATHEMATICAL REPRESENTATION BINARY SEARCH / BASE EXTRACTION ENGINE
     * -------------------------------------------------------------------
     * The objective is to find the smallest base k >= 2 such that the string 
     * representation of n in base k consists entirely of '1's.
     *
     * Mathematical & Search Framework:
     * - An integer n in base k consisting entirely of '1's can be expressed as a geometric series:
     *   n = 1 + k + k^2 + k^3 + ... + k^m
     * - Given that n <= 10^18, the maximum number of bits/power levels m is strictly bounded by 60 (2^60 > 10^18).
     * - To find the *smallest* base k, we must maximize the number of terms m. Therefore, we iterate 
     *   through the maximum possible length m down to 2.
     *
     * Polynomial Binary Search Protocol:
     * - For a fixed power level m, the value of the geometric series is strictly monotonic with respect to k.
     *   This allows us to binary search for the precise integer base k.
     * - Mathematical bound initialization: k^m < n implies k < n^(1/m). We use this value (+1 for safety) 
     *   as our initial upper bound tracker (`right`) to heavily compress the search window.
     * - Inside the search, we compute the series sum. If an integer multiplication overflow occurs or the 
     *   sum exceeds n, we prune the right half. If the sum is smaller than n, we prune the left half.
     * - If no smaller base matches for m >= 2, we fall back to the mathematical guarantee that (n - 1) 
     *   is always a valid base (n = 1 + (n - 1)^1), representing n as "11" in base n - 1.
     */
    string smallestGoodBase(string n_str) {
        long long n = stoll(n_str);
        
        // Iterate from the maximum possible power level m down to 2
        for (int m = 60; m >= 2; --m) {
            // Apply exponential bounds to initialize the binary search window parameters
            long long left = 2;
            long long right = static_cast<long long>(pow(n, 1.0 / m)) + 1;

            while (left <= right) {
                long long midBase = left + (right - left) / 2;
                long long currentSum = 0;
                long long currentTerm = 1;
                bool overflowOccurred = false;

                // Evaluate the geometric series sum: 1 + k + k^2 + ... + k^m
                for (int i = 0; i <= m; ++i) {
                    currentSum += currentTerm;
                    
                    if (i < m) {
                        // Check for long long multiplication overflow before calculating the next term
                        if (currentTerm > (LLONG_MAX / midBase)) {
                            overflowOccurred = true;
                            break;
                        }
                        currentTerm *= midBase;
                    }
                }

                if (overflowOccurred || currentSum > n) {
                    right = midBase - 1; // Candidate base value is too large; branch left
                } else if (currentSum < n) {
                    left = midBase + 1;  // Candidate base value is too small; branch right
                } else {
                    return to_string(midBase); // Perfect structural matching base isolated
                }
            }
        }

        // Base case fallback: (n - 1) always represents the value as "11"
        return to_string(n - 1);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log^2 N) - The outer loop runs at most 60 times. The inner binary search 
 *                  converges within log(N^(1/m)) steps, evaluating a series of length m.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Polynomial evaluations are handled purely in-place 
 *                   using local scalar variables, consuming no additional memory frames.
 */
int main() {
    string n_str;
    cout << "Enter the integer value n (as a string within the range [3, 10^18]): ";
    if (!(cin >> n_str)) return 1;

    try {
        Solution solver;
        cout << "\nExecuting mathematical representation base extraction search..." << endl;
        string baseResult = solver.smallestGoodBase(n_str);
        
        cout << "Smallest good base discovered for " << n_str << " is: " << baseResult << endl;
    } catch (const exception& e) {
        cout << "Execution Error: Provided parameter breached parsing boundaries." << endl;
        return 1;
    }

    return 0;
}