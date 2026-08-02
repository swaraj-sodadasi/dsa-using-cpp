#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * SLIDING WINDOW DIFF ARRAY EVENT TRACKING ENGINE
     * ------------------------------------------------
     * The objective is to return the minimum number of k-consecutive bit flips required 
     * so that there are no 0 elements left in the array. If it is impossible, return -1.
     *
     * Mathematical & Algorithmic Mechanics:
     * - A naive greedy simulation flips a window of size 'k' every time it sees a 0, 
     *   taking O(N * k) time. This violates strict linear boundaries when k is large.
     * - We optimize this to O(N) linear time by decoupling the *decision* to flip from the 
     *   *execution* of the flip. We use a difference array approach (or event queue tracker) 
     *   to maintain a running count of active flips affecting the current index.
     *
     * Event Tracking Mechanics:
     * - `currentlyActiveFlips`: Tracks the total number of flips whose window of influence 
     *   covers the current element `nums[i]`.
     * - If an element `nums[i]` is inside an odd number of active flip windows (`currentlyActiveFlips % 2 == 1`), 
     *   its value is effectively inverted: a 1 becomes a 0, and a 0 becomes a 1.
     * - If the actual state of `nums[i]` evaluates to 0, a new flip window must be opened 
     *   starting at index `i`. 
     *   - If `i + k > n`, the window extends past the array boundaries, making it impossible 
     *     to fix the remaining elements. We return -1 immediately.
     *   - Otherwise, we execute the flip, increment our total flip count, and mark the element 
     *     at `nums[i]` with a special flag (`2`) to remember that a flip window was opened here.
     * - When the index pointer advances to `i >= k`, a previous flip window might expire. We inspect 
     *   `nums[i - k]`. If it contains our flag (`2`), that flip window has closed, so we decrement 
     *   `currentlyActiveFlips`.
     */
    int minKBitFlips(vector<int>& nums, int k) {
        int totalFlipsExecuted = 0;
        int currentlyActiveFlips = 0;
        size_t n = nums.size();
        size_t windowSize = static_cast<size_t>(k);

        for (size_t i = 0; i < n; ++i) {
            // Expiry Step: If the sliding window has passed a flip anchor point, drop it from the active count
            if (i >= windowSize) {
                if (nums[i - windowSize] == 2) {
                    currentlyActiveFlips--;
                }
            }

            // Determine the true current bit value based on parity inversion tracking
            // If currentlyActiveFlips is odd, the bit is flipped. If even, it remains its original value.
            int currentBitState = (nums[i] + currentlyActiveFlips) % 2;

            if (currentBitState == 0) {
                // Out-of-bounds Check: If a window of size K cannot fit from here, the task is impossible
                if (i + windowSize > n) {
                    return -1;
                }

                // In-place marker assignment: mark this cell as the launchpad for a new flip window
                nums[i] = 2;
                currentlyActiveFlips++;
                totalFlipsExecuted++;
            }
        }

        return totalFlipsExecuted;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of bits in the input array.
 *                  The logic runs in a single forward pass, evaluating window constraints 
 *                  and performing state updates in constant O(1) time per element.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The status markers are recorded directly inside the input array 
 *                   by overwriting flipped states with a flag value, utilizing zero extra space on the heap.
 */
int main() {
    int n, k;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements (must be strictly 0 or 1) separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
        if (nums[static_cast<size_t>(i)] != 0 && nums[static_cast<size_t>(i)] != 1) {
            cout << "Constraint Error: Elements must be binary digits (0 or 1)." << endl;
            return 1;
        }
    }

    cout << "Enter consecutive flip window group constraint size (k): ";
    if (!(cin >> k) || k <= 0 || k > n) {
        cout << "Invalid window parameter assignment. Must satisfy 1 <= k <= array size." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting sliding window diff array event tracking pass..." << endl;
    int result = solver.minKBitFlips(nums, k);

    if (result != -1) {
        cout << "Minimum consecutive block flip operations required: " << result << endl;
    } else {
        cout << "Result: -1 (It is impossible to bring the array into a pure uniform all-1 state configuration)" << endl;
    }

    return 0;
}