#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 1: MONOTONIC STACK BOUNDARY HEIGHT INTERCEPTIONS
     * -------------------------------------------------------------------------------------
     * We maintain a decreasing stack of indices. When the current bar height exceeds the 
     * height at the top of the stack, it means we have found a right boundary that can trap 
     * water over the preceding elements.
     * 
     * We pop the top index as the floor of the container (`mid`). If the stack is now empty, 
     * there is no left boundary, so no water can be trapped. Otherwise, the new top of the 
     * stack is the left boundary. The trapped water volume is calculated by multiplying the 
     * bounded width by the intercepted height delta:
     *   - Bounded Width = current_index - left_boundary_index - 1
     *   - Intercepted Height = min(height[left], height[right]) - height[mid]
     * 
     * TIME COMPLEXITY: O(N) - Linear scan where each index is pushed and popped at most once.
     * SPACE COMPLEXITY: O(N) - Stack tracks decreasing height indices.
     */
    int trapStack(const vector<int>& height) {
        stack<int> boundaryStack;
        int totalWater = 0;
        int n = static_cast<int>(height.size());

        for (int i = 0; i < n; ++i) {
            while (!boundaryStack.empty() && height[static_cast<size_t>(i)] > height[static_cast<size_t>(boundaryStack.top())]) {
                int midIdx = boundaryStack.top();
                boundaryStack.pop();

                if (boundaryStack.empty()) {
                    break;
                }

                int leftIdx = boundaryStack.top();
                int boundedWidth = i - leftIdx - 1;
                int interceptedHeight = min(height[static_cast<size_t>(leftIdx)], height[static_cast<size_t>(i)]) - height[static_cast<size_t>(midIdx)];
                
                totalWater += boundedWidth * interceptedHeight;
            }
            boundaryStack.push(i);
        }

        return totalWater;
    }

    /*
     * -------------------------------------------------------------------------------------
     * APPROACH 2: TWO-POINTER CONSTANT MEMORY MATRIX (O(1) AUX SPACE)
     * -------------------------------------------------------------------------------------
     * We use two cursors (`left` and `right`) initialized at both ends of the array, 
     * moving inward. We maintain two running maximums (`leftMax` and `rightMax`).
     * 
     * Since the lower boundary dictates the maximum water volume trapped, we compress our 
     * logic: if `height[left] < height[right]`, we know a taller boundary exists on the right. 
     * If the current `height[left]` is >= `leftMax`, we update `leftMax`; otherwise, it can 
     * hold `leftMax - height[left]` amount of water. We then advance the left pointer. 
     * We apply the inverse logic if `height[right] <= height[left]`.
     * 
     * TIME COMPLEXITY: O(N) - Single linear sweep tracking array positions inward.
     * SPACE COMPLEXITY: O(1) Auxiliary - Eliminates extra space overhead.
     */
    int trapTwoPointers(const vector<int>& height) {
        if (height.empty()) return 0;

        int left = 0;
        int right = static_cast<int>(height.size()) - 1;
        int leftMax = 0;
        int rightMax = 0;
        int totalWater = 0;

        while (left < right) {
            if (height[static_cast<size_t>(left)] < height[static_cast<size_t>(right)]) {
                if (height[static_cast<size_t>(left)] >= leftMax) {
                    leftMax = height[static_cast<size_t>(left)];
                } else {
                    totalWater += leftMax - height[static_cast<size_t>(left)];
                }
                left++;
            } else {
                if (height[static_cast<size_t>(right)] >= rightMax) {
                    rightMax = height[static_cast<size_t>(right)];
                } else {
                    totalWater += rightMax - height[static_cast<size_t>(right)];
                }
                right--;
            }
        }

        return totalWater;
    }
};

int main() {
    cout << "=== Trapping Rain Water Dual-Engine Console ===\n";
    cout << "Enter the total number of terrain coordinate bars: ";
    int barCount;
    if (!(cin >> barCount) || barCount < 0) return 1;

    vector<int> heightMap(static_cast<size_t>(barCount));
    if (barCount > 0) {
        cout << "Enter the raw integer bar heights sequentially:\n";
        for (int i = 0; i < barCount; ++i) {
            cin >> heightMap[static_cast<size_t>(i)];
        }
    }

    Solution solver;

    cout << "\nExecuting Engine Pass 1 (Monotonic Stack Boundary Interceptions)..." << endl;
    int waterOutcome1 = solver.trapStack(heightMap);
    cout << "Calculated Trapped Water (Engine 1): [" << waterOutcome1 << "]\n";

    cout << "\nExecuting Engine Pass 2 (Two-Pointer Convergence)..." << endl;
    int waterOutcome2 = solver.trapTwoPointers(heightMap);
    cout << "Calculated Trapped Water (Engine 2): [" << waterOutcome2 << "]\n";

    return 0;
}