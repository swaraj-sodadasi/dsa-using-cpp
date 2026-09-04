#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SLIDING WINDOW MAXIMUM (MONOTONIC DEQUE INDEX MAX OPTIMIZATION)
     * -------------------------------------------------------------------------------------
     * The objective is to find the maximum element in each sliding window of size k.
     * 
     * 1. MONOTONIC DEQUE STRUCTURE:
     *    We store indices in a deque such that the corresponding elements in `nums` 
     *    are in monotonically decreasing order.
     * 
     * 2. O(1) MAXIMUM RETRIEVAL:
     *    - `dq.front()` always holds the index of the maximum element for the current window.
     * 
     * 3. WINDOW MAINTENANCE:
     *    - Expunge indices that fall out of the sliding window boundary (`dq.front() <= i - k`).
     *    - Maintain decreasing monotonicity: before pushing `i`, pop back indices whose 
     *      corresponding values are `<= nums[i]`.
     *    - Append `dq.front()` value to output once `i >= k - 1`.
     */
    vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
        vector<int> result;
        deque<int> dq; // Monotonically decreasing deque storing indices
        int n = static_cast<int>(nums.size());

        for (int i = 0; i < n; ++i) {
            // Remove indices that are out of the current sliding window boundary
            if (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }

            // Remove smaller elements from the back as they won't be max in current/future windows
            while (!dq.empty() && nums[static_cast<size_t>(dq.back())] <= nums[static_cast<size_t>(i)]) {
                dq.pop_back();
            }

            dq.push_back(i);

            // Record maximum once first window of size k is formed
            if (i >= k - 1) {
                result.push_back(nums[static_cast<size_t>(dq.front())]);
            }
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Each index is pushed and popped from the deque at most once.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Space required for sliding window deque of size at most K.
 */
int main() {
    cout << "=== Sliding Window Maximum Monotonic Deque Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter window size k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    vector<int> maxes = solver.maxSlidingWindow(nums, k);

    cout << "Sliding window maximums: [ ";
    for (int val : maxes) {
        cout << val << " ";
    }
    cout << "]\n";

    return 0;
}