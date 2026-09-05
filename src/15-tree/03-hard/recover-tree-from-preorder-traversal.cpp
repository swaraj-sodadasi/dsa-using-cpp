#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * CORE LOGIC (Stack-Based Character Dash Depth Reconstruction):
 * 1. Parse the input string containing node values prefixed with dashes ('-') 
 *    where the count of consecutive dashes represents the depth of the node.
 * 2. Use a vector/stack `vector<TreeNode*> stack` to track the current path 
 *    from the root down to the last processed node at each depth level.
 * 3. For each parsed (depth, value) pair:
 *    a. Create a new `TreeNode` with the parsed value.
 *    b. Adjust stack size: If `stack.size() > depth`, pop elements until 
 *       `stack.size() == depth`. The top node is now the parent of the new node.
 *    c. Attach node:
 *       - If the parent's `left` child is empty, set `parent->left = node`.
 *       - Otherwise, set `parent->right = node`.
 *    d. Push the new node onto the stack.
 * 4. The root of the reconstructed tree is always `stack[0]`.
 * 
 * TIME COMPLEXITY: O(L)
 * - Where L is the total length of the input traversal string.
 * - Each character is scanned once; each node is pushed and popped from stack at most once.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the maximum height (depth) of the reconstructed binary tree.
 * - Stack size is bounded by the current maximum path depth.
 */

TreeNode* recoverFromPreorder(const string& S) {
    vector<TreeNode*> stack;
    size_t i = 0;
    size_t n = S.length();

    while (i < n) {
        // Count depth based on consecutive dashes '-'
        int depth = 0;
        while (i < n && S[i] == '-') {
            depth++;
            i++;
        }

        // Parse node value
        int val = 0;
        while (i < n && isdigit(S[i])) {
            val = val * 10 + (S[i] - '0');
            i++;
        }

        TreeNode* node = new TreeNode(val);

        // Maintain stack size equal to current depth level
        while (stack.size() > static_cast<size_t>(depth)) {
            stack.pop_back();
        }

        // Attach node to its parent
        if (!stack.empty()) {
            if (stack.back()->left == nullptr) {
                stack.back()->left = node;
            } else {
                stack.back()->right = node;
            }
        }

        stack.push_back(node);
    }

    return stack.empty() ? nullptr : stack[0];
}

// Helper function to print level-order traversal for verification
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

// Helper function to free dynamically allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Recover a Tree From Preorder Traversal ---" << endl;

    string S;
    cout << "Enter the dash-formatted preorder traversal string (e.g., '1-2--3--4-5--6--7'): ";
    cin >> S;

    if (S.empty()) {
        cout << "Invalid input string." << endl;
        return 0;
    }

    // Recover tree structure
    TreeNode* root = recoverFromPreorder(S);

    // Verify constructed tree structure
    cout << "Reconstructed Tree Level-Order Verification: ";
    printLevelOrder(root);

    // Clean up memory
    deleteTree(root);

    return 0;
}