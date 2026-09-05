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
 * 1. The diameter of a binary tree is the length of the longest path between any two nodes.
 *    This path may or may not pass through the root.
 * 2. We use Post-Order Traversal (Bottom-Up) to compute the height of subtrees:
 *    - For every node, the longest path passing through it as the highest ancestor is:
 *      (Height of Left Subtree) + (Height of Right Subtree).
 *    - We maintain a global/referenced variable `diameter` to track the maximum path length found so far.
 * 3. The function returns the height of the current node's tree:
 *    1 + max(leftHeight, rightHeight)
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the total number of nodes in the tree.
 * - Each node is visited once during the post-order traversal.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to the recursive call stack.
 * - Worst case: O(N) for a skewed tree.
 * - Best/Average case: O(log N) for a balanced tree.
 */
int calculateHeightAndDiameter(TreeNode* root, int& diameter) {
    if (root == nullptr) {
        return 0;
    }

    // Post-order traversal: compute left and right subtree heights first
    int leftHeight = calculateHeightAndDiameter(root->left, diameter);
    int rightHeight = calculateHeightAndDiameter(root->right, diameter);

    // Update diameter with the maximum path length passing through the current node
    diameter = max(diameter, leftHeight + rightHeight);

    // Return height of the tree rooted at current node
    return 1 + max(leftHeight, rightHeight);
}

int diameterOfBinaryTree(TreeNode* root) {
    int diameter = 0;
    calculateHeightAndDiameter(root, diameter);
    return diameter;
}

// Helper function to build a binary tree from level-order input (e.g., "1 2 3 4 5")
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
    cout << "--- Diameter of Binary Tree ---" << endl;

    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    // Calculate Diameter
    int result = diameterOfBinaryTree(root);

    // Output Result
    cout << "Diameter of the Binary Tree (Number of Edges): " << result << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}