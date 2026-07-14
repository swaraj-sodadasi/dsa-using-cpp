#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (TWO-POINTER CONVERGENCE):
- Since the input array is already sorted in non-decreasing order, the largest squares will reside at the extreme ends (either the most negative numbers at the beginning or the largest positive numbers at the end).
- We initialize a 'left' pointer at index 0 and a 'right' pointer at the last index ('size - 1').
- We allocate a result vector of the same size and fill it from back to front using a 'writeIndex' pointer starting at 'size - 1'.
- In each step, we square the values at both pointers and compare them.
- Whichever square is larger is placed at 'result[writeIndex]', and we move that respective boundary pointer inward.
- We decrement 'writeIndex' and repeat until the pointers cross.

TIME COMPLEXITY: O(N) - We iterate through the array of size N exactly once to construct the sorted result.
SPACE COMPLEXITY: O(N) - Storing the sorted squared numbers requires an output array of size N. The algorithm uses O(1) auxiliary space beyond the required output storage.
*/
vector<int> sortedSquares(const vector<int>& nums) {
    size_t n = nums.size();
    vector<int> result(n);
    
    int left = 0;
    int right = static_cast<int>(n) - 1;
    int writeIndex = static_cast<int>(n) - 1;
    
    while (left <= right) {
        int leftSquare = nums[left] * nums[left];
        int rightSquare = nums[right] * nums[right];
        
        if (leftSquare > rightSquare) {
            result[writeIndex] = leftSquare;
            left++;
        } else {
            result[writeIndex] = rightSquare;
            right--;
        }
        writeIndex--;
    }
    
    return result;
}

int main() {
    int n;
    cout << "Enter the number of elements in the sorted array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " sorted integers (can include negative values) separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    vector<int> result = sortedSquares(nums);
    
    cout << "\nSquared and sorted array: ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << " ";
    }
    cout << endl;
    
    return 0;
}