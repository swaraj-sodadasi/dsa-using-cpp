#include <iostream>
#include <string>
#include <numeric>

using namespace std;

/*
CORE LOGIC (EUCLIDEAN ALGORITHM ADAPTATION):
- For two strings `str1` and `str2`, a common divisor string `X` exists if and only if both strings 
  are formed by repeating `X` multiple times.
- If such a common divisor string exists, concatenating the two strings in different orders must produce 
  the identical result: `str1 + str2 == str2 + str1`. If this condition is false, no common divisor string exists.
- If the structural condition is met, the length of the Greatest Common Divisor (GCD) string is exactly 
  equal to the greatest common divisor of the lengths of `str1` and `str2`.
- We use the standard Euclidean algorithm (`std::gcd`) to find this length: `gcdLength = std::gcd(str1.length(), str2.length())`.
- The final GCD string is simply the prefix substring of `str1` from index 0 up to `gcdLength`.

TIME COMPLEXITY: O(N + M) - Building and comparing the concatenated strings of lengths N and M takes linear time. 
                 The integer GCD step itself takes O(log(min(N, M))) operations.
SPACE COMPLEXITY: O(N + M) - Necessary allocations to generate the two combined validation string copies (`str1 + str2` and `str2 + str1`).
*/
string gcdOfStrings(const string& str1, const string& str2) {
    // Check if a valid common divisor string is topologically possible
    if (str1 + str2 != str2 + str1) {
        return "";
    }
    
    // Find the greatest common divisor of the two lengths
    size_t gcdLength = std::gcd(str1.length(), str2.length());
    
    // Return the prefix slice corresponding to the calculated length
    return str1.substr(0, gcdLength);
}

int main() {
    string str1, str2;
    cout << "Enter the first string (str1): ";
    cin >> str1;
    cout << "Enter the second string (str2): ";
    cin >> str2;

    string result = gcdOfStrings(str1, str2);

    if (result.empty()) {
        cout << "\nThere is no common divisor string." << endl;
    } else {
        cout << "\nThe greatest common divisor string is: \"" << result << "\"" << endl;
    }

    return 0;
}