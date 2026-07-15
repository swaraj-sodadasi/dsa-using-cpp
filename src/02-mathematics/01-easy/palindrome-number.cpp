#include <iostream>

using namespace std;

/*
CORE LOGIC (HALF-INTEGER REVERSAL WITHOUT OVERFLOW):
- Negative numbers cannot be palindromes because the negative sign at the front will not match the final digit.
- Numbers that end in 0 but are not 0 themselves cannot be palindromes (e.g., 10, 130) because no number starts with 0.
- Instead of reversing the entire integer—which risks triggering a numeric overflow for large inputs—we reverse only the **back half** of the number and compare it against the remaining front half.
- We strip the last digit from the original number (`x % 10`) and append it to our reversed variable (`reversedHalf = reversedHalf * 10 + x % 10`), while simultaneously dividing the original number by 10.
- We stop this process when the original number becomes less than or equal to the reversed half (`x <= reversedHalf`). This indicates we have reached or passed the middle point.
- **Comparison Check**: 
  - For an even number of digits, if the number is a palindrome, `x` will equal `reversedHalf`.
  - For an odd number of digits, the middle digit will be the last digit of `reversedHalf`. We can safely discard it by doing `reversedHalf / 10` before checking for equality.

TIME COMPLEXITY: O(log10(N)) - We divide the input value by 10 in each loop iteration, meaning we process only half of the total base-10 digits.
SPACE COMPLEXITY: O(1) - The check is executed in-place using only a single extra primitive integer variable.
*/
bool isPalindrome(int x) {
    // Edge case: Negative numbers and numbers ending in 0 (except 0 itself) are not palindromes
    if (x < 0 || (x % 10 == 0 && x != 0)) {
        return false;
    }

    int reversedHalf = 0;
    while (x > reversedHalf) {
        reversedHalf = reversedHalf * 10 + (x % 10);
        x /= 10;
    }

    // For even number of digits: x == reversedHalf
    // For odd number of digits: x == reversedHalf / 10 (discards the middle digit)
    return x == reversedHalf || x == reversedHalf / 10;
}

int main() {
    int x;
    cout << "Enter an integer: ";
    cin >> x;

    if (isPalindrome(x)) {
        cout << x << " is a palindrome number." << endl;
    } else {
        cout << x << " is NOT a palindrome number." << endl;
    }

    return 0;
}