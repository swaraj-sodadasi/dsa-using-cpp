#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (HASH MAP APPROACH - Optimal for Unsorted Arrays):
- We iterate through the array once while maintaining a hash map tracking element values and their indices.
- For each element 'nums[i]', we check if its complement ('target - nums[i]') already exists in the map.
- If it exists, we have found our pair and return their indices.
- If not, we insert the current element and its index into the map.

TIME COMPLEXITY: O(N) - We traverse the array of size N exactly once. Map lookups and insertions take O(1) amortized time.
SPACE COMPLEXITY: O(N) - In the worst case, the hash map stores up to N elements if no matching pair is found until the end.
*/
vector<int> twoSumHashMap(const vector<int>& nums, int target) {
    unordered_map<int, int> numMap;
    for (size_t i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];
        if (numMap.count(complement)) {
            return {numMap[complement], static_cast<int>(i)};
        }
        numMap[nums[i]] = static_cast<int>(i);
    }
    return {};
}

/*
CORE LOGIC (TWO-POINTER APPROACH - Optimal Space / Requires Sorting):
- Since the two-pointer approach assumes or requires sorted data, we store the original values alongside 
  their original indices, then sort this pair array based on the values.
- We initialize a 'left' pointer at the start (0) and a 'right' pointer at the end (size - 1).
- We compute the sum of the elements at these two pointers.
- If 'current_sum == target', we return the original stored indices.
- If 'current_sum < target', we increment 'left' to increase the sum.
- If 'current_sum > target', we decrement 'right' to decrease the sum.

TIME COMPLEXITY: O(N log N) - Dominated by the sorting step. The two-pointer traversal itself takes O(N) time.
SPACE COMPLEXITY: O(N) - Required to store the original indices alongside values for sorting. If only values were needed and the input was already sorted, space would be O(1).
*/
vector<int> twoSumTwoPointer(const vector<int>& nums, int target) {
    vector<pair<int, int>> indexedNums(nums.size());
    for (size_t i = 0; i < nums.size(); ++i) {
        indexedNums[i] = {nums[i], static_cast<int>(i)};
    }
    
    sort(indexedNums.begin(), indexedNums.end());
    
    size_t left = 0;
    size_t right = indexedNums.size() - 1;
    
    while (left < right) {
        int currentSum = indexedNums[left].first + indexedNums[right].first;
        if (currentSum == target) {
            return {indexedNums[left].second, indexedNums[right].second};
        } else if (currentSum < target) {
            left++;
        } else {
            right--;
        }
    }
    return {};
}

int main() {
    int n, target;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    cout << "Enter the target sum integer: ";
    cin >> target;
    
    vector<int> hashResult = twoSumHashMap(nums, target);
    cout << "\n[Hash Map Approach] Indices: ";
    if (!hashResult.empty()) {
        cout << hashResult[0] << ", " << hashResult[1] << endl;
    } else {
        cout << "No solution found." << endl;
    }
    
    vector<int> pointerResult = twoSumTwoPointer(nums, target);
    cout << "[Two-Pointer Approach] Indices: ";
    if (!pointerResult.empty()) {
        cout << pointerResult[0] << ", " << pointerResult[1] << endl;
    } else {
        cout << "No solution found." << endl;
    }
    
    return 0;
}