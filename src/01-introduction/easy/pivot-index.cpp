#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

/*
CORE LOGIC (PREFIX SUM EQUILIBRIUM):
- The pivot index is the index where the sum of all numbers strictly to the left of the index is equal to the sum of all numbers strictly to the right of the index.
- We calculate the total sum of all elements in the array using `std::accumulate`.
- We maintain a running `leftSum` variable initialized to 0 as we iterate through the array.
- For each index `i`, the sum of elements to the right can be derived mathematically as: `rightSum = totalSum - leftSum - nums[i]`.
- If `leftSum == rightSum`, then index `i` is the equilibrium point, and we return it immediately.
- If not, we add the current element `nums[i]` to `leftSum` and proceed to the next iteration.
- If no such index exists, we return -1.

TIME COMPLEXITY: O(N) - One pass to compute the total sum and a second pass to locate the pivot index.
SPACE COMPLEXITY: O(1) - Only a few primitive scalar tracking variables are allocated.
*/
int pivotIndex(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    int leftSum = 0;
    
    for (size_t i = 0; i < nums.size(); ++i) {
        if (leftSum == totalSum - leftSum - nums[i]) {
            return static_cast<int>(i);
        }
        leftSum += nums[i];
    }
    
    return -1;
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
    
    int result = pivotIndex(nums);
    
    if (result != -1) {
        cout << "\nThe pivot index is: " << result << endl;
    } else {
        cout << "\nNo pivot index exists." << endl;
    }
    
    return 0;
}