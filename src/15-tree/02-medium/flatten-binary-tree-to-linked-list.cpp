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
 * CORE LOGIC (In-place Morris-style Threading):
 * 1. Initialize `curr` as the root node.
 * 2. While `curr` is not nullptr:
 *    - If `curr` has a left child:
 *        * Find the rightmost node in the left subtree (`predecessor`).
 *        * Thread/link `predecessor->right` to `curr->right`.
 *        * Move the entire left subtree to the right (`curr->right = curr->left`).
 *        * Set `curr->left = nullptr`.
 *    - Move `curr` to the next right node (`curr = curr->right`).
 * 3. This transforms the binary tree in-place into a right-skewed single linked list
 *    following pre-order traversal order without extra memory allocations.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node is visited at most twice during traversal and predecessor linking.
 * 
 * SPACE COMPLEXITY: O(1)
 * - Auxiliary space is strictly constant since updates are performed in-place 
 *   by modifying existing pointers without stack or queue structures.
 */
void flatten(TreeNode* root) {
    TreeNode* curr = root;

    while (curr != nullptr) {
        if (curr->left != nullptr) {
            // Find the rightmost node in the left subtree
            TreeNode* predecessor = curr->left;
            while (predecessor->right != nullptr) {
                predecessor = predecessor->right;
            }

            // Connect predecessor's right to curr's right
            predecessor->right = curr->right;

            // Move left subtree to right and nullify left
            curr->right = curr->left;
            curr->left = nullptr;
        }
        // Move to next node along the right path
        curr = curr->right;
    }
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

// Helper function to print flattened linked list
void printFlattenedList(TreeNode* root) {
    TreeNode* curr = root;
    while (curr != nullptr) {
        cout << curr->val << (curr->right ? " -> " : "");
        curr = curr->right;
    }
    cout << endl;
}

// Helper function to free dynamically allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->right); // List is right-skewed after flattening
    delete root;
}

int main() {
    cout << "--- Flatten Binary Tree to Linked List ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Flatten Tree In-place
    flatten(root);

    // Output Result
    cout << "Flattened Linked List: ";
    printFlattenedList(root);

    // Clean up memory
    deleteTree(root);

    return 0;
}