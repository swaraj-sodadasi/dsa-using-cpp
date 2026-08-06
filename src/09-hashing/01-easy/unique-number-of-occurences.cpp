#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: FREQUENCY VALUE-TO-SET UNIQUENESS VERIFICATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine if the frequency of occurrences of each value in an 
     * integer array is unique. Return true if all frequencies are distinct, and false otherwise.
     * 
     * 1. TWO-STAGE EXTRACTION PIPELINE:
     *    A nested-loop solution or an in-place sort approach can be inefficient or break input 
     *    invariants. Instead, we divide the tracking task into two clean linear stages:
     *      - Stage 1 (Frequency Collection Map): We iterate through the array and aggregate occurrence 
     *        counts inside an `unordered_map` (`element -> occurrenceCount`).
     *      - Stage 2 (Set Uniqueness Filtering): We parse the collected frequency integers from the map 
     *        and insert them one by one into an `unordered_set`. 
     * 
     * 2. EARLY DETECTION ELEMENT COLLISION:
     *    During Stage 2, for each frequency entry we process, we check if it already exists in the set:
     *      - If found: A frequency collision has occurred (meaning two different elements appeared 
     *        the exact same number of times). We immediately return false.
     *      - If not found: We register the frequency count in the set and continue.
     * 
     * 3. CONSTANT-BOUND EFFICIENCY LAYER:
     *    By using hash containers for both counting and uniqueness filtering, the algorithm guarantees 
     *    amortized linear-time execution.
     */
    bool uniqueOccurrences(vector<int>& arr) {
        // Step 1: Map elements to their corresponding occurrence counts
        unordered_map<int, int> elementCounts;
        for (int num : arr) {
            elementCounts[num]++;
        }

        // Step 2 & 3: Filter frequency values through a uniqueness hash set
        unordered_set<int> uniqueFrequencies;
        for (const auto& pair : elementCounts) {
            int frequency = pair.second;
            
            // Check if this specific occurrence count has already been registered
            if (uniqueFrequencies.find(frequency) != uniqueFrequencies.end()) {
                return false; 
            }
            uniqueFrequencies.insert(frequency);
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of elements in the array. 
 *                  The first loop scans the input array to populate frequencies, and the 
 *                  second loop iterates through the unique elements map entries. Hash map 
 *                  and hash set lookups/insertions operate in amortized O(1) constant time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal `elementCounts` map and `uniqueFrequencies` 
 *                   set allocate memory space proportional to the number of distinct elements, 
 *                   storing up to N records in the worst-case scenario.
 */
int main() {
    int count;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> count) || count <= 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    vector<int> arr(static_cast<size_t>(count));
    cout << "Enter the array elements sequentially:\n";
    for (int i = 0; i < count; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting value frequency mapping and set uniqueness checks..." << endl;
    bool hasUniqueFrequencies = solver.uniqueOccurrences(arr);

    if (hasUniqueFrequencies) {
        cout << "Outcome: All element frequency occurrence counts are unique. (Result = true)" << endl;
    } else {
        cout << "Outcome: Frequency collision detected. Duplicate frequency counts found. (Result = false)" << endl;
    }

    return 0;
}