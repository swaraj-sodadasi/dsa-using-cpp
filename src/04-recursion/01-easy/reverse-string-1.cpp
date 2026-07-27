#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
CORE LOGIC (IN-PLACE RECURSIVE SWAPPING):
- The objective is to reverse an array of characters in-place using a recursive formulation.
- **Two-Pointer Recursive Reduction**:
  - We define a recursive helper function that accepts a left boundary pointer (`left`) and a right boundary pointer (`right`).
  - **Base Case**: If `left >= right`, the pointers have met or crossed in the center of the string. The entire structure is inverted, so the recursion returns.
  - **Recursive Step**: 
    1. Swap the characters at the current boundaries: `swap(s[left], s[right])`.
    2. Progress inward by calling the function recursively with updated boundaries: `left + 1` and `right - 1`.
- **In-Place Mutation**:
  - By passing the character vector by reference (`vector<char>&`), modifications affect the original string directly without allocating new arrays.

TIME COMPLEXITY: O(N) - We perform exactly $N/2$ swaps, visiting each element once.
SPACE COMPLEXITY: O(N) auxiliary - The recursion stack grows linearly with the depth of the call stack, which reaches $N/2$ frames.
*/

void reverseStringHelper(vector<char>& s, size_t left, size_t right) {
    // Base Case: Pointers have converged or crossed in the middle
    if (left >= right) {
        return;
    }

    // Swapping components at the current exterior index boundaries
    swap(s[left], s[right]);

    // Recurse inward toward the remaining center characters
    reverseStringHelper(s, left + 1, right - 1);
}

void reverseString(vector<char>& s) {
    if (s.empty()) return;
    reverseStringHelper(s, 0, s.size() - 1);
}

int main() {
    string inputLine;
    cout << "Enter a string to reverse: ";
    if (!getline(cin, inputLine)) {
        cout << "Invalid input string stream entry." << endl;
        return 1;
    }

    // Convert the standard input string into a vector of characters
    vector<char> s(inputLine.begin(), inputLine.end());

    cout << "\nOriginal character array sequence:  [ ";
    for (char c : s) cout << c << " ";
    cout << "]" << endl;

    reverseString(s);

    cout << "Reversed character array sequence:  [ ";
    for (char c : s) cout << c << " ";
    cout << "]" << endl;

    // Output unified string format back for confirmation
    string resultString(s.begin(), s.end());
    cout << "Unified result outcome: \"" << resultString << "\"" << endl;

    return 0;
}