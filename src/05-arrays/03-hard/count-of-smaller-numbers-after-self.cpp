#include <iostream>
#include <vector>

using namespace std;

class Solution {
private:
    // Struct to lock the original value together with its initial index location
    struct ElementNode {
        int value;
        int originalIndex;
    };

    // Shared tracking buffer used during the merge sort phase to avoid constant allocations
    vector<ElementNode> mergeBuffer;

    /*
     * RECURSIVE DIVIDE AND CONQUER MERGE SORT SPLIT
     * ---------------------------------------------
     * Standard merge sort splitting mechanics that bounds the scope by tracking 
     * a left target boundary up to a right boundary.
     */
    void divideAndConquerSort(vector<ElementNode>& nodes, int left, int right, vector<int>& smallerCountsRegistry) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        
        // Recursively split the system down into isolated sub-problems
        divideAndConquerSort(nodes, left, mid, smallerCountsRegistry);
        divideAndConquerSort(nodes, mid + 1, right, smallerCountsRegistry);
        
        // Merge the sorted halves back together while tracking inversions
        consolidateAndTrackInversions(nodes, left, mid, right, smallerCountsRegistry);
    }

    /*
     * INVERSION MARKER CONSOLIDATION ENGINE
     * -------------------------------------
     * As we merge two sorted sub-arrays (Left Half and Right Half) back together in descending order:
     * - If the element in the Left Half is strictly greater than the element in the Right Half,
     *   it is mathematically guaranteed to be greater than *all* remaining elements to the right 
     *   of the Right Half pointer.
     * - Therefore, the number of smaller elements after self for this item increases by the count 
     *   of elements currently remaining in the Right Half block: `(right - rightPointer + 1)`.
     */
    void consolidateAndTrackInversions(vector<ElementNode>& nodes, int left, int mid, int right, vector<int>& smallerCountsRegistry) {
        int leftPointer = left;
        int rightPointer = mid + 1;
        int bufferWriteIndex = left;

        // Merge both blocks in descending order to count smaller items cleanly
        while (leftPointer <= mid && rightPointer <= right) {
            if (nodes[static_cast<size_t>(leftPointer)].value > nodes[static_cast<size_t>(rightPointer)].value) {
                // Inversion Track: All elements from rightPointer to right are smaller than leftPointer's value
                int smallerElementsCount = right - rightPointer + 1;
                int targetOriginalIndex = nodes[static_cast<size_t>(leftPointer)].originalIndex;
                smallerCountsRegistry[static_cast<size_t>(targetOriginalIndex)] += smallerElementsCount;

                mergeBuffer[static_cast<size_t>(bufferWriteIndex++)] = nodes[static_cast<size_t>(leftPointer++)];
            } else {
                // The right element is greater or equal -> move it to the buffer, no inversion tracked
                mergeBuffer[static_cast<size_t>(bufferWriteIndex++)] = nodes[static_cast<size_t>(rightPointer++)];
            }
        }

        // Flush out any remaining items left over in the Left Half container
        while (leftPointer <= mid) {
            mergeBuffer[static_cast<size_t>(bufferWriteIndex++)] = nodes[static_cast<size_t>(leftPointer++)];
        }

        // Flush out any remaining items left over in the Right Half container
        while (rightPointer <= right) {
            mergeBuffer[static_cast<size_t>(bufferWriteIndex++)] = nodes[static_cast<size_t>(rightPointer++)];
        }

        // Write the sorted buffer data back into our primary working array segment
        for (int i = left; i <= right; ++i) {
            nodes[static_cast<size_t>(i)] = mergeBuffer[static_cast<size_t>(i)];
        }
    }

public:
    /*
     * MERGE SORT WITH COORDINATE INVERSION TRACKING ENGINE
     * ----------------------------------------------------
     * The objective is to return an integer array containing the count of smaller elements 
     * to the right of each element in the input array.
     */
    vector<int> countSmaller(vector<int>& nums) {
        size_t n = nums.size();
        if (n == 0) return {};

        vector<int> smallerCountsRegistry(n, 0);
        vector<ElementNode> nodes(n);
        mergeBuffer.resize(n);

        // Map values along with their original index tags to maintain tracking coordinates
        for (size_t i = 0; i < n; ++i) {
            nodes[i] = {nums[i], static_cast<int>(i)};
        }

        // Launch the modified merge sort operation
        divideAndConquerSort(nodes, 0, static_cast<int>(n) - 1, smallerCountsRegistry);

        return smallerCountsRegistry;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total number of items inside the array.
 *                  The system breaks structural patterns down across standard log(N) divide 
 *                  levels, executing linear sorting evaluations bounded by N steps at each tier.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization engine allocates a persistent tracking array 
 *                   and structural nodes layout pool proportional to the size of the array dataset.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The array size must be greater than 0." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting merge sort with coordinate inversion tracking sweep..." << endl;
    vector<int> result = solver.countSmaller(nums);

    cout << "\nCalculated Counts of Smaller Elements After Self:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}