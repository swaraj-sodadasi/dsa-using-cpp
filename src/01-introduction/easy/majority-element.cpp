#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BOYER-MOORE VOTING ALGORITHM):
- The algorithm processes the array element by element, maintaining a 'candidate' and a 'count'.
- The 'count' is initialized to 0. When 'count' becomes 0, we choose the current element as our new 'candidate'.
- For each element encountered:
  - If it matches the 'candidate', we increment 'count'.
  - If it differs from the 'candidate', we decrement 'count'.
- Because the majority element appears more than floor(N / 2) times, its votes will always outnumber 
  the combined votes of all other minority elements, ensuring it remains the final candidate.

TIME COMPLEXITY: O(N) - We perform a single linear pass over the array of size N.
SPACE COMPLEXITY: O(1) - Only a few primitive variables ('candidate', 'count') are used; no dynamic memory allocated.
*/
int majorityElement(const vector<int>& nums) {
    int candidate = 0;
    int count = 0;
    
    for (int num : nums) {
        if (count == 0) {
            candidate = num;
        }
        count += (num == candidate) ? 1 : -1;
    }
    
    return candidate;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline (ensure a majority element exists): ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    int result = majorityElement(nums);
    cout << "\nThe majority element is: " << result << endl;
    
    return 0;
}