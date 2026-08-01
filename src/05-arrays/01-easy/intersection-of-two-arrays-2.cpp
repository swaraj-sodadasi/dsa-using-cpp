#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * APPROACH 1: HASH MAP FREQUENCY TRACKING (O(N + M) TIME, O(MIN(N, M)) SPACE)
     * --------------------------------------------------------------------------
     * This strategy is highly efficient when the input arrays are unsorted and 
     * can fit entirely within system memory.
     *
     * Implementation Details:
     * - We count the frequencies of each number in the smaller array using a hash map.
     *   This keeps the extra space footprint as small as possible.
     * - We then iterate through the second array. For each number, we check if it 
     *   exists with a frequency greater than 0 in our map.
     * - If it does, the number is part of the intersection. We append it to our results 
     *   and decrement its frequency count in the map to handle duplicate numbers correctly.
     */
    vector<int> intersectHashMap(vector<int>& nums1, vector<int>& nums2) {
        // Optimization: Ensure nums1 is the smaller array to minimize map memory overhead
        if (nums1.size() > nums2.size()) {
            return intersectHashMap(nums2, nums1);
        }

        unordered_map<int, int> frequencyMap;
        for (int num : nums1) {
            frequencyMap[num]++;
        }

        vector<int> intersectionResult;
        for (int num : nums2) {
            if (frequencyMap.count(num) && frequencyMap[num] > 0) {
                intersectionResult.push_back(num);
                frequencyMap[num]--; // Consume one instance of the matched duplicate
            }
        }

        return intersectionResult;
    }

    /*
     * APPROACH 2: SORTED TWO-POINTER INTERSECTION (O(N LOG N + M LOG M) TIME, O(1) SPACE)
     * -----------------------------------------------------------------------------------
     * This strategy is ideal when the input arrays are already sorted, or if memory space 
     * is limited and cannot accommodate an auxiliary hash map structure.
     *
     * Implementation Details:
     * - We sort both arrays in ascending order.
     * - We initialize two tracking pointers (`p1` and `p2`) at the beginning of both arrays.
     * - We compare elements at the pointers:
     *   - If `nums1[p1] == nums2[p2]`, we found a match. We save the value and advance both pointers.
     *   - If `nums1[p1] < nums2[p2]`, we advance `p1` to search for larger matching candidates.
     *   - If `nums1[p1] > nums2[p2]`, we advance `p2` to search for larger matching candidates.
     */
    vector<int> intersectTwoPointer(vector<int>& nums1, vector<int>& nums2) {
        // Sort both arrays to prepare for the synchronized linear sweep
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());

        vector<int> intersectionResult;
        size_t p1 = 0, p2 = 0;

        while (p1 < nums1.size() && p2 < nums2.size()) {
            if (nums1[p1] == nums2[p2]) {
                intersectionResult.push_back(nums1[p1]);
                p1++;
                p2++;
            } else if (nums1[p1] < nums2[p2]) {
                p1++; // Move the pointer on the smaller value forward
            } else {
                p2++; // Move the pointer on the smaller value forward
            }
        }

        return intersectionResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * Hash Map Approach:
 *   - TIME COMPLEXITY: O(N + M) - Linear sweep to populate the frequency tracking map, 
 *                      followed by a linear sweep through the second array.
 *   - SPACE COMPLEXITY: O(min(N, M)) - Auxiliary allocation allocated to hold frequency 
 *                       counts for elements of the smaller array inside the hash map.
 *
 * Sorted Two-Pointer Approach:
 *   - TIME COMPLEXITY: O(N log N + M log M) - The time limit is governed by the upfront sorting 
 *                      steps. The subsequent two-pointer intersection sweep runs in linear O(N + M) time.
 *   - SPACE COMPLEXITY: O(1) Auxiliary - Operates directly on the input structures, requiring 
 *                       no extra memory tracking frameworks outside of standard loop variables.
 */
int main() {
    int n, m;
    cout << "Enter the size of the first array (nums1): ";
    if (!(cin >> n) || n < 0) return 1;

    vector<int> nums1(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter elements for the first array separated by spaces:\n";
        for (int i = 0; i < n; ++i) cin >> nums1[static_cast<size_t>(i)];
    }

    cout << "Enter the size of the second array (nums2): ";
    if (!(cin >> m) || m < 0) return 1;

    vector<int> nums2(static_cast<size_t>(m));
    if (m > 0) {
        cout << "Enter elements for the second array separated by spaces:\n";
        for (int i = 0; i < m; ++i) cin >> nums2[static_cast<size_t>(i)];
    }

    Solution solver;
    
    // Create copies to keep the original input data clean for both independent runs
    vector<int> hashInput1 = nums1;
    vector<int> hashInput2 = nums2;
    vector<int> resHash = solver.intersectHashMap(hashInput1, hashInput2);

    vector<int> ptrInput1 = nums1;
    vector<int> ptrInput2 = nums2;
    vector<int> resPtr = solver.intersectTwoPointer(ptrInput1, ptrInput2);

    cout << "\n[Approach 1] Hash Map Frequency Intersection Result:\n[ ";
    for (size_t i = 0; i < resHash.size(); ++i) {
        cout << resHash[i] << (i + 1 < resHash.size() ? ", " : "");
    }
    cout << " ]\n";

    cout << "[Approach 2] Sorted Two-Pointer Intersection Result:\n[ ";
    for (size_t i = 0; i < resPtr.size(); ++i) {
        cout << resPtr[i] << (i + 1 < resPtr.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}