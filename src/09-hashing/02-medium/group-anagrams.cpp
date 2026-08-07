#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        // Map stores: {Sorted_Anagram_Signature_Key -> List_Of_Original_Matching_Strings}
        unordered_map<string, vector<string>> groupingRegistry;

        // Step 2: Distribute strings into corresponding equivalence buckets
        for (const string& currentWord : strs) {
            string signatureKey = currentWord;
            sort(signatureKey.begin(), signatureKey.end());
            
            groupingRegistry[signatureKey].push_back(currentWord);
        }

        // Step 3: Extract the grouped clusters from the map structure
        vector<vector<string>> groupedAnagramsResult;
        groupedAnagramsResult.reserve(groupingRegistry.size());
        
        for (auto& bucketPair : groupingRegistry) {
            // FIX: Explicitly qualified with 'std::' to avoid unqualified lookup warnings
            groupedAnagramsResult.push_back(std::move(bucketPair.second));
        }

        return groupedAnagramsResult;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * L * log(L)) - Where N represents the total number of words in the vector, 
 *                  and L represents the maximum length of an individual string. For each word, sorting 
 *                  takes O(L log L) time, followed by an amortized O(1) hash map lookup and insertion.
 *
 * SPACE COMPLEXITY: O(N * L) Auxiliary - Required to store the original words and their corresponding 
 *                   sorted signature key records inside the dictionary registry structure.
 */
int main() {
    int wordCount;
    cout << "Enter the total number of strings to process: ";
    if (!(cin >> wordCount) || wordCount < 0) {
        cout << "Invalid string count parameter configured." << endl;
        return 1;
    }

    vector<string> strs(static_cast<size_t>(wordCount));
    cout << "Enter the strings consecutively (Lowercase alphabetic strings only):\n";
    for (int i = 0; i < wordCount; ++i) {
        cin >> strs[static_cast<size_t>(i)];
        
        // Enforce lowercase parameter constraints
        for (char c : strs[static_cast<size_t>(i)]) {
            if (c < 'a' || c > 'z') {
                cout << "Constraint Error: Inputs must contain lowercase letters exclusively." << endl;
                return 1;
            }
        }
    }

    Solution solver;
    cout << "\nExecuting signature-key equivalence partition sweeps..." << endl;
    vector<vector<string>> outputResult = solver.groupAnagrams(strs);

    cout << "\nComputed Anagram Groups Map Output:\n[\n";
    for (const auto& group : outputResult) {
        cout << "  [ ";
        for (size_t i = 0; i < group.size(); ++i) {
            cout << "\"" << group[i] << "\"" << (i + 1 < group.size() ? ", " : "");
        }
        cout << " ]\n";
    }
    cout << "]\n";

    return 0;
}