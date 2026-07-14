#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (TWO-POINTER IN-PLACE MANIPULATION):
- Since the array is already sorted, all duplicate elements will be adjacent to each other.
- We maintain a 'writeIndex' pointer initialized to 1, which tracks the position where the next unique element should be written.
- We iterate through the array with a 'readIndex' pointer starting from 1 to the end of the array.
- At each step, we compare the element at 'readIndex' with the element immediately preceding it ('readIndex - 1').
- If they are different, it means we have encountered a new unique element. We overwrite the element at 'writeIndex' with the unique element and increment 'writeIndex'.
- If they are the same, we simply skip the duplicate.
- The value of 'writeIndex' at the end represents the number of unique elements, and the first 'writeIndex' elements of the array will contain these unique items in order.

TIME COMPLEXITY: O(N) - We traverse the array of size N exactly once with a single loop.
SPACE COMPLEXITY: O(1) - The manipulation is performed directly within the input array without allocating extra heap space.
*/
int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    
    size_t writeIndex = 1;
    for (size_t readIndex = 1; readIndex < nums.size(); ++readIndex) {
        if (nums[readIndex] != nums[readIndex - 1]) {
            nums[writeIndex] = nums[readIndex];
            writeIndex++;
        }
    }
    return static_cast<int>(writeIndex);
}

int main() {
    int n;
    cout << "Enter the number of elements in the sorted array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " sorted integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    int uniqueCount = removeDuplicates(nums);
    
    cout << "\nNumber of unique elements: " << uniqueCount << endl;
    cout << "Array after removing duplicates: ";
    for (int i = 0; i < uniqueCount; ++i) {
        cout << nums[i] << " ";
    }
    cout << endl;
    
    return 0;
}