#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (INDEX-AS-KEY PLACEMENT / CYCLIC SORTING):
- The first missing positive integer must fall within the range [1, N + 1], where N is the size of the array.
- We treat the array itself as a cyclic hash map. We place each valid positive integer 'X' at its target index 'X - 1'.
- We iterate through the array. For each element `nums[i]`, while `nums[i]` is in the range [1, N] and `nums[i]` is not at its correct position (`nums[i] != nums[nums[i] - 1]`), we swap `nums[i]` with the element at its target index.
- After arranging the elements, we make a second pass through the array. The first index `i` where `nums[i] != i + 1` reveals the missing number, which is `i + 1`.
- If all indices hold their correct values, the missing number is `N + 1`.

TIME COMPLEXITY: O(N) - Although there is a nested while loop, each swap places at least one element into its final correct position. Since an element can be swapped at most once into its final position, the total number of swaps is bounded by N, resulting in an overall linear time complexity.
SPACE COMPLEXITY: O(1) - The array elements are reorganized entirely in-place, using zero auxiliary heap memory.
*/
int firstMissingPositive(vector<int>& nums) {
    int n = static_cast<int>(nums.size());

    for (int i = 0; i < n; ++i) {
        while (nums[i] > 0 && nums[i] <= n && nums[i] != nums[nums[i] - 1]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }

    return n + 1;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter " << n << " integers (can include negatives and zeroes) separated by space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    int result = firstMissingPositive(nums);
    cout << "\nThe first missing positive integer is: " << result << endl;

    return 0;
}