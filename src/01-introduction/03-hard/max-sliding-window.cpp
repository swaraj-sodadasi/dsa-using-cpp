#include <iostream>
#include <vector>
#include <deque>

using namespace std;

/*
CORE LOGIC (MONOTONIC DEQUE OPTIMIZATION):
- We maintain a double-ended queue (deque) that stores indices of array elements. 
- The deque is kept monotonically decreasing in terms of the elements at those indices. That is, the element corresponding to the front index is always the maximum inside the current window.
- For each element `nums[i]` as the window slides forward:
  1. **Evict Out-of-Window Indices**: If the index at the front of the deque falls outside the current window boundary (`deque.front() <= i - k`), pop it from the front.
  2. **Maintain Monotonicity**: While the deque is not empty and the current element `nums[i]` is strictly greater than or equal to the element at the back index (`nums[deque.back()]`), pop from the back. Elements smaller than `nums[i]` can never be the maximum for any future window layout.
  3. **Insert Current Index**: Push the current index `i` onto the back of the deque.
- Once the index reaches `k - 1` (the first complete window), the element at the front index of the deque is recorded into the result vector.

TIME COMPLEXITY: O(N) - Each element's index is pushed onto the deque exactly once and popped at most once during the linear pass over the array of size N.
SPACE COMPLEXITY: O(K) - The deque stores at most K indices corresponding to elements inside the current sliding window.
*/
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    vector<int> result;
    if (nums.empty() || k == 0) return result;
    
    deque<size_t> dq;
    result.reserve(nums.size() - static_cast<size_t>(k) + 1);

    for (size_t i = 0; i < nums.size(); ++i) {
        // 1. Remove indices that are out of the current window boundary
        if (!dq.empty() && dq.front() + static_cast<size_t>(k) <= i) {
            dq.pop_front();
        }

        // 2. Remove elements smaller than the current element from the back
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }

        // 3. Add current element's index to the back
        dq.push_back(i);

        // 4. Append maximum element of the window to result once window size k is reached
        if (i >= static_cast<size_t>(k) - 1) {
            result.push_back(nums[dq.front()]);
        }
    }

    return result;
}

int main() {
    int n, k;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    cout << "Enter the sliding window size k: ";
    cin >> k;

    vector<int> result = maxSlidingWindow(nums, k);

    cout << "\nMaximum elements in each sliding window:\n";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}