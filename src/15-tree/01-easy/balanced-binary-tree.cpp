#include <iostream>
#include <algorithm>
#include <cmath>
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
 * 1. A binary tree is height-balanced if the depth of the two subtrees of every node
 *    never differs by more than 1.
 * 2. Optimized Bottom-Up Approach (Post-Order Traversal):
 *    - Recursively compute the height of the left and right subtrees.
 *    - If any subtree is unbalanced (returns -1), immediately propagate -1 upwards (short-circuiting).
 *    - If abs(leftHeight - rightHeight) > 1, the current node is unbalanced -> return -1.
 *    - Otherwise, return the height of the current subtree: 1 + max(leftHeight, rightHeight).
 * 3. This avoids redundant top-down height recalculations (O(N^2)) and achieves linear time complexity.
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the total number of nodes in the binary tree.
 * - Each node is visited at most once in a bottom-up post-order fashion.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to recursion call stack.
 * - Worst case: O(N) for a skewed tree.
 * - Best/Average case: O(log N) for a balanced tree.
 */
int checkHeight(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    // Check left subtree
    int leftHeight = checkHeight(root->left);
    if (leftHeight == -1) return -1; // Unbalanced left subtree

    // Check right subtree
    int rightHeight = checkHeight(root->right);
    if (rightHeight == -1) return -1; // Unbalanced right subtree

    // Height discrepancy check at current node
    if (abs(leftHeight - rightHeight) > 1) {
        return -1; // Current node is unbalanced
    }

    // Return height of current tree
    return 1 + max(leftHeight, rightHeight);
}

bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
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
    cout << "--- Balanced Binary Tree Check ---" << endl;

    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    // Verify balance property
    bool balanced = isBalanced(root);

    // Output Result
    if (balanced) {
        cout << "Result: The binary tree IS height-balanced." << endl;
    } else {
        cout << "Result: The binary tree is NOT height-balanced." << endl;
    }

    // Clean up memory
    deleteTree(root);

    return 0;
}