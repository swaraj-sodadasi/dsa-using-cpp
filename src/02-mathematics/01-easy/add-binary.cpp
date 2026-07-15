#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BITWISE-LIKE STRING ARITHMETIC):
- We simulate the process of manual binary addition by processing both string layouts from right to left (least significant bit to most significant bit).
- We maintain two pointers, `i` and `j`, initialized to the final character boundaries of string `a` and string `b` respectively.
- A variable `carry` tracks the overflow bit from the previous position. It is initialized to 0.
- In each loop iteration, we continue if there are still characters to process in `a` or `b`, or if a residual `carry` remains to be written.
- For each position:
  - We extract the bit numeric values: if `i >= 0`, we add `a[i] - '0'` to the running sum and decrement `i`. If `j >= 0`, we add `b[j] - '0'` to the running sum and decrement `j`.
  - The new bit to append to our result is calculated as: `sum % 2`.
  - The updated carry for the next position is calculated as: `sum / 2`.
- Since we construct the result from right to left, the characters are appended in reverse order. We apply a final string inversion (`std::reverse`) before returning the result.

TIME COMPLEXITY: O(max(N, M)) - Where N and M are the lengths of strings `a` and `b`. The algorithm executes a single linear pass proportional to the longer string.
SPACE COMPLEXITY: O(1) auxiliary - The structural memory utilization is restricted entirely to the returning result string buffer.
*/
string addBinary(const string& a, const string& b) {
    string result = "";
    int i = static_cast<int>(a.length()) - 1;
    int j = static_cast<int>(b.length()) - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;

        if (i >= 0) {
            sum += a[static_cast<size_t>(i)] - '0';
            i--;
        }
        if (j >= 0) {
            sum += b[static_cast<size_t>(j)] - '0';
            j--;
        }

        // Calculate the current position bit and the next carry
        result.push_back(static_cast<char>((sum % 2) + '0'));
        carry = sum / 2;
    }

    // Reverse the string to get the final correct binary placement order
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    string a, b;
    cout << "Enter the first binary string (a): ";
    cin >> a;
    cout << "Enter the second binary string (b): ";
    cin >> b;

    // Optional basic validation for input string types
    for (char c : a) {
        if (c != '0' && c != '1') {
            cout << "Error: String 'a' is not a valid binary format string." << endl;
            return 1;
        }
    }
    for (char c : b) {
        if (c != '0' && c != '1') {
            cout << "Error: String 'b' is not a valid binary format string." << endl;
            return 1;
        }
    }

    string finalSum = addBinary(a, b);
    cout << "\nThe resulting binary sum string is: \"" << finalSum << "\"" << endl;

    return 0;
}