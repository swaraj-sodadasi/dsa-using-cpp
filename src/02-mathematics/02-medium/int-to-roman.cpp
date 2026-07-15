#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
CORE LOGIC (GREEDY VALUE-MAPPING DECOMPOSITION):
- Roman numerals are written largest to smallest from left to right. 
- To convert an integer to its Roman numeral representation, we map out all unique numeral symbols alongside their respective subtractive combination edge cases (like 4 -> IV, 9 -> IX, 40 -> XL, etc.) in descending order.
- We iterate through our mapped pairs starting from the largest value (1000). 
- While the input `num` is greater than or equal to the current numeral value, we append the corresponding Roman numeral string to our result and subtract the value from `num`.
- This greedy decomposition guarantees that we naturally produce the shortest and most accurate standard Roman numeral representation.

TIME COMPLEXITY: O(1) - The loop runs over a fixed, finite array of 13 numeral mappings. The number of subtractions is strictly bounded because the maximum possible input value is bounded (typically 3999).
SPACE COMPLEXITY: O(1) auxiliary - The structural memory utilization is restricted entirely to the output string buffer.
*/
string intToRoman(int num) {
    // Parallel vectors mapping numeric values to their structural Roman numeral tokens
    const vector<pair<int, string>> RomanMapping = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
        {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"},
        {1, "I"}
    };

    string romanNumeral = "";

    // Greedily match and subtract values from largest to smallest
    for (const auto& pair : RomanMapping) {
        while (num >= pair.first) {
            romanNumeral += pair.second;
            num -= pair.first;
        }
    }

    return romanNumeral;
}

int main() {
    int num;
    cout << "Enter an integer between 1 and 3999: ";
    cin >> num;

    if (num < 1 || num > 3999) {
        cout << "Invalid input. Roman numerals traditionally cover the range 1 to 3999." << endl;
        return 1;
    }

    string result = intToRoman(num);
    cout << "\nThe Roman numeral representation of " << num << " is: \"" << result << "\"" << endl;

    return 0;
}