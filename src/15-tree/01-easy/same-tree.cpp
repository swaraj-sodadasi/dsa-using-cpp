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
 * 1. Base Cases:
 *    - If both nodes (p and q) are nullptr, they are structurally identical at this point -> return true.
 *    - If only one of the nodes is nullptr while the other is not, the structure differs -> return false.
 *    - If the values at current nodes p and q do not match -> return false.
 * 2. Recursive Step:
 *    - Recursively check if the left subtrees are identical AND the right subtrees are identical.
 * 
 * TIME COMPLEXITY: O(min(N, M))
 * - Where N and M are the number of nodes in tree p and tree q respectively.
 * - The traversal stops as soon as a mismatch is found or both trees end.
 * 
 * SPACE COMPLEXITY: O(min(H1, H2))
 * - Where H1 and H2 are the heights of tree p and tree q respectively.
 * - Memory is consumed by the recursion call stack.
 * - Worst case: O(N) for a skewed tree structure.
 * - Best/Average case: O(log N) for balanced trees.
 */
bool isSameTree(TreeNode* p, TreeNode* q) {
    // Both nodes are null
    if (p == nullptr && q == nullptr) {
        return true;
    }
    // One of the nodes is null, or values don't match
    if (p == nullptr || q == nullptr || p->val != q->val) {
        return false;
    }
    // Recursively check left and right subtrees
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// Helper function to build a binary tree from level-order input (e.g., "1 2 3" or "1 2 null")
TreeNode* buildTreeFromLevelOrder(const string& treeName) {
    cout << "Enter nodes for " << treeName << " in level-order separated by space (use 'null' or 'N' for empty nodes): ";
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
    cout << "--- Same Tree Evaluation ---" << endl;

    // Dynamic Input for Tree 1 and Tree 2
    TreeNode* p = buildTreeFromLevelOrder("Tree 1");
    TreeNode* q = buildTreeFromLevelOrder("Tree 2");

    // Perform Check
    bool identical = isSameTree(p, q);

    // Output Result
    if (identical) {
        cout << "Result: The trees are structurally identical and have the same node values." << endl;
    } else {
        cout << "Result: The trees are NOT identical." << endl;
    }

    // Clean up memory
    deleteTree(p);
    deleteTree(q);

    return 0;
}