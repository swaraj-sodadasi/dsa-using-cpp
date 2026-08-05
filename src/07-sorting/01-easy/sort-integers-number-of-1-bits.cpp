#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
private:
    /*
     * BIT COUNT EXTRACTION HELPER
     * ---------------------------
     * Computes the total number of set bits (1 bits) in the binary representation of an integer.
     * Uses Brian Kernighan’s algorithm to clear the lowest set bit in each iteration loop,
     * ensuring performance proportional strictly to the number of set bits rather than total bits.
     */
    int countSetBits(int num) {
        int count = 0;
        while (num > 0) {
            num &= (num - 1); // Clears the lowest set bit structure
            count++;
        }
        return count;
    }

public:
    /*
     * CUSTOM COMPARATOR LAMBDA SORTING ENGINE
     * ---------------------------------------
     * The objective is to sort an integer array in ascending order based on the number of 
     * 1 bits in their binary representation. If two or more integers share the exact same 
     * number of 1 bits, they must be sorted in ascending order by their numerical values.
     *
     * Algorithmic Strategy:
     * - We implement a custom comparator passed directly inside an inline lambda expression 
     *   to the standard library `std::sort` template algorithm.
     * - For any two values `a` and `b`, the custom comparator extracts their respective 
     *   set bit counts: `bitCountA = countSetBits(a)` and `bitCountB = countSetBits(b)`.
     * - Evaluation Priority Protocol:
     *   1. If `bitCountA != bitCountB`, return `bitCountA < bitCountB` to order by bit weights.
     *   2. If `bitCountA == bitCountB`, fall back to standard value comparisons: `return a < b`.
     */
    vector<int>& sortByBits(vector<int>& arr) {
        // Execute sort operation bound to the inline custom lambda constraint profile
        sort(arr.begin(), arr.end(), [this](int a, int b) {
            int bitCountA = countSetBits(a);
            int bitCountB = countSetBits(b);
            
            if (bitCountA != bitCountB) {
                return bitCountA < bitCountB;
            }
            return a < b; // Tie-breaker condition using pure value weights
        });

        return arr;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements in the collection array. 
 *                  The standard sort algorithm takes O(N log N) comparison steps. Each step 
 *                  invokes the bit count extraction utility running in O(K) time, where K is 
 *                  bounded by the bit width of the integer data type (at most 32 operations).
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The elements are shifted completely in-place 
 *                   by standard swapping mechanisms, consuming no extra dynamic container space.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter non-negative integer array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
        if (arr[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Array elements must be non-negative integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting custom comparator lambda bit weight sorting sweep..." << endl;
    solver.sortByBits(arr);

    cout << "Bit-weight ordered array layout output:\n[ ";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << (i + 1 < arr.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}