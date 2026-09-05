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
 *    - If root1 is nullptr, return root2 (the subtree from root2 becomes the merged node).
 *    - If root2 is nullptr, return root1 (the subtree from root1 becomes the merged node).
 * 2. Overlapping Node Summarization:
 *    - When both root1 and root2 are non-null, sum their values and store in root1->val.
 * 3. Recursive Step:
 *    - Recursively merge the left children: root1->left = mergeTrees(root1->left, root2->left)
 *    - Recursively merge the right children: root1->right = mergeTrees(root1->right, root2->right)
 * 4. Return root1 as the root of the newly merged tree.
 * 
 * TIME COMPLEXITY: O(min(N, M))
 * - Where N and M are the number of nodes in root1 and root2 respectively.
 * - The traversal only visits overlapping nodes between the two trees.
 * 
 * SPACE COMPLEXITY: O(min(H1, H2))
 * - Where H1 and H2 are the heights of root1 and root2 respectively.
 * - Memory is used by the recursion call stack up to the maximum overlapping depth.
 * - Worst case: O(N) for skewed overlapping trees.
 * - Best/Average case: O(log N) for balanced trees.
 */
TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
    if (root1 == nullptr) return root2;
    if (root2 == nullptr) return root1;

    // Sum overlapping node values
    root1->val += root2->val;

    // Recursively overlay left and right subtrees
    root1->left = mergeTrees(root1->left, root2->left);
    root1->right = mergeTrees(root1->right, root2->right);

    return root1;
}

// Helper function to build a binary tree from level-order input (e.g., "1 3 2 5")
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

// Helper function to print tree in level order to display merged result
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
    cout << "--- Merge Two Binary Trees ---" << endl;

    // Dynamic Input for Tree 1 and Tree 2
    TreeNode* root1 = buildTreeFromLevelOrder("Tree 1");
    TreeNode* root2 = buildTreeFromLevelOrder("Tree 2");

    // Perform Tree Overlay / Merge
    TreeNode* mergedRoot = mergeTrees(root1, root2);

    // Output Result
    cout << "Merged Tree (Level-Order): ";
    printLevelOrder(mergedRoot);

    // Clean up memory
    deleteTree(mergedRoot);

    return 0;
}