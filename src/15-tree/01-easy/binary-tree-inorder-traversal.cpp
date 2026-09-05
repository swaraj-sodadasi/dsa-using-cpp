#include <iostream>
#include <vector>
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
 * CORE LOGIC (Morris Traversal / Threaded Binary Tree):
 * 1. Initialize `curr` as root.
 * 2. While `curr` is not nullptr:
 *    - If `curr->left` is nullptr:
 *        * Visit `curr` (add `curr->val` to result).
 *        * Move to `curr->right`.
 *    - Else (`curr->left` exists):
 *        * Find the in-order predecessor of `curr` (rightmost node in left subtree).
 *        * If predecessor's right pointer is nullptr:
 *            - Create a temporary thread (link): `predecessor->right = curr`.
 *            - Move `curr` to its left child (`curr = curr->left`).
 *        * If predecessor's right pointer points to `curr` (thread already exists):
 *            - Remove the thread (restore tree structure): `predecessor->right = nullptr`.
 *            - Visit `curr` (add `curr->val` to result).
 *            - Move `curr` to its right child (`curr = curr->right`).
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the total number of nodes in the binary tree.
 * - Each edge is traversed at most 3 times (once to build thread, once to traverse, once to remove thread).
 * 
 * SPACE COMPLEXITY: O(1)
 * - Auxiliary space is strictly O(1) constant because pointers are temporarily updated to form 
 *   threads instead of using recursion or an explicit stack.
 */
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    TreeNode* curr = root;

    while (curr != nullptr) {
        if (curr->left == nullptr) {
            // No left child, process current node and go right
            result.push_back(curr->val);
            curr = curr->right;
        } else {
            // Find the in-order predecessor of curr
            TreeNode* predecessor = curr->left;
            while (predecessor->right != nullptr && predecessor->right != curr) {
                predecessor = predecessor->right;
            }

            // Make curr the right child of its in-order predecessor
            if (predecessor->right == nullptr) {
                predecessor->right = curr; // Create thread
                curr = curr->left;
            } 
            // Revert the changes made in the tree structure to restore original tree
            else {
                predecessor->right = nullptr; // Remove thread
                result.push_back(curr->val);
                curr = curr->right;
            }
        }
    }

    return result;
}

// Helper function to build a binary tree from level-order input (e.g., "1 null 2 3")
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
    cout << "--- Binary Tree Inorder Traversal (Morris Traversal - O(1) Auxiliary Space) ---" << endl;

    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    // Perform Morris Inorder Traversal
    vector<int> result = inorderTraversal(root);

    // Output Result
    cout << "Inorder Traversal: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}