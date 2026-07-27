#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

/*
CORE LOGIC (RECURSIVE PATH SELECTION):
- The objective is to find a node in a Binary Search Tree (BST) whose value equals `val` and return the subtree rooted with that node.
- **BST Property Exploitation**:
  - In a standard binary tree, searching takes linear time because we must check both branches.
  - A BST maintains an ordered key structure: for any given node, all keys in the left subtree are strictly less than the node's key, and all keys in the right subtree are strictly greater.
  - This allows us to make a binary decision at each step, pruning half of the remaining search space.
- **Recursive Decomposition Strategy**:
  - **Base Cases**: 
    1. If the current `root` node is null, the target value does not exist in the tree $\rightarrow$ return `nullptr`.
    2. If `root->val == val`, we have found our target node $\rightarrow$ return `root`.
  - **Path Selection**:
    - If `val < root->val`, the target must reside in the left branch $\rightarrow$ return `searchBST(root->left, val)`.
    - If `val > root->val`, the target must reside in the right branch $\rightarrow$ return `searchBST(root->right, val)`.

TIME COMPLEXITY: O(H) - Where H is the height of the BST. In a balanced tree, this runs in optimal logarithmic time O(log N). In a skewed/degenerate tree, it scales to linear time O(N).
SPACE COMPLEXITY: O(H) auxiliary - Space required on the system runtime call stack for the deep recursive search path frames.
*/

// Definition for a binary tree node structure
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* searchBST(TreeNode* root, int val) {
    // Base Case: The value is not present or we found the exact node matches the value
    if (root == nullptr || root->val == val) {
        return root;
    }

    // Path selection based on the Binary Search Tree structural property
    if (val < root->val) {
        return searchBST(root->left, val);
    } else {
        return searchBST(root->right, val);
    }
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

// Helper function to print a subtree in level-order to visualize the result
void printLevelOrder(TreeNode* root) {
    if (!root) {
        cout << "[ ]" << endl;
        return;
    }
    queue<TreeNode*> q;
    q.push(root);
    cout << "[ ";
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        if (curr) {
            cout << curr->val << " ";
            q.push(curr->left);
            q.push(curr->right);
        } else {
            // Only print null if there are valid nodes remaining in the queue queue
            // For cleaner look in this simple output, we skip printing excessive trailing nulls
        }
    }
    cout << "]" << endl;
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
    cout << "Enter BST nodes in level-order format separated by spaces (e.g., 4 2 7 1 3):" << endl;
    cout << "Use 'null' to represent empty child positions." << endl;
    cout << "Input: ";
    
    if (!getline(cin, inputLine)) {
        cout << "Invalid input parsing string parameters." << endl;
        return 1;
    }

    TreeNode* root = buildTreeFromLevelOrder(inputLine);

    int targetValue;
    cout << "Enter the integer value to search for within the BST: ";
    if (!(cin >> targetValue)) {
        cout << "Invalid search value entry input configuration." << endl;
        freeTree(root);
        return 1;
    }

    cout << "\nExecuting recursive path selection search..." << endl;
    TreeNode* resultSubtree = searchBST(root, targetValue);

    if (resultSubtree != nullptr) {
        cout << "Target value " << targetValue << " located successfully!" << endl;
        cout << "Subtree rooted at the matched node: ";
        printLevelOrder(resultSubtree);
    } else {
        cout << "Target value " << targetValue << " was not found in the BST (returns NULL)." << endl;
    }

    // Clean up allocated heap memory resources
    freeTree(root);

    return 0;
}