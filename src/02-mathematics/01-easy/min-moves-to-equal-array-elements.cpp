#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (MATHEMATICAL REDUCTION / MIN ELEMENT ANCHOR):
- The problem asks for the minimum moves to make all array elements equal, where a move consists of incrementing n - 1 elements by 1.
- Incrementing n - 1 elements by 1 is mathematically equivalent to *decrementing* a single element by 1 relative to the rest of the array.
- Therefore, to make all elements equal with the minimum number of changes, our target value for all elements must be the minimum element currently in the array. Any higher target value would require unnecessary extra operations.
- The problem reduces to finding the sum of the differences between each element and the absolute minimum element in the array:
  Total Moves = Sum(nums[i] - minElement) for all i
- This can be rewritten as: Total Moves = Sum(nums[i]) - (n * minElement).

TIME COMPLEXITY: O(N) - We perform a single linear pass over the array of size N to find both the minimum element and the total sum.
SPACE COMPLEXITY: O(1) - The calculation runs in-place using scalar variables, satisfying the constant space constraint.
*/
int minMoves(const vector<int>& nums) {
    if (nums.empty()) return 0;

    int minElement = nums[0];
    long long totalSum = 0; // Using long long to prevent integer overflow during summation

    for (int num : nums) {
        if (num < minElement) {
            minElement = num;
        }
        totalSum += num;
    }

    // Total Moves = totalSum - (n * minElement)
    return static_cast<int>(totalSum - static_cast<long long>(nums.size()) * minElement);
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid array size." << endl;
        return 1;
    }

    vector<int> nums(n);
    cout << "Enter " << n << " integers (can include negatives) separated by space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    int result = minMoves(nums);
    cout << "\nThe minimum number of moves required to make all elements equal is: " << result << endl;

    return 0;
}