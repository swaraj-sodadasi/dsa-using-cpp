#include <iostream>
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
 * 1. Base Case: If the current node (root) is nullptr, return nullptr.
 * 2. Recursive Step: Recursively invert the left subtree and the right subtree.
 * 3. Swap Mechanics: Swap the left child pointer with the right child pointer 
 *    for the current node.
 * 4. Return the root node after performing the swap.
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the total number of nodes in the binary tree.
 * - Every node is visited once to perform the swap operation.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree.
 * - Memory is used by the call stack during recursive depth-first execution.
 * - Worst case: O(N) for a skewed tree.
 * - Best/Average case: O(log N) for a balanced tree.
 */
TreeNode* invertTree(TreeNode* root) {
    if (root == nullptr) {
        return nullptr;
    }

    // Recursively process left and right subtrees
    TreeNode* leftInverted = invertTree(root->left);
    TreeNode* rightInverted = invertTree(root->right);

    // Swap the children
    root->left = rightInverted;
    root->right = leftInverted;

    return root;
}

// Helper function to build a binary tree from level-order input (e.g., "4 2 7 1 3 6 9")
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

// Helper function to print tree in level order to display result
void printLevelOrder(TreeNode* root) {
    if (root == nullptr) {
        cout << "Empty Tree" << endl;
        return;
    }

    queue<TreeNode*> q;
    q.push(root);

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
    cout << endl;
}

// Helper function to free allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Invert Binary Tree ---" << endl;

    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    // Perform Inversion
    root = invertTree(root);

    // Output Result
    cout << "Inverted Tree (Level-Order): ";
    printLevelOrder(root);

    // Clean up memory
    deleteTree(root);

    return 0;
}