#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /*
     * BUCKET SORT FREQUENCY OPTIMIZATION ENGINE
     * -----------------------------------------
     * The objective is to extract the 'k' most frequent elements from an unsorted 
     * integer array, running within a guaranteed O(N) linear time boundary.
     *
     * Algorithmic Strategy (Bucket Sort Architecture):
     * - A standard sorting scheme takes O(N log N) time, while a heap (priority queue) 
     *   takes O(N log K) time. To optimize down to a strict linear O(N) execution bound, 
     *   we apply a frequency-to-value bucket distribution mapping technique.
     * - The absolute maximum frequency any single element can achieve is bounded by the total 
     *   length of the array `N`.
     *
     * Transformation Protocol Steps:
     * - Step 1: Count element frequencies using a fast hash map (`unordered_map<int, int>`).
     * - Step 2: Instantiate an array of vectors called `buckets`, where the array size is 
     *   set to `N + 1`. The index of each slot in the `buckets` array represents a specific 
     *   frequency count.
     * - Step 3: Iterate through the frequency map and place each element value into the 
     *   bucket corresponding to its frequency (`buckets[frequency].push_back(element)`).
     * - Step 4: To gather the top `k` most frequent items, scan backwards from the highest 
     *   frequency bucket (`index = N`) down toward 0. Collect all values stored within these 
     *   buckets until our result container holds exactly `k` elements.
     */
    vector<int> topKFrequent(vector<int>& nums, int k) {
        size_t n = nums.size();
        
        // Step 1: Calculate raw element distributions via hash frequency tracking
        unordered_map<int, int> frequencyMap;
        for (int num : nums) {
            frequencyMap[num]++;
        }

        // Step 2: Initialize the frequency bucket array system
        // Array index represents frequency, mapping to a vector of numbers sharing that frequency
        vector<vector<int>> buckets(n + 1);

        // Step 3: Distribute unique elements across frequency buckets
        for (auto& pair : frequencyMap) {
            int element = pair.first;
            int frequency = pair.second;
            buckets[static_cast<size_t>(frequency)].push_back(element);
        }

        // Step 4: Scan from highest frequency down to gather the top k elements
        vector<int> topElementsResult;
        topElementsResult.reserve(static_cast<size_t>(k));

        for (int i = static_cast<int>(n); i >= 0; --i) {
            if (topElementsResult.size() >= static_cast<size_t>(k)) {
                break;
            }

            // Extract all elements sharing this current frequency level
            for (int num : buckets[static_cast<size_t>(i)]) {
                topElementsResult.push_back(num);
                if (topElementsResult.size() == static_cast<size_t>(k)) {
                    return topElementsResult;
                }
            }
        }

        return topElementsResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total count of elements inside the nums array. 
 *                  Building the frequency map takes O(N) time. Populating the bucket lists takes O(N) 
 *                  time, and the final backwards collection pass scans at most N bucket items, 
 *                  ensuring a linear execution runtime profile.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization strategy instantiates an internal hash map 
 *                   container alongside a frequency bucket matrix layout that scales proportionally 
 *                   with the total size footprint of the input array.
 */
int main() {
    int n;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the array elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int k;
    cout << "Enter the target count parameter (K, where 1 <= K <= number of unique elements): ";
    if (!(cin >> k) || k < 1 || k > n) {
        cout << "Constraint Error: Rank element limit parameter falls outside boundary constraints." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting bucket sort frequency distribution and extraction sweep..." << endl;
    vector<int> result = solver.topKFrequent(nums, k);

    cout << "The top " << k << " most frequent elements isolated inside the collection:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    return 0;
}