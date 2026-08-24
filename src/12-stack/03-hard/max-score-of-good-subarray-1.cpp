#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 1: GREEDY BOUNDED EXPANSION (TWO POINTERS)
     * -------------------------------------------------------------------------------------
     * We start with both pointers at the mandatory index 'k'. 
     * In a greedy manner, we expand our window outwards. To maximize the score, we always 
     * choose to include the larger of the two adjacent elements (left or right).
     * This ensures that the minimum element of our current subarray decreases as slowly 
     * as possible, preserving the highest possible running score.
     * 
     * TIME COMPLEXITY: O(N) - Linear sweep expanding outward from k to the edges.
     * SPACE COMPLEXITY: O(1) Auxiliary - Constant memory overhead.
     */
    int maximumScoreGreedy(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        int left = k;
        int right = k;
        int currentMin = nums[static_cast<size_t>(k)];
        int maxScore = currentMin;

        while (left > 0 || right < n - 1) {
            int leftVal = (left > 0) ? nums[static_cast<size_t>(left - 1)] : 0;
            int rightVal = (right < n - 1) ? nums[static_cast<size_t>(right + 1)] : 0;

            if (leftVal > rightVal) {
                left--;
                currentMin = min(currentMin, nums[static_cast<size_t>(left)]);
            } else {
                right++;
                currentMin = min(currentMin, nums[static_cast<size_t>(right)]);
            }
            
            int currentWidth = right - left + 1;
            maxScore = max(maxScore, currentMin * currentWidth);
        }

        return maxScore;
    }

    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 2: MONOTONIC DEQUE/STACK (HISTOGRAM BOUNDARY INTERSECTIONS)
     * -------------------------------------------------------------------------------------
     * We can view the array as a histogram. We use a monotonic increasing stack to find 
     * the maximum rectangular area, identical to the "Largest Rectangle in Histogram" algorithm.
     * The key difference is a conditional filter: we only consider rectangles (subarrays) 
     * valid if their left and right boundaries encompass the mandatory index 'k'.
     * 
     * TIME COMPLEXITY: O(N) - Every element is pushed and popped at most once.
     * SPACE COMPLEXITY: O(N) Auxiliary - Stack depth scales linearly with the input layout.
     */
    int maximumScoreStack(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        stack<int> s;
        int maxScore = 0;

        // Virtual element 0 at index n flushes out all remaining stack nodes
        for (int i = 0; i <= n; ++i) {
            int currentHeight = (i == n) ? 0 : nums[static_cast<size_t>(i)];

            while (!s.empty() && nums[static_cast<size_t>(s.top())] > currentHeight) {
                int heightIdx = s.top();
                s.pop();

                int barHeight = nums[static_cast<size_t>(heightIdx)];
                int leftBoundaryIdx = s.empty() ? -1 : s.top();
                int rightBoundaryIdx = i;

                // Good subarray requirement: left_bound < k < right_bound
                if (leftBoundaryIdx + 1 <= k && k <= rightBoundaryIdx - 1) {
                    int currentWidth = rightBoundaryIdx - leftBoundaryIdx - 1;
                    maxScore = max(maxScore, barHeight * currentWidth);
                }
            }
            s.push(i);
        }

        return maxScore;
    }
};

int main() {
    cout << "=== Maximum Score of a Good Subarray Dual-Engine Console ===\n";
    cout << "Enter the total number of elements in the array: ";
    int length;
    if (!(cin >> length) || length <= 0) return 1;

    vector<int> nums(static_cast<size_t>(length));
    cout << "Enter the integer elements sequentially:\n";
    for (int i = 0; i < length; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    cout << "Enter the target index 'k' for the good subarray constraint: ";
    int k;
    if (!(cin >> k) || k < 0 || k >= length) {
        cout << "Error: Target index 'k' is out of array bounds." << endl;
        return 1;
    }

    Solution solver;

    cout << "\nExecuting Engine Pass 1 (Greedy Bounded Expansion Two Pointers)..." << endl;
    int outcome1 = solver.maximumScoreGreedy(nums, k);
    cout << "Maximum Good Subarray Score (Engine 1): [" << outcome1 << "]\n";

    cout << "\nExecuting Engine Pass 2 (Monotonic Stack Boundary Intersections)..." << endl;
    int outcome2 = solver.maximumScoreStack(nums, k);
    cout << "Maximum Good Subarray Score (Engine 2): [" << outcome2 << "]\n";

    return 0;
}