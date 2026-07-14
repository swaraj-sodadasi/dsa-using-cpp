#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (DIVIDE AND CONQUER INVERSION COUNTING VIA MERGE SORT):
- A reverse pair is defined as an index pair (i, j) such that i < j and nums[i] > 2 * nums[j].
- We use a modified Merge Sort algorithm to count these pairs efficiently. During the divide step, the array is split into left and right sorted halves.
- Before merging the two sorted halves, we count the reverse pairs formed across them:
  - We maintain a pointer 'j' in the right half. For each element 'i' in the left half, we advance 'j' as long as `nums[i] > 2LL * nums[j]` (using long long to prevent integer overflow).
  - Since both halves are sorted, as 'i' moves forward, the threshold increases, meaning 'j' does not need to reset. This allows us to count pairs in linear time for that merge step.
- After counting, we perform the standard merge step to maintain the sorted order for higher levels of recursion.

TIME COMPLEXITY: O(N log N) - The array is split log N times, and at each level, the counting and merging operations take O(N) time.
SPACE COMPLEXITY: O(N) - An auxiliary buffer vector of size N is utilized during the merge step to store elements temporarily.
*/

void mergeAndCount(vector<int>& nums, int start, int mid, int end, int& reversePairsCount) {
    int j = mid + 1;
    for (int i = start; i <= mid; ++i) {
        while (j <= end && nums[static_cast<size_t>(i)] > 2LL * nums[static_cast<size_t>(j)]) {
            j++;
        }
        reversePairsCount += (j - (mid + 1));
    }

    // Standard Merge operation
    vector<int> temp;
    temp.reserve(static_cast<size_t>(end - start + 1));
    int left = start, right = mid + 1;

    while (left <= mid && right <= end) {
        if (nums[static_cast<size_t>(left)] <= nums[static_cast<size_t>(right)]) {
            temp.push_back(nums[static_cast<size_t>(left++)]);
        } else {
            temp.push_back(nums[static_cast<size_t>(right++)]);
        }
    }

    while (left <= mid) {
        temp.push_back(nums[static_cast<size_t>(left++)]);
    }
    while (right <= end) {
        temp.push_back(nums[static_cast<size_t>(right++)]);
    }

    for (int i = 0; i < static_cast<int>(temp.size()); ++i) {
        nums[static_cast<size_t>(start + i)] = temp[static_cast<size_t>(i)];
    }
}

void mergeSort(vector<int>& nums, int start, int end, int& reversePairsCount) {
    if (start >= end) return;

    int mid = start + (end - start) / 2;
    mergeSort(nums, start, mid, reversePairsCount);
    mergeSort(nums, mid + 1, end, reversePairsCount);
    mergeAndCount(nums, start, mid, end, reversePairsCount);
}

int reversePairs(vector<int>& nums) {
    int reversePairsCount = 0;
    mergeSort(nums, 0, static_cast<int>(nums.size()) - 1, reversePairsCount);
    return reversePairsCount;
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

    int result = reversePairs(nums);
    cout << "\nTotal number of reverse pairs: " << result << endl;

    return 0;
}