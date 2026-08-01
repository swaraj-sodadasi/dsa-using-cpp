#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * GREEDY ADJACENT ELEMENT INSPECTION ENGINE
     * -----------------------------------------
     * The problem asks if we can plant 'n' new flowers in a flowerbed without violating 
     * the no-adjacent-flowers rule (no two flowers can be planted in adjacent plots).
     *
     * Architectural Tracking Mechanics:
     * - We can solve this optimally using a greedy approach. We scan the flowerbed from 
     *   left to right, and whenever we encounter an empty plot, we check its left and 
     *   right neighbors.
     * - If both neighbors are also empty (or if the plot is at the boundary edge of the 
     *   array where a neighbor doesn't exist), we can safely plant a flower there.
     * - By planting greedily at the first available valid opportunity, we maximize the 
     *   remaining space left for subsequent placements.
     *
     * State Window Evaluation Sequence:
     * - Loop through the flowerbed array using index tracker `i`.
     * - For each plot, check if `flowerbed[i] == 0`.
     * - If empty, verify the left neighbor: `i == 0 || flowerbed[i - 1] == 0`.
     * - Verify the right neighbor: `i == size - 1 || flowerbed[i + 1] == 0`.
     * - If both conditions hold true, we plant a flower by setting `flowerbed[i] = 1`, 
     *   and decrement our target counter `n`.
     * - Early Pruning: If `n <= 0` at any point, all required flowers have been planted 
     *   successfully, allowing us to terminate early and return true.
     */
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        // Early baseline validation check: if no flowers need planting, we are done
        if (n <= 0) {
            return true;
        }

        size_t size = flowerbed.size();

        for (size_t i = 0; i < size; ++i) {
            if (flowerbed[i] == 0) {
                // Check if the left plot is empty or if we are at the left boundary edge
                bool leftEmpty = (i == 0 || flowerbed[i - 1] == 0);
                
                // Check if the right plot is empty or if we are at the right boundary edge
                bool rightEmpty = (i == size - 1 || flowerbed[i + 1] == 0);

                if (leftEmpty && rightEmpty) {
                    // Commit placement step change and decrement the remaining requirement counter
                    flowerbed[i] = 1;
                    n--;

                    // OPTIMIZATION PRUNING: Terminate early if target placement is reached
                    if (n <= 0) {
                        return true;
                    }
                }
            }
        }

        return n <= 0;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of plots in the flowerbed.
 *                  The logic performs a single linear pass over the input sequence,
 *                  evaluating neighboring plots in constant O(1) time per step.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The evaluation mutates the input structure in place,
 *                   requiring no extra dynamic memory tracking frameworks on the heap.
 */
int main() {
    int size, n;
    cout << "Enter the total number of plots in the flowerbed: ";
    if (!(cin >> size) || size <= 0) {
        cout << "Invalid parameter. The flowerbed size must be greater than 0." << endl;
        return 1;
    }

    vector<int> flowerbed(static_cast<size_t>(size));
    cout << "Enter plot states (0 for empty, 1 for occupied) separated by spaces:\n";
    for (int i = 0; i < size; ++i) {
        cin >> flowerbed[static_cast<size_t>(i)];
        if (flowerbed[static_cast<size_t>(i)] != 0 && flowerbed[static_cast<size_t>(i)] != 1) {
            cout << "Constraint Error: Input values must be either 0 or 1." << endl;
            return 1;
        }
    }

    cout << "Enter the number of new flowers to plant (n): ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid target flower count parameters configuration." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting greedy adjacent element inspection sweep..." << endl;
    
    if (solver.canPlaceFlowers(flowerbed, n)) {
        
        cout << "Result: True! The requested number of flowers can be planted legally." << endl;
    } else {
        cout << "Result: False! The requested number of flowers breaks adjacency constraints." << endl;
    }

    return 0;
}