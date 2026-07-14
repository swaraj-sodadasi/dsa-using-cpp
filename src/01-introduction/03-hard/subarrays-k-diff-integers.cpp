#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/*
CORE LOGIC (EXACT K VIA AT-MOST-K WINDOW DIFFERENCE):
- Finding subarrays with exactly K distinct integers directly using a sliding window is difficult because the window state does not change monotonically when expanding or shrinking.
- Instead, we transform the problem: `Exact(K) = AtMost(K) - AtMost(K - 1)`.
- The number of subarrays with *at most* K distinct integers can be calculated efficiently using a standard sliding window. For a fixed right boundary `right`, if the window from `left` to `right` contains at most K distinct elements, then any subarray ending at `right` and starting anywhere from `left` to `right` is also valid. The count of such subarrays is exactly `right - left + 1`.
- We implement a helper function `atMostK(nums, k)` that maintains a frequency map of elements within the window. We expand the window using the `right` pointer. If the number of unique elements exceeds `k`, we shrink the window from the left until the count of unique elements returns to `k` or fewer.
- The difference between `atMostK(nums, k)` and `atMostK(nums, k - 1)` gives the exact number of subarrays containing exactly K distinct integers.

TIME COMPLEXITY: O(N) - The `atMostK` helper function performs a single linear scan over the array of size N where both `left` and `right` pointers move forward monotonically. We call this helper twice, maintaining overall O(N) time complexity.
SPACE COMPLEXITY: O(N) - In the worst case, the frequency hash map will store up to N distinct elements inside the window.
*/

int atMostK(const vector<int>& nums, int k) {
    unordered_map<int, int> countMap;
    int left = 0;
    int totalSubarrays = 0;
    int distinctElementsCount = 0;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        if (countMap[nums[static_cast<size_t>(right)]] == 0) {
            distinctElementsCount++;
        }
        countMap[nums[static_cast<size_t>(right)]]++;

        while (distinctElementsCount > k) {
            countMap[nums[static_cast<size_t>(left)]]--;
            if (countMap[nums[static_cast<size_t>(left)]] == 0) {
                distinctElementsCount--;
            }
            left++;
        }

        totalSubarrays += (right - left + 1);
    }

    return totalSubarrays;
}

int subarraysWithKDistinct(const vector<int>& nums, int k) {
    return atMostK(nums, k) - atMostK(nums, k - 1);
}

int main() {
    int n, k;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter " << n << " positive integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    cout << "Enter the target number of distinct integers K: ";
    cin >> k;

    int result = subarraysWithKDistinct(nums, k);
    cout << "\nTotal number of subarrays with exactly " << k << " distinct integers: " << result << endl;

    return 0;
}