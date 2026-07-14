#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (GREEDY TWO-POINTER EXPANSION):
- We use two pointers, 'left' at the beginning (0) and 'right' at the end (size - 1) of the height array.
- The width of the container is the distance between the two pointers: `right - left`.
- The height of the water is constrained by the shorter of the two vertical lines: `min(height[left], height[right])`.
- The area is calculated as: `currentArea = width * height`.
- To maximize the area, we track the maximum area seen so far.
- We then use a greedy strategy to move the pointers: we shift the pointer pointing to the shorter line inward.
- This is because moving the pointer pointing to the longer line would only decrease the width while the height remains bounded by the shorter line, which can never yield a larger area.
- The process repeats until the two pointers meet.

TIME COMPLEXITY: O(N) - We perform a single linear traversal inward, visiting each element at most once.
SPACE COMPLEXITY: O(1) - Only primitive integer tracking variables are allocated.
*/
int maxArea(const vector<int>& height) {
    int maxWater = 0;
    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    
    while (left < right) {
        int currentWidth = right - left;
        int currentHeight = min(height[static_cast<size_t>(left)], height[static_cast<size_t>(right)]);
        int currentArea = currentWidth * currentHeight;
        
        maxWater = max(maxWater, currentArea);
        
        if (height[static_cast<size_t>(left)] < height[static_cast<size_t>(right)]) {
            left++;
        } else {
            right--;
        }
    }
    
    return maxWater;
}

int main() {
    int n;
    cout << "Enter the number of vertical lines: ";
    cin >> n;
    
    vector<int> height(n);
    cout << "Enter " << n << " non-negative integers representing heights separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> height[i];
    }
    
    int result = maxArea(height);
    cout << "\nThe maximum volume of water the container can store is: " << result << endl;
    
    return 0;
}