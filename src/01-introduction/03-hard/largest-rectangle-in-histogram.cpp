#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (MONOTONIC STACK BOUNDARY EXPANSION):
- For each bar in the histogram, we want to find the maximum area of a rectangle where this bar is the shortest (the bottleneck).
- To do this, we need to find the boundary of this bar: the first bar to its left that is shorter than it, and the first bar to its right that is shorter than it.
- We maintain a monotonic increasing stack that stores the indices of the bars.
- We iterate through the histogram from left to right. For each bar `heights[i]`:
  - While the stack is not empty and the current bar's height is strictly less than the height of the bar at the index stored at the top of the stack (`heights[stack.top()]`), it means we have found the right boundary for the bar at `stack.top()`.
  - We pop the index from the stack; let this index be `mid`. The height of the rectangle is `heights[mid]`.
  - The right boundary is the current index `i`.
  - The left boundary is the new index at the top of the stack (if empty, it means all bars to the left are taller, so the left boundary is effectively -1).
  - The width of the rectangle is calculated as: `width = i - stack.top() - 1`.
  - The area is `heights[mid] * width`. We update our global maximum area.
- To handle the remaining bars left in the stack after processing all elements, we append a dummy bar of height 0 at the end of our loop or explicitly process them. Adding a height of 0 ensures all remaining elements are popped out.

TIME COMPLEXITY: O(N) - Each element is pushed onto the stack exactly once and popped at most once.
SPACE COMPLEXITY: O(N) - In the worst-case scenario (monotonically increasing heights), the stack will store all N indices.
*/
int largestRectangleArea(vector<int>& heights) {
    stack<int> st;
    int maxArea = 0;
    
    // Append a dummy bar of height 0 to clear the stack completely at the end
    heights.push_back(0);
    
    for (int i = 0; i < static_cast<int>(heights.size()); ++i) {
        while (!st.empty() && heights[static_cast<size_t>(i)] < heights[static_cast<size_t>(st.top())]) {
            int height = heights[static_cast<size_t>(st.top())];
            st.pop();
            
            // If stack is empty, width spans from index 0 up to i
            int width = st.empty() ? i : (i - st.top() - 1);
            
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    
    // Restore the original array state
    heights.pop_back();
    
    return maxArea;
}

int main() {
    int n;
    cout << "Enter the number of bars in the histogram: ";
    cin >> n;

    vector<int> heights(n);
    cout << "Enter " << n << " non-negative integers representing heights separated by space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> heights[i];
    }

    int result = largestRectangleArea(heights);
    cout << "\nThe area of the largest rectangle in the histogram is: " << result << endl;

    return 0;
}