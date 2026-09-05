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
 * 1. A tree is symmetric if the left and right subtrees are mirror images of each other.
 * 2. We use a recursive helper function `isMirror(t1, t2)` that takes two nodes:
 *    - Base Cases:
 *      * If both t1 and t2 are nullptr, return true.
 *      * If only one is nullptr, return false (structure mismatch).
 *      * If t1->val != t2->val, return false (value mismatch).
 *    - Recursive Step (Cross Mirror Check):
 *      * Compare left child of t1 with right child of t2: isMirror(t1->left, t2->right)
 *      * AND compare right child of t1 with left child of t2: isMirror(t1->right, t2->left)
 * 3. The main function calls `isMirror(root->left, root->right)`.
 * 
 * TIME COMPLEXITY: O(N)
 * - Where N is the total number of nodes in the binary tree.
 * - In the worst case, we traverse all nodes once.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree.
 * - The call stack uses memory proportional to the height of the tree.
 * - Worst case: O(N) for a completely unbalanced/skewed tree.
 * - Best/Average case: O(log N) for a balanced tree.
 */
bool isMirror(TreeNode* t1, TreeNode* t2) {
    if (t1 == nullptr && t2 == nullptr) return true;
    if (t1 == nullptr || t2 == nullptr) return false;
    if (t1->val != t2->val) return false;

    // Dual cross-checking: t1's left with t2's right, and t1's right with t2's left
    return isMirror(t1->left, t2->right) && isMirror(t1->right, t2->left);
}

bool isSymmetric(TreeNode* root) {
    if (root == nullptr) return true;
    return isMirror(root->left, root->right);
}

// Helper function to build a binary tree from level-order input (e.g., "1 2 2 3 4 4 3")
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
    cout << "--- Symmetric Tree (Dual Cross Mirror Check) ---" << endl;

    // Dynamic Input
    TreeNode* root = buildTreeFromLevelOrder();

    // Perform Check
    bool result = isSymmetric(root);

    // Output Result
    if (result) {
        cout << "Result: The binary tree IS symmetric around its center." << endl;
    } else {
        cout << "Result: The binary tree is NOT symmetric." << endl;
    }

    // Clean up memory
    deleteTree(root);

    return 0;
}