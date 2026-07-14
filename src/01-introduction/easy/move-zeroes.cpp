#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (TWO-POINTER IN-PLACE MANIPULATION):
- We maintain a 'writeIndex' pointer initialized to 0, which tracks where the next non-zero element should be placed.
- We iterate through the array using a 'readIndex' pointer from start to finish.
- Whenever 'nums[readIndex]' is non-zero, we swap the elements at 'readIndex' and 'writeIndex', then increment 'writeIndex'.
- Swapping instead of overwriting inherently handles pushing the zeroes to the back without needing a secondary loop to fill trailing elements, while maintaining the relative ordering of all non-zero elements.

TIME COMPLEXITY: O(N) - We perform a single linear traversal over the array of size N.
SPACE COMPLEXITY: O(1) - The operations are performed in-place using shifts/swaps, avoiding extra dynamic allocations.
*/
void moveZeroes(vector<int>& nums) {
    size_t writeIndex = 0;
    for (size_t readIndex = 0; readIndex < nums.size(); ++readIndex) {
        if (nums[readIndex] != 0) {
            swap(nums[readIndex], nums[writeIndex]);
            writeIndex++;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    moveZeroes(nums);
    
    cout << "\nArray after moving zeroes: ";
    for (int i = 0; i < n; ++i) {
        cout << nums[i] << " ";
    }
    cout << endl;
    
    return 0;
}