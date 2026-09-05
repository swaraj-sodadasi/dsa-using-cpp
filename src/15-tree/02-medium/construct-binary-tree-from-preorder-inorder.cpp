#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * CORE LOGIC (Hash Map Dynamic Range Division with Bounds Safety):
 * 1. Preorder traversal visits nodes in (Root -> Left -> Right) order. Thus, the first 
 *    element of preorder range is always the root of the current subtree.
 * 2. Inorder traversal visits nodes in (Left -> Root -> Right) order. Finding the root 
 *    position in inorder gives us the exact size of the left and right subtrees.
 * 3. We use an `unordered_map` to store (inorder_value -> index) mappings for O(1) root lookups.
 * 4. Fix for Segmentation Fault:
 *    - Check if `rootVal` exists in `inMap` before accessing `inMap[rootVal]`.
 *    - Verify that `rootIndex` falls within current valid boundaries `[inStart, inEnd]`.
 *    - If invalid or missing value is encountered (due to corrupted or non-matching input sequences),
 *      return `nullptr` safely to prevent out-of-bounds indexing or stack overflow recursion.
 * 5. Inorder dynamic range [inStart, inEnd] is split into:
 *    - Left Subtree Inorder Range: [inStart, rootIndex - 1]
 *    - Right Subtree Inorder Range: [rootIndex + 1, inEnd]
 * 6. Preorder dynamic range [preStart, preEnd] is split using left subtree length (`leftLen = rootIndex - inStart`):
 *    - Left Subtree Preorder Range: [preStart + 1, preStart + leftLen]
 *    - Right Subtree Preorder Range: [preStart + leftLen + 1, preEnd]
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of nodes.
 * - Hash map construction takes O(n).
 * - Each node is processed exactly once in O(1) time during recursive tree build.
 * 
 * SPACE COMPLEXITY: O(n)
 * - Hash map stores n elements: O(n).
 * - Recursion stack takes O(H) space, where H is height of the tree (O(n) worst case, O(log n) best case).
 */

TreeNode* buildTreeHelper(const vector<int>& preorder, int preStart, int preEnd,
                        const vector<int>& inorder, int inStart, int inEnd,
                        unordered_map<int, int>& inMap) {
    // Base Case: invalid boundaries
    if (preStart > preEnd || inStart > inEnd) {
        return nullptr;
    }

    // Root value is the first element of current preorder range
    int rootVal = preorder[preStart];

    // Segmentation Fault Prevention: Ensure rootVal exists in inorder map within current bounds
    if (inMap.find(rootVal) == inMap.end()) {
        return nullptr;
    }

    int rootIndex = inMap[rootVal];
    if (rootIndex < inStart || rootIndex > inEnd) {
        return nullptr; // Out of valid boundary range
    }

    TreeNode* root = new TreeNode(rootVal);
    int leftLen = rootIndex - inStart; // Number of nodes in left subtree

    // Dynamically recurse on left and right subtrees
    root->left = buildTreeHelper(preorder, preStart + 1, preStart + leftLen,
                                 inorder, inStart, rootIndex - 1, inMap);
    
    root->right = buildTreeHelper(preorder, preStart + leftLen + 1, preEnd,
                                  inorder, rootIndex + 1, inEnd, inMap);

    return root;
}

TreeNode* buildTree(const vector<int>& preorder, const vector<int>& inorder, int n) {
    if (n <= 0 || preorder.size() != static_cast<size_t>(n) || inorder.size() != static_cast<size_t>(n)) {
        return nullptr;
    }

    unordered_map<int, int> inMap;
    for (int i = 0; i < n; ++i) {
        inMap[inorder[i]] = i;
    }

    return buildTreeHelper(preorder, 0, n - 1, inorder, 0, n - 1, inMap);
}

// Helper function to print tree in Preorder to verify construction
void printPreorder(TreeNode* root) {
    if (root == nullptr) return;
    cout << root->val << " ";
    printPreorder(root->left);
    printPreorder(root->right);
}

// Helper function to free dynamically allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Construct Binary Tree from Preorder & Inorder ---" << endl;

    int n;
    cout << "Enter the total number of nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    vector<int> preorder(n);
    vector<int> inorder(n);

    cout << "Enter " << n << " space-separated values for PREORDER traversal: ";
    for (int i = 0; i < n; ++i) {
        cin >> preorder[i];
    }

    cout << "Enter " << n << " space-separated values for INORDER traversal: ";
    for (int i = 0; i < n; ++i) {
        cin >> inorder[i];
    }

    // Construct Tree safely
    TreeNode* root = buildTree(preorder, inorder, n);

    if (root == nullptr) {
        cout << "Error: Could not construct tree. Input preorder and inorder sequences may be invalid or mismatched." << endl;
    } else {
        // Verify constructed tree structure
        cout << "Re-constructed Tree Preorder Traversal Verification: ";
        printPreorder(root);
        cout << endl;

        // Clean up memory
        deleteTree(root);
    }

    return 0;
}