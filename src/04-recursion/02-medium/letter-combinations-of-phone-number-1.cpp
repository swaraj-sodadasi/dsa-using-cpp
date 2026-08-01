#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
CORE LOGIC (MULTI-BRANCH CARTESIAN PRODUCT BACKTRACKING):
- The problem asks us to return all possible letter combinations that a string of digits (from 2-9) 
  could represent based on a traditional phone keypad mapping.
- **Cartesian Product via Backtracking State Representation**:
  - We can solve this by generating the Cartesian product of the character sets mapped to each input digit.
  - We define a recursive function `backtrack(digitIndex, currentCombination)` where:
    - `digitIndex`: Tracks our current position within the input `digits` string.
    - `currentCombination`: A string acting as an accumulator for the current path combination.
- **State Transition Strategy**:
  - **Base Case**: When `digitIndex == digits.length()`, we have successfully chosen one valid letter 
    for each digit in the input. We add the completed `currentCombination` to our results list and return.
  - **Branching Step**: 
    1. Look up the string of possible letters corresponding to `digits[digitIndex]` from our keypad map.
    2. Loop through each character `ch` in that letter string:
       - Append `ch` to `currentCombination`.
       - Recurse deeper by calling `backtrack(digitIndex + 1, currentCombination)`.
       - Undo the choice by removing `ch` from `currentCombination` (Backtracking).

TIME COMPLEXITY: O(4^N) - Where N is the total number of digits in the input string. A digit can map to 
                 at most 4 letters (e.g., 7 and 9). The total number of leaf nodes in the choice tree is bounded by $4^N$.
SPACE COMPLEXITY: O(N) auxiliary - The depth of the runtime recursion stack scales linearly with the input length N.
*/

class Solution {
private:
    vector<string> allCombinations;
    
    // Constant mapping matching traditional phone keypad letter layouts
    const vector<string> keypadMap = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

    void generateCombinations(const string& digits, size_t digitIndex, string& currentCombination) {
        // Base Case: The combination length matches the input digit count
        if (digitIndex == digits.length()) {
            allCombinations.push_back(currentCombination);
            return;
        }

        // Extract the active digit character and map it to its integer value
        int digitVal = digits[digitIndex] - '0';
        const string& possibleLetters = keypadMap[static_cast<size_t>(digitVal)];

        // Branch out for every possible letter option mapped to this digit
        for (char ch : possibleLetters) {
            // Push choice onto the accumulator path string
            currentCombination.push_back(ch);

            // Recurse to solve the sub-problem for the subsequent digit index
            generateCombinations(digits, digitIndex + 1, currentCombination);

            // Backtracking state recovery step
            currentCombination.pop_back();
        }
    }

public:
    vector<string> letterCombinations(const string& digits) {
        allCombinations.clear();
        
        // Edge Case: An empty input string yields zero combinations
        if (digits.empty()) {
            return allCombinations;
        }

        string currentCombination = "";
        generateCombinations(digits, 0, currentCombination);
        
        return allCombinations;
    }
};

int main() {
    string digits;
    cout << "Enter the string of phone digits (containing values between 2 and 9 only): ";
    if (!(cin >> digits)) {
        cout << "Invalid input string stream reading parameters." << endl;
        return 1;
    }

    // Input verification check to ensure digits fall strictly within the 2-9 range
    for (char c : digits) {
        if (c < '2' || c > '9') {
            cout << "Constraint Error: Keypad digits must strictly range between '2' and '9'." << endl;
            return 1;
        }
    }

    Solution solver;
    vector<string> result = solver.letterCombinations(digits);

    cout << "\nAll combinations generated via multi-branch Cartesian backtracking:\n";
    cout << "[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "\"" << result[i] << "\"";
        if (i + 1 < result.size()) cout << ", ";
    }
    cout << " ]\n";
    cout << "Total distinct letter combinations found: " << result.size() << endl;

    return 0;
}