#include <iostream>
#include <algorithm>
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
 * CORE LOGIC:
 * 1. Base Case: If the current node (root) is nullptr, the depth is 0.
 * 2. Recursive Step: Recursively calculate the maximum depth of the left subtree
 *    and the right subtree.
 * 3. The maximum depth of the current tree is 1 (for the current node) plus the 
 *    maximum of the left and right subtree depths:
 *    maxDepth(root) = 1 + max(maxDepth(root->left), maxDepth(root->right))
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the number of nodes in the binary tree.
 * - Each node is visited exactly once during the traversal.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree.
 * - This space is consumed by the implicit recursion call stack.
 * - Worst case: O(N) for a skewed/skew-tree (completely unbalanced).
 * - Best/Average case: O(log N) for a balanced binary tree.
 */
int maxDepth(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return 1 + max(leftDepth, rightDepth);
}

// Helper function to build a binary tree from level-order input (e.g., "3 9 20 null null 15 7")
TreeNode* buildTreeFromLevelOrder() {
    cout << "Enter tree nodes in level-order separated by space (use 'null' or 'N' for empty nodes): ";
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

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        // Process left child
        if (ss >> item) {
            if (item != "null" && item != "N") {
                current->left = new TreeNode(stoi(item));
                q.push(current->left);
            }
        } else {
            break;
        }

        // Process right child
        if (ss >> item) {
            if (item != "null" && item != "N") {
                current->right = new TreeNode(stoi(item));
                q.push(current->right);
            }
        } else {
            break;
        }
    }
    return root;
}

// Helper function to free allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Maximum Depth of Binary Tree ---" << endl;
    
    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    // Compute maximum depth
    int depth = maxDepth(root);

    // Output Result
    cout << "Maximum Depth of the Binary Tree is: " << depth << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}