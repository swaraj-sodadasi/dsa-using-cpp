#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (DYNAMIC STATE TRACKING / SLIDING WINDOW):
- A subarray is turbulent if the comparison sign flips between adjacent elements at each step (e.g., > , < , > , <).
- We iterate through the array starting from index 1 while maintaining the size of the current turbulent subarray.
- We can track the current condition using a sign indicator or by comparing the relationship of the current pair with the previous pair.
- Let `climb` represent the length of the turbulent subarray ending at the current index.
- For each element `arr[i]`:
  - If `arr[i] > arr[i-1]` and the previous step was a decrease (`arr[i-1] < arr[i-2]`), or vice versa, the turbulence continues: we increment our window size.
  - If the turbulence breaks but the two adjacent elements are not equal (`arr[i] != arr[i-1]`), a new turbulent window starts with a size of 2.
  - If they are equal (`arr[i] == arr[i-1]`), the window resets completely to a size of 1.
- We maintain a global maximum of these window lengths across the entire traversal.

TIME COMPLEXITY: O(N) - We perform a single linear traversal over the array of size N.
SPACE COMPLEXITY: O(1) - Only a few primitive scalar variables are maintained to track the state and maximum length.
*/
int maxTurbulenceSize(const vector<int>& arr) {
    size_t n = arr.size();
    if (n < 2) return static_cast<int>(n);

    int maxLength = 1;
    int currentLength = 1;
    int prevSign = 0; // 0: flat/start, 1: positive diff, -1: negative diff

    for (size_t i = 1; i < n; ++i) {
        int currentSign = (arr[i] > arr[i - 1]) ? 1 : ((arr[i] < arr[i - 1]) ? -1 : 0);

        if (currentSign == 0) {
            currentLength = 1;
        } else if (currentSign == -prevSign) {
            currentLength++;
        } else {
            currentLength = 2;
        }

        maxLength = max(maxLength, currentLength);
        prevSign = currentSign;
    }

    return maxLength;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    int result = maxTurbulenceSize(arr);
    cout << "\nThe maximum length of a turbulent subarray is: " << result << endl;

    return 0;
}