#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: UNIQUE ELEMENTS COLLECTION VIA HASH SET
     * -------------------------------------------------------------------------------------
     * The objective is to compute the intersection of two arrays, returning a vector of 
     * unique elements present in both inputs. The result elements can appear in any order.
     * 
     * 1. VALUE DEDUPLICATION ARCHITECTURE:
     *    A naive nested search checks all combinations, taking O(N * M) time. Instead, we
     *    leverage the O(1) membership lookup properties of a hash set (`unordered_set`). 
     *    We populate a hash set `nums1Set` with all numbers from `nums1`, which strips away 
     *    duplicate entries and compresses the lookup space.
     * 
     * 2. SINGLE-PASS CROSS-INTERSECTION VERIFICATION:
     *    We parse the second array `nums2` element by element. For each number, we query `nums1Set`:
     *      - If found: The element is an intersection match. We add it to our `result` container.
     *      - Deduplication Safeguard: To prevent adding the same intersection match multiple times 
     *        if it appears repeatedly in `nums2`, we immediately erase the matched number from 
     *        `nums1Set` after adding it to the result list.
     * 
     * 3. TIME-SPACE TRADEOFF OPTIMIZATION:
     *    By spending linear space to hold unique elements, we transform element lookup penalties
     *    from linear scans to amortized constant time.
     */
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // Step 1: Deduplicate and store first array components in a lookup set
        unordered_set<int> nums1Set(nums1.begin(), nums1.end());
        vector<int> intersectionResult;

        // Step 2: Cross-reference second array against the set
        for (int num : nums2) {
            if (nums1Set.count(num)) {
                intersectionResult.push_back(num);
                // Erase key from lookup set to ensure distinct results
                nums1Set.erase(num);
            }
        }

        return intersectionResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N represents the count of elements in nums1 and M represents 
 *                  the count of elements in nums2. Initial set building takes O(N) time, and the 
 *                  subsequent scan across nums2 queries keys in amortized O(1) constant lookup time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal `nums1Set` collection stores up to N unique key 
 *                   records in the worst-case allocation scenario.
 */
int main() {
    int size1, size2;
    cout << "Enter the element capacity count for the first array (nums1): ";
    if (!(cin >> size1) || size1 <= 0) {
        cout << "Invalid sizing bounds configured." << endl;
        return 1;
    }
    vector<int> nums1(static_cast<size_t>(size1));
    cout << "Enter " << size1 << " elements for nums1:\n";
    for (int i = 0; i < size1; ++i) {
        cin >> nums1[static_cast<size_t>(i)];
    }

    cout << "Enter the element capacity count for the second array (nums2): ";
    if (!(cin >> size2) || size2 <= 0) {
        cout << "Invalid sizing bounds configured." << endl;
        return 1;
    }
    vector<int> nums2(static_cast<size_t>(size2));
    cout << "Enter " << size2 << " elements for nums2:\n";
    for (int i = 0; i < size2; ++i) {
        cin >> nums2[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting unique element hash set intersection sweeps..." << endl;
    vector<int> outputResult = solver.intersection(nums1, nums2);

    cout << "Computed Intersection Set Result: [ ";
    for (size_t i = 0; i < outputResult.size(); ++i) {
        cout << outputResult[i] << (i + 1 < outputResult.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}