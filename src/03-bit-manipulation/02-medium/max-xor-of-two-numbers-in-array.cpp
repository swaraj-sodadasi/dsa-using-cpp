#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

/*
CORE LOGIC (TRIE-BASED BITWISE BIT-MATCHING):
- The problem asks us to find the maximum result of $nums[i] \oplus nums[j]$ from a given integer array.
- **Trie Structure for Binary Paths**:
  - We represent the 32-bit binary structure of each number as a path down a binary Trie.
  - Each node in the Trie has exactly two possible child pointers: `children[0]` for a 0 bit, and `children[1]` for a 1 bit.
- **Greedy Maximum XOR Selection Strategy**:
  - To maximize an XOR value, we want the resulting bits to be 1 as high up (as close to the Most Significant Bit) as possible.
  - For each number in the array, we first insert its 32-bit binary path into our Trie.
  - Then, we traverse the Trie using the bits of that same number to find the optimal matching path. For each bit position `i`:
    - We look at the current bit of our number. To get a 1 from the XOR operation, we want to choose the opposite bit value if it exists in the Trie (`toggledBit = 1 - currentBit`).
    - If the opposite bit branch exists, we greedily follow it and add $2^i$ to our current XOR sum.
    - If it does not exist, we are forced to take the branch that matches our current bit, adding 0 to our XOR sum for that position.
  - We track and update the highest XOR sum found across all numbers.

TIME COMPLEXITY: O(32 * N) = O(N) - Where N is the total number of elements in the array. We insert and search for each number in a fixed 32-level depth structure.
SPACE COMPLEXITY: O(32 * N) = O(N) - The total number of nodes created inside our dynamic Trie is bounded linearly by the binary expansions of the dataset.
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

    // Insert the 32-bit binary sequence of a number into the Trie structure
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

    // Find the maximum possible XOR output for a given number using the Trie paths
    int getMaxXor(int num) {
        TrieNode* curr = root.get();
        int maxXorForNum = 0;
        
        for (int i = 31; i >= 0; --i) {
            int bit = (num >> i) & 1;
            int toggledBit = 1 - bit; // The ideal bit to maximize the XOR output
            
            // Greedily traverse the opposite path if available to secure a 1 bit
            if (curr->children[toggledBit]) {
                maxXorForNum |= (1 << i);
                curr = curr->children[toggledBit].get();
            } else if (curr->children[bit]) {
                curr = curr->children[bit].get();
            } else {
                break; // Boundary exit if the path terminates unexpectedly
            }
        }
        return maxXorForNum;
    }
};

int findMaximumXOR(const vector<int>& nums) {
    if (nums.empty()) return 0;

    BinaryTrie trie;
    
    // Build out the full binary paths for the array within our Trie framework
    for (int num : nums) {
        trie.insert(num);
    }

    int globalMaxXor = 0;
    
    // Evaluate the maximum matching path combination for each number
    for (int num : nums) {
        globalMaxXor = max(globalMaxXor, trie.getMaxXor(num));
    }

    return globalMaxXor;
}

int main() {
    int n;
    cout << "Enter the total number of elements: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input configuration length boundaries." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the non-negative integer elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    int result = findMaximumXOR(nums);
    cout << "\nThe maximum XOR product attainable from two elements is: " << result << endl;

    return 0;
}