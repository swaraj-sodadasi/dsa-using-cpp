#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * STRING LEXICOGRAPHICAL CONCATENATION CUSTOM COMPARISON ENGINE
     * -------------------------------------------------------------
     * The objective is to arrange a list of non-negative integers such that they 
     * form the largest possible combined number representation, returning it as a string.
     *
     * Mathematical & Algorithmic Strategy:
     * - A naive approach sorting numbers by value or pure dictionary order fails. For example, 
     *   given 3 and 30, standard sorting puts 30 ahead of 3, but "3" + "30" = "330" is larger than "303".
     * - To establish a total ordering across all elements, we apply a custom concatenation comparator:
     *   For any two integer string representations `a` and `b`, we compare the combined outcomes:
     *   - Option A: `order1 = a + b`
     *   - Option B: `order2 = b + a`
     * - If `order1 > order2`, then string `a` must precede string `b` in the final layout. 
     *   This comparison rule forms a strict weak ordering, which guarantees a globally optimal sorted sequence.
     *
     * Transformation Protocol Steps:
     * - Step 1: Convert all integer elements into their respective string formats.
     * - Step 2: Sort the string array using our custom inline lambda concatenation comparator.
     * - Step 3: Handle the edge case where the largest sorted number is "0". If the head element 
     *   is "0", the entire array consists of zeroes. Return "0" directly instead of "000...".
     * - Step 4: Concatenate the sorted elements sequentially into a single string container.
     */
    string largestNumber(vector<int>& nums) {
        if (nums.empty()) return "";

        // Step 1: Convert all input integers into string layout profiles
        vector<string> numStrings;
        numStrings.reserve(nums.size());
        for (int num : nums) {
            numStrings.push_back(to_string(num));
        }

        // Step 2: Sort strings using the custom combination lambda rule
        sort(numStrings.begin(), numStrings.end(), [](const string& a, const string& b) {
            return (a + b) > (b + a);
        });

        // Step 3: Handle the global zero edge case scenario
        if (numStrings[0] == "0") {
            return "0";
        }

        // Step 4: Concatenate the sorted elements sequentially to form the unified result
        string largestNumberResult = "";
        for (const string& str : numStrings) {
            largestNumberResult += str;
        }

        return largestNumberResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N * K) - Where N represents the number of elements in the array and K 
 *                  is the average length of the string representations. The sorting phase makes 
 *                  O(N log N) comparisons, and each comparison performs a string concatenation 
 *                  and evaluation taking O(K) time.
 *
 * SPACE COMPLEXITY: O(N * K) Auxiliary - To store the temporary string transformations of each 
 *                   integer inside our layout processing vector.
 */
int main() {
    int n;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive integer value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter non-negative integer array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Elements cannot be negative integers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting string lexicographical concatenation sort steps..." << endl;
    string computedLargestNumber = solver.largestNumber(nums);

    cout << "Maximized aggregate combined number sequence: " << computedLargestNumber << endl;

    return 0;
}