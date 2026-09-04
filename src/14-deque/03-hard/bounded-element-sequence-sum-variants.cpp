#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <climits>

using namespace std;

class SegmentTree {
private:
    int size;
    vector<long long> tree;

    void build(const vector<long long>& arr, int node, int start, int end) {
        if (start == end) {
            tree[static_cast<size_t>(node)] = arr[static_cast<size_t>(start)];
            return;
        }
        int mid = start + (end - start) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[static_cast<size_t>(node)] = max(tree[static_cast<size_t>(2 * node)], tree[static_cast<size_t>(2 * node + 1)]);
    }

    void update(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[static_cast<size_t>(node)] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[static_cast<size_t>(node)] = max(tree[static_cast<size_t>(2 * node)], tree[static_cast<size_t>(2 * node + 1)]);
    }

    long long query(int node, int start, int end, int l, int r) const {
        if (r < start || end < l) {
            return LLONG_MIN / 2;
        }
        if (l <= start && end <= r) {
            return tree[static_cast<size_t>(node)];
        }
        int mid = start + (end - start) / 2;
        long long leftMax = query(2 * node, start, mid, l, r);
        long long rightMax = query(2 * node + 1, mid + 1, end, l, r);
        return max(leftMax, rightMax);
    }

public:
    SegmentTree(int n) : size(n) {
        tree.assign(static_cast<size_t>(4 * n), LLONG_MIN / 2);
    }

    void build(const vector<long long>& arr) {
        if (size > 0) build(arr, 1, 0, size - 1);
    }

    void update(int idx, long long val) {
        if (size > 0) update(1, 0, size - 1, idx, val);
    }

    long long query(int l, int r) const {
        if (size <= 0 || l > r) return LLONG_MIN / 2;
        return query(1, 0, size - 1, l, r);
    }
};

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BOUNDED ELEMENT SEQUENCE SUM VARIANTS (SEGMENT-TREE MONOTONIC DEQUE)
     * -------------------------------------------------------------------------------------
     * Computes maximum valid sequence sum bounded by sliding window range [minLen, maxLen] 
     * and dynamic point value constraints.
     * 
     * 1. DP & SLIDING WINDOW TRANSITION:
     *    `dp[i] = val[i] + max(0, max(dp[j]))` for `j` in `[i - maxLen, i - minLen]`.
     * 
     * 2. MONOTONIC DEQUE & SEGMENT TREE INTEGRATION:
     *    - Segment Tree maintains range maximum DP transitions over dynamic updates.
     *    - Monotonic Deque maintains sliding window boundary constraint filtering.
     */
    long long maxBoundedSequenceSum(const vector<int>& nums, int minLen, int maxLen) {
        int n = static_cast<int>(nums.size());
        vector<long long> dp(static_cast<size_t>(n), LLONG_MIN / 2);
        SegmentTree segTree(n);

        deque<int> dq; // Sliding window monotonic deque tracking max DP states
        long long maxOverallSum = LLONG_MIN / 2;

        for (int i = 0; i < n; ++i) {
            // Add valid eligible candidate index (i - minLen) to window
            int addIdx = i - minLen;
            if (addIdx >= 0) {
                long long candidateVal = (addIdx == 0) ? max(0LL, (long long)nums[0]) : dp[static_cast<size_t>(addIdx)];
                segTree.update(addIdx, candidateVal);

                while (!dq.empty() && dp[static_cast<size_t>(dq.back())] <= candidateVal) {
                    dq.pop_back();
                }
                dq.push_back(addIdx);
            }

            // Remove candidate indices exceeding maximum length boundary (i - maxLen)
            while (!dq.empty() && dq.front() < i - maxLen) {
                dq.pop_front();
            }

            // Transition DP computation using integrated Segment Tree & Monotonic Deque
            long long maxPrev = 0;
            if (i >= minLen) {
                int leftBound = max(0, i - maxLen);
                int rightBound = i - minLen;
                maxPrev = segTree.query(leftBound, rightBound);
            }

            if (i < minLen) {
                dp[static_cast<size_t>(i)] = nums[static_cast<size_t>(i)];
            } else {
                dp[static_cast<size_t>(i)] = nums[static_cast<size_t>(i)] + max(0LL, maxPrev);
            }

            maxOverallSum = max(maxOverallSum, dp[static_cast<size_t>(i)]);
        }

        return maxOverallSum;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Segment tree query and point update per array element.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Segment tree nodes, DP table, and monotonic deque.
 */
int main() {
    cout << "=== Bounded Element Sequence Sum Segment-Tree Monotonic Deque Engine ===\n";
    cout << "Enter array size: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter minimum bounded length minLen: ";
    int minLen;
    if (!(cin >> minLen) || minLen <= 0) return 1;

    cout << "Enter maximum bounded length maxLen: ";
    int maxLen;
    if (!(cin >> maxLen) || maxLen < minLen) return 1;

    Solution solver;
    long long result = solver.maxBoundedSequenceSum(nums, minLen, maxLen);

    cout << "Maximum bounded sequence sum: [" << result << "]\n";

    return 0;
}