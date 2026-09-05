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
 * 1. Base Case: If the current node (root) is nullptr, return false.
 * 2. Leaf Node Check: If the current node is a leaf (both left and right are nullptr),
 *    check if its value equals the remaining target sum.
 * 3. Recursive Step: Subtract the current node's value from targetSum and 
 *    recursively check if a valid path exists in either the left OR right subtree:
 *    hasPathSum(root->left, targetSum - root->val) || hasPathSum(root->right, targetSum - root->val)
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the total number of nodes in the binary tree.
 * - In the worst case, every node is visited once until a valid path is found or the tree is traversed.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to the recursive stack.
 * - Worst case: O(N) for a completely skewed tree.
 * - Best/Average case: O(log N) for a balanced tree.
 */
bool hasPathSum(TreeNode* root, int targetSum) {
    if (root == nullptr) {
        return false;
    }

    // Check if it's a leaf node
    if (root->left == nullptr && root->right == nullptr) {
        return targetSum == root->val;
    }

    // Recurse down subtrees with reduced target value
    int remainingSum = targetSum - root->val;
    return hasPathSum(root->left, remainingSum) || hasPathSum(root->right, remainingSum);
}

// Helper function to build a binary tree from level-order input (e.g., "5 4 8 11 null 13 4 7 2 null null null 1")
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
    cout << "--- Path Sum Check ---" << endl;

    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    int targetSum;
    cout << "Enter the target sum: ";
    cin >> targetSum;

    // Perform Check
    bool exists = hasPathSum(root, targetSum);

    // Output Result
    if (exists) {
        cout << "Result: A root-to-leaf path with target sum " << targetSum << " EXISTS." << endl;
    } else {
        cout << "Result: NO root-to-leaf path with target sum " << targetSum << " exists." << endl;
    }

    // Clean up memory
    deleteTree(root);

    return 0;
}