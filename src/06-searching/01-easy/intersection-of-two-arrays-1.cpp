#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * UNORDERED SET VALUE MEMBERSHIP ENGINE
     * -------------------------------------
     * The objective is to compute the intersection of two integer arrays. 
     * Each element in the result must be unique, and the result can be returned in any order.
     *
     * Algorithmic Strategy:
     * - A naive approach compares every element of `nums1` with `nums2`, taking O(N * M) time.
     * - We optimize this to O(N + M) linear time by using a hash set (`unordered_set`) 
     *   to achieve average constant-time O(1) membership lookups.
     *
     * Membership Extraction Protocol:
     * - Step 1: Insert all elements of `nums1` into a unique lookup hash set (`nums1Set`). 
     *   This filters out duplicates from the first array and sets up O(1) searches.
     * - Step 2: Iterate through `nums2`. For each element, verify if it exists inside `nums1Set`.
     * - Step 3: If a match is found, add the element to our `intersectionResult` list and 
     *   instantly remove it from `nums1Set`. Deleting the matched element prevents any subsequent 
     *   duplicate values in `nums2` from adding it to the result again, avoiding a secondary deduplication step.
     */
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // Populate the primary reference set with elements from the first collection
        unordered_set<int> nums1Set(nums1.begin(), nums1.end());
        vector<int> intersectionResult;

        // Traverse the second collection to find overlapping members
        for (int num : nums2) {
            if (nums1Set.count(num) > 0) {
                intersectionResult.push_back(num);
                // Erase the value to ensure it is not captured multiple times
                nums1Set.erase(num);
            }
        }

        return intersectionResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N and M represent the sizes of nums1 and nums2 respectively.
 *                  Building the initial hash set takes O(N) time, and scanning the second array 
 *                  takes O(M) time, with average constant O(1) lookup and erasure transformations.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization strategy instantiates a lookup set 
 *                   proportional to the size of the first array data collection.
 */
int main() {
    int n, m;
    cout << "Enter the total number of items in the first array (nums1): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be greater than 0." << endl;
        return 1;
    }
    vector<int> nums1(static_cast<size_t>(n));
    cout << "Enter elements for nums1 separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums1[static_cast<size_t>(i)];
    }

    cout << "Enter the total number of items in the second array (nums2): ";
    if (!(cin >> m) || m <= 0) {
        cout << "Invalid parameter. Array size must be greater than 0." << endl;
        return 1;
    }
    vector<int> nums2(static_cast<size_t>(m));
    cout << "Enter elements for nums2 separated by spaces:\n";
    for (int i = 0; i < m; ++i) {
        cin >> nums2[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting unordered set value membership verification..." << endl;
    vector<int> result = solver.intersection(nums1, nums2);

    cout << "\nUnique Intersection Elements Outcome:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}