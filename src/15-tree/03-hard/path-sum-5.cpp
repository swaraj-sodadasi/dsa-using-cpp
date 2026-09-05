#include <iostream>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * CORE LOGIC (Prefix Sum Hash Map Backtracking DFS):
 * 1. A continuous path summing to `targetSum` starting at any ancestor node and ending at `curr` satisfies:
 *    `currentPrefixSum - targetSum = oldPrefixSum`
 *    Where `oldPrefixSum` is a prefix sum seen higher up on the current path branch.
 * 2. Hash Map Maintenance (`unordered_map<long long, int> prefixSumMap`):
 *    - Stores frequency of running prefix sums along the current DFS call stack.
 *    - Base condition: Initialize `prefixSumMap[0] = 1` to account for paths starting directly at the root.
 * 3. DFS Processing Steps:
 *    - Add `root->val` to `currSum` (using `long long` to prevent integer overflow).
 *    - Check if `(currSum - targetSum)` exists in `prefixSumMap` and increment `totalPaths` by its frequency.
 *    - Record current `currSum` in `prefixSumMap`.
 *    - Recurse down `left` and `right` subtrees.
 *    - Backtrack: Decrement frequency of `currSum` in `prefixSumMap` before returning to parent caller.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node is visited exactly once, and map lookups take O(1) average time.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to recursion call stack and hash map space.
 * - O(n) worst case for skewed tree, O(log n) best case for balanced tree.
 */

void pathSumDFS(TreeNode* root, long long currSum, int targetSum, 
                unordered_map<long long, int>& prefixSumMap, int& totalPaths) {
    if (root == nullptr) {
        return;
    }

    // Accumulate prefix sum along current path branch
    currSum += root->val;

    // Check if a sub-path matching targetSum exists ending at current node
    if (prefixSumMap.count(currSum - targetSum)) {
        totalPaths += prefixSumMap[currSum - targetSum];
    }

    // Record current prefix sum
    prefixSumMap[currSum]++;

    // Recurse left and right subtrees
    pathSumDFS(root->left, currSum, targetSum, prefixSumMap, totalPaths);
    pathSumDFS(root->right, currSum, targetSum, prefixSumMap, totalPaths);

    // Backtrack: remove current prefix sum state before returning
    prefixSumMap[currSum]--;
}

int pathSum(TreeNode* root, int targetSum) {
    int totalPaths = 0;
    unordered_map<long long, int> prefixSumMap;
    prefixSumMap[0] = 1; // Base case for paths starting directly from root

    pathSumDFS(root, 0, targetSum, prefixSumMap, totalPaths);
    return totalPaths;
}

// Helper function to dynamically construct tree for exactly 'n' non-null nodes
TreeNode* buildTreeForN(int n) {
    if (n <= 0) return nullptr;

    cout << "Enter values in level-order space separated (use 'null' or 'N' for empty slots): ";
    string inputLine;
    getline(cin >> ws, inputLine);

    stringstream ss(inputLine);
    string item;

    if (!(ss >> item) || item == "null" || item == "N") {
        return nullptr;
    }

    TreeNode* root = new TreeNode(stoi(item));
    queue<TreeNode*> q;
    q.push(root);

    int allocatedCount = 1;

    while (!q.empty() && allocatedCount < n) {
        TreeNode* current = q.front();
        q.pop();

        // Process left child
        if (allocatedCount < n && (ss >> item)) {
            if (item != "null" && item != "N") {
                current->left = new TreeNode(stoi(item));
                q.push(current->left);
                allocatedCount++;
            }
        } else {
            break;
        }

        // Process right child
        if (allocatedCount < n && (ss >> item)) {
            if (item != "null" && item != "N") {
                current->right = new TreeNode(stoi(item));
                q.push(current->right);
                allocatedCount++;
            }
        } else {
            break;
        }
    }

    return root;
}

// Helper function to free dynamically allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Path Sum III (Prefix Sum Hash Map DFS) ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    int targetSum;
    cout << "Enter target sum: ";
    cin >> targetSum;

    // Compute total downward paths matching targetSum
    int totalMatchingPaths = pathSum(root, targetSum);

    // Output Result
    cout << "Total Downward Paths Matching Target Sum " << targetSum << ": " << totalMatchingPaths << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}