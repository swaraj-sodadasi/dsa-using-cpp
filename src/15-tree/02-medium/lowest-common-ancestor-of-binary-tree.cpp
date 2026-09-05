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
 * CORE LOGIC (Recursive Split Path Search):
 * 1. Base Cases:
 *    - If root is nullptr, return nullptr.
 *    - If root equals p or q, we have found one of the targets, so return root.
 * 2. Recursive Traversal:
 *    - Search for p and q in the left subtree: lowestCommonAncestor(root->left, p, q)
 *    - Search for p and q in the right subtree: lowestCommonAncestor(root->right, p, q)
 * 3. Split Point Decision:
 *    - If both left and right return non-null pointers, p and q are located in 
 *      different subtrees of the current node. Hence, `root` is the Lowest Common Ancestor (LCA).
 *    - If only one side returns non-null, return that non-null node upwards (both p and q 
 *      reside on that side).
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - In the worst case, every node in the tree is visited once.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree.
 * - Call stack memory proportional to tree height: O(log n) for balanced, O(n) for skewed.
 */
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr || root == p || root == q) {
        return root;
    }

    TreeNode* leftLCA = lowestCommonAncestor(root->left, p, q);
    TreeNode* rightLCA = lowestCommonAncestor(root->right, p, q);

    // If p and q are found in separate subtrees, root is the LCA
    if (leftLCA != nullptr && rightLCA != nullptr) {
        return root;
    }

    // Otherwise, return the non-null child
    return (leftLCA != nullptr) ? leftLCA : rightLCA;
}

// Helper function to find node by value in binary tree
TreeNode* findNode(TreeNode* root, int val) {
    if (root == nullptr) return nullptr;
    if (root->val == val) return root;
    TreeNode* leftSearch = findNode(root->left, val);
    if (leftSearch != nullptr) return leftSearch;
    return findNode(root->right, val);
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
    cout << "--- Lowest Common Ancestor of a Binary Tree ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    int valP, valQ;
    cout << "Enter value of target node p: ";
    cin >> valP;
    cout << "Enter value of target node q: ";
    cin >> valQ;

    TreeNode* p = findNode(root, valP);
    TreeNode* q = findNode(root, valQ);

    if (p == nullptr || q == nullptr) {
        cout << "Error: One or both target nodes do not exist in the tree." << endl;
    } else {
        TreeNode* lca = lowestCommonAncestor(root, p, q);
        if (lca != nullptr) {
            cout << "Lowest Common Ancestor of " << valP << " and " << valQ << " is: " << lca->val << endl;
        } else {
            cout << "No Lowest Common Ancestor found." << endl;
        }
    }

    // Clean up memory
    deleteTree(root);

    return 0;
}