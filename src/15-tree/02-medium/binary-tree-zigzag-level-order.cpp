#include <iostream>
#include <vector>
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
 * CORE LOGIC (Alternating Level Order Processing):
 * 1. Base Case: If the root is nullptr, return an empty 2D vector.
 * 2. Use a standard FIFO queue (`std::queue<TreeNode*>`) to conduct standard level-order BFS.
 * 3. Maintain a boolean flag `leftToRight` initialized to true (Level 0 goes Left -> Right).
 * 4. For each level:
 *    a. Get `levelSize = q.size()`.
 *    b. Allocate a vector `currentLevel(levelSize)`.
 *    c. Traverse `levelSize` nodes:
 *       - Pop node from queue.
 *       - Compute target index: `index = leftToRight ? i : (levelSize - 1 - i)`.
 *       - Place node value at `currentLevel[index]`.
 *       - Enqueue non-null left and right child nodes for subsequent level.
 *    d. Toggle `leftToRight = !leftToRight`.
 *    e. Store `currentLevel` into the final result 2D grid.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node is pushed into and popped from the queue exactly once. Direct index positioning is O(1).
 * 
 * SPACE COMPLEXITY: O(W)
 * - Where W is the maximum width (number of nodes at any level) of the binary tree.
 * - In the worst case (perfectly balanced tree), queue holds up to n/2 leaf nodes at the lowest level.
 */

vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();

            // Calculate positional index based on direction flag
            int index = leftToRight ? i : (levelSize - 1 - i);
            currentLevel[index] = node->val;

            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }

        result.push_back(currentLevel);
        leftToRight = !leftToRight; // Flip traversal direction for next level
    }

    return result;
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
    cout << "--- Binary Tree Zigzag Level Order Traversal ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Perform Zigzag Level Order Traversal
    vector<vector<int>> levels = zigzagLevelOrder(root);

    // Output Result Grid
    cout << "Zigzag Level Order Traversal Result:" << endl;
    cout << "[" << endl;
    for (size_t i = 0; i < levels.size(); ++i) {
        cout << "  [ ";
        for (size_t j = 0; j < levels[i].size(); ++j) {
            cout << levels[i][j] << (j + 1 < levels[i].size() ? ", " : " ");
        }
        cout << "]" << (i + 1 < levels.size() ? "," : "") << endl;
    }
    cout << "]" << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}