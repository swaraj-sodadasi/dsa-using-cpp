#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * CORE LOGIC (Combinatorial Dynamic Programming / Memoization & Shared Pointer Safety):
 * 1. Cause of Double Free / Segmentation Fault:
 *    - In All Possible Full Binary Trees with memoization, subtrees are shared across 
 *      multiple root nodes in the Cartesian product construction.
 *    - Iterating through each root and calling naive `deleteTree(root)` recursively deletes 
 *      subtrees multiple times, leading to a double-free crash/segfault in cleanup.
 * 2. Solution:
 *    - Use an `unordered_set<TreeNode*> visited` during cleanup to ensure every dynamically 
 *      allocated node in the shared object graph is freed exactly once.
 * 
 * TIME COMPLEXITY: O(2^(n / 2)) - Proportional to Catalan number C_((n-1)/2).
 * SPACE COMPLEXITY: O(2^(n / 2)) - For memoized subtree configurations.
 */

class Solution {
    unordered_map<int, vector<TreeNode*>> memo;

public:
    vector<TreeNode*> allPossibleFBT(int n) {
        // Full binary trees must have an odd number of nodes
        if (n % 2 == 0) {
            return {};
        }

        // Base case: single node FBT
        if (n == 1) {
            return {new TreeNode(0)};
        }

        // Return memoized result if already computed
        if (memo.count(n)) {
            return memo[n];
        }

        vector<TreeNode*> result;

        // Partition remaining n - 1 nodes between left (i) and right (n - 1 - i) subtrees
        for (int i = 1; i < n; i += 2) {
            int leftNodes = i;
            int rightNodes = n - 1 - i;

            vector<TreeNode*> leftTrees = allPossibleFBT(leftNodes);
            vector<TreeNode*> rightTrees = allPossibleFBT(rightNodes);

            // Construct Cartesian Product of all left and right subtree configurations
            for (TreeNode* left : leftTrees) {
                for (TreeNode* right : rightTrees) {
                    TreeNode* root = new TreeNode(0);
                    root->left = left;
                    root->right = right;
                    result.push_back(root);
                }
            }
        }

        return memo[n] = result;
    }
};

// Helper function to print level-order representation of a tree
void printLevelOrder(TreeNode* root) {
    if (root == nullptr) return;

    queue<TreeNode*> q;
    q.push(root);

    cout << "[ ";
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        if (current != nullptr) {
            cout << current->val << " ";
            q.push(current->left);
            q.push(current->right);
        } else {
            cout << "null ";
        }
    }
    cout << "]";
}

// Safe deletion helper to avoid double-free errors across shared memoized subtrees
void safeDeleteTree(TreeNode* root, unordered_set<TreeNode*>& visited) {
    if (root == nullptr || visited.count(root)) return;

    visited.insert(root);
    safeDeleteTree(root->left, visited);
    safeDeleteTree(root->right, visited);
    delete root;
}

int main() {
    cout << "--- All Possible Full Binary Trees ---" << endl;

    int n;
    cout << "Enter total number of nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    Solution solver;
    vector<TreeNode*> fbtList = solver.allPossibleFBT(n);

    cout << "Total Full Binary Trees generated for n = " << n << ": " << fbtList.size() << endl;

    for (size_t i = 0; i < fbtList.size(); ++i) {
        cout << "Tree " << i + 1 << ": ";
        printLevelOrder(fbtList[i]);
        cout << endl;
    }

    // Clean up memory safely using a visited set to handle shared subtree nodes
    unordered_set<TreeNode*> visited;
    for (TreeNode* root : fbtList) {
        safeDeleteTree(root, visited);
    }

    return 0;
}