#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (TWO-POINTER APPROACH - Optimal O(1) Space):
- We maintain two pointers, 'left' at index 0 and 'right' at 'size - 1'.
- We also maintain two variables, 'leftMax' and 'rightMax', tracking the maximum heights seen from either direction.
- At each step, we process the pointer with the smaller height value. Why? Because the amount of water trapped is bounded by the shorter wall. If height[left] <= height[right], the height of water at 'left' is completely determined by 'leftMax', independent of 'rightMax'.
- If the current height at the pointer is greater than or equal to its maximum boundary, we update the boundary. Otherwise, the water trapped at that specific index is `boundaryMax - height[pointer]`.
- We move the processed pointer inward and repeat until they meet.

TIME COMPLEXITY: O(N) - Single pass over the array of size N using two pointers.
SPACE COMPLEXITY: O(1) - Uses only primitive scalar variables for tracking boundaries and pointers.
*/
int trapTwoPointer(const vector<int>& height) {
    if (height.empty()) return 0;

    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    int leftMax = 0;
    int rightMax = 0;
    int totalWater = 0;

    while (left < right) {
        if (height[static_cast<size_t>(left)] <= height[static_cast<size_t>(right)]) {
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

/*
CORE LOGIC (MONOTONIC STACK APPROACH):
- We maintain a monotonic decreasing stack that stores the indices of the bars.
- We iterate through the array with a current pointer.
- While the stack is not empty and the current bar height is strictly greater than the bar height at the top of the stack:
  - The bar at the top of the stack represents a valley (bottom) container. We pop it out: `int mid = stack.top(); stack.pop();`
  - If the stack becomes empty, there is no left boundary to trap water, so we break out.
  - If a left boundary exists, the left boundary index is the new `stack.top()`.
  - The bounded width between the left and right walls is: `distance = current_index - stack.top() - 1`.
  - The bounded height is calculated as: `boundedHeight = min(height[current_index], height[stack.top()]) - height[mid]`.
  - We accumulate `distance * boundedHeight` into our total trapped water.
- We push the current index onto the stack and continue the traversal.

TIME COMPLEXITY: O(N) - Each index is pushed onto the stack exactly once and popped at most once.
SPACE COMPLEXITY: O(N) - In the worst case (monotonically decreasing profile), the stack can store up to N indices.
*/
int trapMonotonicStack(const vector<int>& height) {
    stack<int> st;
    int totalWater = 0;
    
    for (int i = 0; i < static_cast<int>(height.size()); ++i) {
        while (!st.empty() && height[static_cast<size_t>(i)] > height[static_cast<size_t>(st.top())]) {
            int mid = st.top();
            st.pop();
            
            if (st.empty()) break;
            
            int leftBoundaryIdx = st.top();
            int distance = i - leftBoundaryIdx - 1;
            int boundedHeight = min(height[static_cast<size_t>(i)], height[static_cast<size_t>(leftBoundaryIdx)]) - height[static_cast<size_t>(mid)];
            
            totalWater += distance * boundedHeight;
        }
        st.push(i);
    }
    return totalWater;
}

int main() {
    int n;
    cout << "Enter the number of bars in the elevation map: ";
    cin >> n;

    vector<int> height(n);
    cout << "Enter " << n << " non-negative integers representing bar heights separated by space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> height[i];
    }

    int waterTwoPointer = trapTwoPointer(height);
    int waterStack = trapMonotonicStack(height);

    cout << "\n[Two-Pointer Approach] Total water trapped: " << waterTwoPointer << endl;
    cout << "[Monotonic Stack Approach] Total water trapped: " << waterStack << endl;

    return 0;
}