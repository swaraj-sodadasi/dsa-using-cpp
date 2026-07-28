#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

/*
CORE LOGIC (BINARY TREE TOP-DOWN RECURSIVE PATH TRACKING):
- The objective is to find all root-to-leaf paths in a binary tree where the sum of the node values equals `targetSum`.
- **Top-Down Backtracking State Representation**:
  - We traverse the tree from the root down to the leaves using a depth-first search (DFS) strategy.
  - The recursive helper function `findPaths` maintains:
    - `node`: The current binary tree node pointer under consideration.
    - `remainingSum`: The target value minus the sum of all nodes selected along the current path.
    - `currentPath`: A dynamic array tracking the chronological order of node values visited from the root.
- **State Transition Strategy**:
  - **Base Case (Null Node)**: If `node` is null, we have reached past a leaf or an empty branch $\rightarrow$ return immediately.
  - **Path Accumulation**: If the node is valid, we include its value in our path: `currentPath.push_back(node->val)`.
  - **Base Case (Leaf Convergence)**: If the current node is a leaf (`!node->left && !node->right`), we check if its value exactly matches `remainingSum`. If it does, our path is valid $\rightarrow$ add a copy of `currentPath` to our global list.
  - **Branching Step**: If it's an internal node, we recursively call the function on its left and right children, updating the target to `remainingSum - node->val`.
  - **Backtracking State Recovery**: Before the function frame returns up the call stack, we pop the current node's value from `currentPath` to clean up the path for alternative branches.

TIME COMPLEXITY: O(N^2) worst-case - Where N is the total number of nodes in the binary tree. In a highly skewed tree where every node leads to a valid path, copying the paths into the result vector can take O(N) time per path. In a balanced tree, the time complexity is bounded by O(N log N).
SPACE COMPLEXITY: O(H) auxiliary - Where H is the height of the tree. The depth of the runtime recursion stack is bounded by the longest root-to-leaf path.
*/

// Definition for a binary tree node structure
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
private:
    vector<vector<int>> allPaths;

    void findPaths(TreeNode* node, int remainingSum, vector<int>& currentPath) {
        if (node == nullptr) {
            return;
        }

        // Include the current node value in the active tracking path array
        currentPath.push_back(node->val);

        // Check if we reached a leaf node and if it satisfies the remaining sum requirement
        if (node->left == nullptr && node->right == nullptr) {
            if (node->val == remainingSum) {
                allPaths.push_back(currentPath);
            }
        } else {
            // Otherwise, continue the top-down exploration down both child branches
            findPaths(node->left, remainingSum - node->val, currentPath);
            findPaths(node->right, remainingSum - node->val, currentPath);
        }

        // Backtracking state recovery step: pop element before moving back up the call stack
        currentPath.pop_back();
    }

public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        allPaths.clear();
        vector<int> currentPath;
        findPaths(root, targetSum, currentPath);
        return allPaths;
    }
};

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
    cout << "Enter binary tree nodes in level-order format separated by spaces (e.g., 5 4 8 11 null 13 4 7 2 null null 5 1):" << endl;
    cout << "Use 'null' to represent empty child positions." << endl;
    cout << "Input: ";
    
    if (!getline(cin, inputLine)) {
        cout << "Invalid input parsing string parameters." << endl;
        return 1;
    }

    TreeNode* root = buildTreeFromLevelOrder(inputLine);

    int targetSum;
    cout << "Enter the target sum integer value: ";
    if (!(cin >> targetSum)) {
        cout << "Invalid target sum configuration parameter entry." << endl;
        freeTree(root);
        return 1;
    }

    Solution solver;
    cout << "\nExecuting top-down recursive path tracking..." << endl;
    vector<vector<int>> result = solver.pathSum(root, targetSum);

    cout << "\nAll root-to-leaf paths matching target sum " << targetSum << ":\n";
    cout << "[\n";
    for (const auto& path : result) {
        cout << "  [ ";
        for (int val : path) {
            cout << val << " ";
        }
        cout << "]\n";
    }
    cout << "]\n";
    cout << "Total matching paths located: " << result.size() << endl;

    // Clean up allocated heap memory resources
    freeTree(root);

    return 0;
}