#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

/*
CORE LOGIC (TRIE-BASED BITWISE QUERY WITH SORTING CONSTRAINTS):
- The problem asks us to answer queries of the form [xi, mi]. For each query, we want to maximize `xi ^ nums[j]` 
  subject to the constraint that `nums[j] <= mi`. If no such element exists, the answer for that query is -1.
- **Offline Query Processing**:
  - Since queries can be answered in any order, we can process them offline. 
  - We sort the array `nums` in ascending order.
  - We sort the `queries` based on their limit values (`mi`) in ascending order. To ensure we return the answers 
    in the correct order, we store the original index of each query alongside its parameters.
- **Dynamic Trie Insertion**:
  - We maintain a pointer `numsPtr` tracking the next element to insert from the sorted `nums` array into a Binary Trie.
  - As we iterate through our sorted queries, we look at the limit `mi` for the current query. We insert all elements 
    from `nums` that are less than or equal to `mi` into the Trie.
  - This ensures that when we evaluate a query, the Trie contains *only* the valid candidate numbers that satisfy the constraint `nums[j] <= mi`.
- **Greedy Maximum XOR Matching**:
  - Once the valid numbers are in the Trie, we query the Trie using `xi`. For each bit position from 31 down to 0, 
    we greedily try to follow the path opposite to the current bit of `xi` to maximize the XOR output.
  - If the Trie is empty (meaning no element in `nums` is `<= mi`), we directly record -1 for that query.

TIME COMPLEXITY: O(N log N + Q log Q + (N + Q) * 32) -> O(N log N + Q log Q) where N is the size of `nums` and Q is the size of `queries`. Sorting dominates, while the Trie operations scale linearly with the bit length.
SPACE COMPLEXITY: O(N * 32 + Q) -> O(N + Q) auxiliary space to build out the structural Trie nodes and store the offline query indexes.
*/

struct TrieNode {
    unique_ptr<TrieNode> children[2];
};

class BinaryTrie {
private:
    unique_ptr<TrieNode> root;

public:
    BinaryTrie() {
        root = make_unique<TrieNode>();
    }

    // Insert the 32-bit binary structure of a number into the Trie
    void insert(int num) {
        TrieNode* curr = root.get();
        for (int i = 31; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (!curr->children[bit]) {
                curr->children[bit] = make_unique<TrieNode>();
            }
            curr = curr->children[bit].get();
        }
    }

    // Find the maximum possible XOR output for a given number using the current Trie paths
    int getMaxXor(int num) {
        TrieNode* curr = root.get();
        int maxXorForNum = 0;
        
        for (int i = 31; i >= 0; --i) {
            int bit = (num >> i) & 1;
            int toggledBit = 1 - bit; // The ideal bit to maximize the XOR result
            
            // Greedily traverse the opposite path if available to secure a 1 bit
            if (curr->children[toggledBit]) {
                maxXorForNum |= (1 << i);
                curr = curr->children[toggledBit].get();
            } else if (curr->children[bit]) {
                curr = curr->children[bit].get();
            } else {
                return -1; // Fallback security check
            }
        }
        return maxXorForNum;
    }
};

struct QueryInfo {
    int x;
    int m;
    int originalIndex;
};

vector<int> maximizeXor(vector<int>& nums, const vector<vector<int>>& queries) {
    size_t numCount = nums.size();
    size_t queryCount = queries.size();
    
    vector<int> answers(queryCount, -1);
    
    // Package query info alongside original index arrays for offline sorting execution
    vector<QueryInfo> offlineQueries;
    offlineQueries.reserve(queryCount);
    for (int i = 0; i < static_cast<int>(queryCount); ++i) {
        offlineQueries.push_back({queries[static_cast<size_t>(i)][0], queries[static_cast<size_t>(i)][1], i});
    }
    
    // Sort array elements and queries based on constraints in ascending fashion
    sort(nums.begin(), nums.end());
    sort(offlineQueries.begin(), offlineQueries.end(), [](const QueryInfo& a, const QueryInfo& b) {
        return a.m < b.m;
    });
    
    BinaryTrie trie;
    size_t numsPtr = 0;
    
    // Process each structured query offline
    for (const auto& q : offlineQueries) {
        // Incrementally insert all elements from nums that fit within the constraint threshold
        while (numsPtr < numCount && nums[numsPtr] <= q.m) {
            trie.insert(nums[numsPtr]);
            numsPtr++;
        }
        
        // If elements were successfully added to the Trie, execute the maximum bit-matching query
        if (numsPtr > 0) {
            answers[static_cast<size_t>(q.originalIndex)] = trie.getMaxXor(q.x);
        } else {
            answers[static_cast<size_t>(q.originalIndex)] = -1;
        }
    }
    
    return answers;
}

int main() {
    int n, q;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input sizing array initialization constraints." << endl;
        return 1;
    }
    
    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }
    
    cout << "Enter the total number of queries: ";
    if (!(cin >> q) || q <= 0) {
        cout << "Invalid query allocation bounds constraints." << endl;
        return 1;
    }
    
    vector<vector<int>> queries(static_cast<size_t>(q), vector<int>(2));
    cout << "Enter each query as 'x m' pair sequence parameters:\n";
    for (int i = 0; i < q; ++i) {
        cin >> queries[static_cast<size_t>(i)][0] >> queries[static_cast<size_t>(i)][1];
    }
    
    vector<int> results = maximizeXor(nums, queries);
    
    cout << "\nThe maximum XOR evaluation response outputs matching query conditions are:\n";
    for (int i = 0; i < q; ++i) {
        cout << "Query " << i << " [x=" << queries[static_cast<size_t>(i)][0] 
             << ", m=" << queries[static_cast<size_t>(i)][1] << "] -> " << results[static_cast<size_t>(i)] << "\n";
    }
    
    return 0;
}