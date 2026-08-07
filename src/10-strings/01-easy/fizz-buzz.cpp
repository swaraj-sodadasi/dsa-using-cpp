#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: STRING VECTOR ALLOCATION PATTERNS
     * -------------------------------------------------------------------------------------
     * The objective is to return a string array representation of the integers from 1 to n.
     * 
     * 1. THE HEAP ALLOCATION TRADEOFF (OPTIMIZING MEMORY CHURN):
     *    A naive implementation might repeatedly use `push_back()` to add elements to the vector. 
     *    However, as the vector grows beyond its pre-allocated capacity, it triggers expensive 
     *    reallocation steps—copying old elements to a larger block of memory. To avoid this overhead, 
     *    we pre-allocate the exact space needed using `reserve()` or by initializing the vector with a fixed size.
     * 
     * 2. MODULO CONDITIONAL HIERARCHY:
     *    For each integer `i` in the range [1, n]:
     *      - Check divisibility by both 3 and 5 first (i.e., `i % 15 == 0`). If matched, append "FizzBuzz".
     *      - Check divisibility by 3 (`i % 3 == 0`). If matched, append "Fizz".
     *      - Check divisibility by 5 (`i % 5 == 0`). If matched, append "Buzz".
     *      - If none of the conditions match, convert the integer to a string using `to_string(i)`.
     */
    vector<string> fizzBuzz(int n) {
        // Step 1: Pre-allocate the vector size to prevent dynamic resizing overhead
        vector<string> result;
        if (n <= 0) return result;
        
        result.reserve(static_cast<size_t>(n));

        // Step 2: Evaluate divisibility rules sequentially
        for (int i = 1; i <= n; ++i) {
            if (i % 15 == 0) {
                result.push_back("FizzBuzz");
            } else if (i % 3 == 0) {
                result.push_back("Fizz");
            } else if (i % 5 == 0) {
                result.push_back("Buzz");
            } else {
                result.push_back(to_string(i));
            }
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the target number boundary limit n. The single loop 
 *                  executes exactly N times, performing basic mathematical modulo updates. 
 *                  String conversions scale with the number of digits, which is bounded by a constant.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Excluding the memory required to store the final answer vector, 
 *                   the algorithm runs in true constant space without initializing temporary tables.
 */
int main() {
    int targetBoundary;
    cout << "=== Fizz Buzz String Vector Allocator Console ===\n";
    cout << "Enter the upper target integer range limit cap (n): ";
    if (!(cin >> targetBoundary)) return 1;

    Solution solver;
    cout << "\nPre-allocating string containers and checking modulo division patterns..." << endl;
    vector<string> fizzBuzzArray = solver.fizzBuzz(targetBoundary);

    cout << "\nGenerated Output Result Matrix:\n[ ";
    for (size_t i = 0; i < fizzBuzzArray.size(); ++i) {
        cout << "\"" << fizzBuzzArray[i] << "\"" << (i + 1 < fizzBuzzArray.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}