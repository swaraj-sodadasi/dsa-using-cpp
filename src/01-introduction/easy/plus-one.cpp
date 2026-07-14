#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (ARRAY-BASED ARBITRARY-PRECISION ARITHMETIC):
- We iterate through the digits array backwards, starting from the least significant digit (index size - 1).
- For each digit, if it is less than 9, we can simply increment it by 1 and return the array immediately, as there will be no further carry.
- If the digit is 9, incrementing it turns it into 10. We set this digit to 0 and continue to the next iteration to process the carry.
- If the loop finishes entirely, it implies every single digit in the original array was 9 (e.g., 999 becomes 000). 
- In this specific edge case, we insert a 1 at the beginning of the vector using `digits.insert()` or by setting the first element to 1 and pushing a 0 to the back, expanding the array capacity dynamically.

TIME COMPLEXITY: O(N) - In the worst case (all 9s), we traverse the entire array of size N once.
SPACE COMPLEXITY: O(N) - In-place manipulation occurs unless all digits are 9, which requires allocating an additional digit space, taking O(N) space to allocate/return the modified vector.
*/
vector<int> plusOne(vector<int>& digits) {
    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        if (digits[i] < 9) {
            digits[i]++;
            return digits;
        }
        digits[i] = 0;
    }
    
    digits.insert(digits.begin(), 1);
    return digits;
}

int main() {
    int n;
    cout << "Enter the number of digits in the integer array: ";
    cin >> n;
    
    vector<int> digits(n);
    cout << "Enter " << n << " digits (0-9) representing the integer, separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> digits[i];
    }
    
    vector<int> result = plusOne(digits);
    
    cout << "\nResult after adding one: ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i];
    }
    cout << endl;
    
    return 0;
}