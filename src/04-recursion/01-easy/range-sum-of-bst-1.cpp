#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

/*
CORE LOGIC (RECURSIVE NODE BOUNDS PRUNING):
- The objective is to calculate the sum of values of all nodes in a Binary Search Tree (BST) that fall within a closed range `[low, high]`.
- **Exploiting the BST Property for Range Pruning**:
  - In a standard binary tree, we would have to visit every node to check if its value falls inside the range. 
  - In a BST, for any given node, all values in its left subtree are less than or equal to its value, and all values in its right subtree are greater than or equal to it.
  - This allows us to perform high-efficiency **bounds pruning**:
    1. If `root->val > low`, it means there could still be valid nodes in the left subtree. We recursively visit `root->left`. If `root->val <= low`, the entire left subtree will be strictly less than `low`, so we completely prune it.
    2. If `root->val < high`, it means there could still be valid nodes in the right subtree. We recursively visit `root->right`. If `root->val >= high`, the entire right subtree will be strictly greater than `high`, so we completely prune it.
- **Value Accumulation**:
  - If the current node's value falls inside the range (`low <= root->val && root->val <= high`), we include it in our total sum.

TIME COMPLEXITY: O(N) worst-case - Where N is the total number of nodes in the tree. If the range `[low, high]` is wide enough to cover all nodes, we must visit each one. However, for tight ranges, the operational execution path approaches O(H), where H is the height of the tree.
SPACE COMPLEXITY: O(H) auxiliary - Space allocated on the runtime system stack to manage recursive depth call frames.
*/

// Definition for a binary tree node structure
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int rangeSumBST(TreeNode* root, int low, int high) {
    // Base case: An empty node contributes 0 to the sum
    if (root == nullptr) {
        return 0;
    }

    int currentSum = 0;

    // If the node's value falls within the target range, accumulate it
    if (root->val >= low && root->val <= high) {
        currentSum += root->val;
    }

    // Pruning Optimization: Only descend into the left branch if the current value is greater than the lower bound
    if (root->val > low) {
        currentSum += rangeSumBST(root->left, low, high);
    }

    // Pruning Optimization: Only descend into the right branch if the current value is less than the upper bound
    if (root->val < high) {
        currentSum += rangeSumBST(root->right, low, high);
    }

    return currentSum;
}

// Helper function to build a binary tree dynamically from level-order input strings
TreeNode* buildTreeFromLevelOrder(const string& input) {
    if (input.empty()) return nullptr;

    vector<string> tokens;
    stringstream ss(input);
    string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty() || tokens[0] == "null") return nullptr;

    TreeNode* root = new TreeNode(stoi(tokens[0]));
    queue<TreeNode*> q;
    q.push(root);

    size_t i = 1;
    while (!q.empty() && i < tokens.size()) {
        TreeNode* curr = q.front();
        q.pop();

        // Process Left Child
        if (i < tokens.size()) {
            if (tokens[i] != "null") {
                curr->left = new TreeNode(stoi(tokens[i]));
                q.push(curr->left);
            }
            i++;
        }

        // Process Right Child
        if (i < tokens.size()) {
            if (tokens[i] != "null") {
                curr->right = new TreeNode(stoi(tokens[i]));
                q.push(curr->right);
            }
            i++;
        }
    }

    return root;
}

// Helper function to safely deallocate tree memory and prevent resource leaks
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    string inputLine;
    cout << "Enter BST nodes in level-order format separated by spaces (e.g., 10 5 15 3 7 null 18):" << endl;
    cout << "Use 'null' to represent empty child positions." << endl;
    cout << "Input: ";
    
    if (!getline(cin, inputLine)) {
        cout << "Invalid input parsing string parameters." << endl;
        return 1;
    }

    TreeNode* root = buildTreeFromLevelOrder(inputLine);

    int low, high;
    cout << "Enter the lower bound integer (low): ";
    if (!(cin >> low)) {
        cout << "Invalid range boundary configuration input entry." << endl;
        freeTree(root);
        return 1;
    }
    cout << "Enter the upper bound integer (high): ";
    if (!(cin >> high)) {
        cout << "Invalid range boundary configuration input entry." << endl;
        freeTree(root);
        return 1;
    }

    if (low > high) {
        cout << "Boundary Error: The lower range limit cannot exceed the upper limit configuration boundaries." << endl;
        freeTree(root);
        return 1;
    }

    cout << "\nExecuting recursive node bounds pruning tracking..." << endl;
    int totalSum = rangeSumBST(root, low, high);

    cout << "The accumulated range sum of the BST within [" << low << ", " << high << "] is: " << totalSum << endl;

    // Clean up allocated heap memory resources
    freeTree(root);

    return 0;
}