#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (SINGLE-PASS CLIMBING TRACKING):
- A valid mountain array must strictly increase to a single peak and then strictly decrease.
- It requires a minimum size of 3 elements.
- We simulate climbing the mountain using a single pointer 'i' starting at index 0.
- Step 1: Walk up the mountain by moving 'i' forward as long as the next element is strictly greater than the current element (`nums[i] < nums[i + 1]`).
- Step 2: Validate the peak. The peak cannot be the first element (meaning it never climbed) and it cannot be the last element (meaning it never descended). Therefore, `i` must be greater than 0 and less than `size - 1`.
- Step 3: Walk down the mountain by moving 'i' forward as long as the next element is strictly smaller than the current element (`nums[i] > nums[i + 1]`).
- If the array is a valid mountain, the pointer 'i' will reach the final index (`size - 1`).

TIME COMPLEXITY: O(N) - We perform a single linear traversal over the array of size N.
SPACE COMPLEXITY: O(1) - Only a scalar iterator tracking index variable is maintained.
*/
bool validMountainArray(const vector<int>& nums) {
    size_t n = nums.size();
    if (n < 3) return false;

    size_t i = 0;

    // Walk up
    while (i + 1 < n && nums[i] < nums[i + 1]) {
        i++;
    }

    // Peak cannot be the first or last element
    if (i == 0 || i == n - 1) {
        return false;
    }

    // Walk down
    while (i + 1 < n && nums[i] > nums[i + 1]) {
        i++;
    }

    return i == n - 1;
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

    bool result = validMountainArray(nums);

    if (result) {
        cout << "\nThe array is a valid mountain array." << endl;
    } else {
        cout << "\nThe array is NOT a valid mountain array." << endl;
    }

    return 0;
}