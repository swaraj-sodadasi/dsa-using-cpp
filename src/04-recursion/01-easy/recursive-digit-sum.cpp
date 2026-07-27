#include <iostream>
#include <string>

using namespace std;

/*
CORE LOGIC (SUPER DIGIT SUM COMPUTATION):
- The problem asks us to find the "super digit" of a number represented as a string `n` concatenated `k` times.
- The super digit of a single-digit number is the number itself. For a number with multiple digits, its super digit is calculated by recursively summing its digits until only a single digit remains.
- **Mathematical Optimization / Structural Reduction**:
  - Instead of concatenating the string `n` a total of `k` times (which would cause a massive memory explosion and a crash), we use a distributive property of arithmetic:
    $$ \text{superDigit}(n \times k) = \text{superDigit}(\text{superDigit}(n) \times k) $$
  - We first calculate the initial sum of the digits of `n` in a single linear pass using a 64-bit integer (`long long`) to prevent integer overflow.
  - We then multiply this initial sum by `k` to get the total starting value.
- **Recursive Processing**:
  - We pass this total sum into a recursive function `computeSuperDigit`.
  - **Base Case**: If the value is less than 10, it is already a single digit, so we return it.
  - **Recursive Step**: If the value has multiple digits, we calculate the sum of its digits and make a recursive call with this new sum.

TIME COMPLEXITY: O(L + log_10(InitialSum)) - Where L is the length of the string `n`. The initial pass takes linear time proportional to L. Subsequent recursive digit extractions reduce the number size logarithmically, taking negligible time.
SPACE COMPLEXITY: O(log_10(InitialSum)) auxiliary - Stack space required for the shallow recursive call frames.
*/

// Recursive function to reduce a number down to its single-digit super digit
int computeSuperDigit(long long value) {
    // Base Case: The value is already a single digit
    if (value < 10) {
        return static_cast<int>(value);
    }

    long long digitSum = 0;
    while (value > 0) {
        digitSum += value % 10;
        value /= 10;
    }

    // Recursive Step: Call again with the computed sum of digits
    return computeSuperDigit(digitSum);
}

int superDigit(const string& n, int k) {
    long long initialDigitSum = 0;

    // Calculate the sum of the digits of the initial string string n
    for (char c : n) {
        initialDigitSum += (c - '0');
    }

    // Multiply the single-string sum by the replication factor k
    long long totalStartingSum = initialDigitSum * k;

    // Begin the recursive single-digit compaction process
    return computeSuperDigit(totalStartingSum);
}

int main() {
    string n;
    int k;

    cout << "Enter the numeric string (n): ";
    if (!(cin >> n)) {
        cout << "Invalid string configuration input entry." << endl;
        return 1;
    }

    // Input verification check
    for (char c : n) {
        if (c < '0' || c > '9') {
            cout << "Constraint Error: String must consist entirely of numeric digits." << endl;
            return 1;
        }
    }

    cout << "Enter the repetition factor (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid repetition factor constraint alignment." << endl;
        return 1;
    }

    cout << "\nExecuting super digit sum computation..." << endl;
    int result = superDigit(n, k);

    cout << "The super digit of the configuration is: " << result << endl;

    return 0;
}