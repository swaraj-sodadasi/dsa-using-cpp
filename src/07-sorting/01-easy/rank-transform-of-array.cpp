#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * SORTED MAP / HASH COORDINATE COMPRESSION ENGINE
     * ------------------------------------------------
     * The objective is to replace each element in an array with its corresponding rank. 
     * Ranks are assigned in ascending order starting from 1, where smaller elements receive 
     * smaller ranks. Identical elements must share the exact same rank.
     *
     * Algorithmic & Structural Framework:
     * - A coordinate compression technique maps the raw data value spectrum down onto a 
     *   dense, smaller sequence range.
     *
     * Mapping Protocol:
     * - Step 1: Clone the initial array layout configuration (`clonedNums = arr`).
     * - Step 2: Sort the cloned collection in ascending order to resolve relative ordering.
     * - Step 3: Iterate through the sorted sequence to build a unique index profile. We use 
     *   an efficient hash map (`unordered_map<int, int>`) to map values to ranks. We skip 
     *   duplicates during this step so that identical values receive the same rank.
     * - Step 4: Transform the original array in-place by looking up each element's rank 
     *   in the hash map.
     */
    vector<int> arrayRankTransform(vector<int>& arr) {
        if (arr.empty()) return arr;

        // Step 1: Clone the initial input structure to preserve the original position layout
        vector<int> clonedNums = arr;

        // Step 2: Sort the collection to establish ascending rank layout orders
        sort(clonedNums.begin(), clonedNums.end());

        // Step 3: Populate the coordinate hash mapping container while removing duplicate elements
        unordered_map<int, int> rankMap;
        int currentRank = 1;

        for (int num : clonedNums) {
            // If the element is not already registered, assign it the current rank and increment
            if (rankMap.find(num) == rankMap.end()) {
                rankMap[num] = currentRank;
                currentRank++;
            }
        }

        // Step 4: Map values to ranks in the original array
        for (size_t i = 0; i < arr.size(); ++i) {
            arr[i] = rankMap[arr[i]];
        }

        return arr;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total elements inside the array. 
 *                  Cloning and sorting the collection takes O(N log N) time. The hash map 
 *                  insertions and lookups run in linear O(N) average time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The coordinate mapping model instantiates a copy 
 *                   of the input values alongside an internal hash structure mapping size bounds 
 *                   proportional to N.
 */
int main() {
    int n;
    cout << "Enter the total number of items in the array: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid parameter. Array size cannot be negative." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter the array elements separated by spaces:\n";
        for (int i = 0; i < n; ++i) {
            cin >> arr[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nExecuting sorted map coordinate hash rank tracking updates..." << endl;
    solver.arrayRankTransform(arr);

    cout << "Rank transformed array layout sequence output:\n[ ";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << (i + 1 < arr.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}