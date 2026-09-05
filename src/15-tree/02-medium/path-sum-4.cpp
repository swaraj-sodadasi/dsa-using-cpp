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
 * CORE LOGIC (Backtracking Vector Accumulator):
 * 1. Base Case: If the current node (root) is nullptr, return immediately.
 * 2. Push state: Add the current node's value to `currentPath` and subtract 
 *    `root->val` from `remainingSum`.
 * 3. Leaf Node Check: If current node is a leaf (both left and right are nullptr) 
 *    and `remainingSum == 0`, we have found a valid root-to-leaf path -> push 
 *    `currentPath` into `allPaths`.
 * 4. Recursive Exploration: Recursively explore left and right subtrees.
 * 5. Backtrack (Pop state): Remove the current node's value from `currentPath` 
 *    before returning to the parent caller to maintain correct path context.
 * 
 * TIME COMPLEXITY: O(n^2)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - In the worst case (e.g., a complete binary tree where all paths sum to target), 
 *   copying valid path vectors of length O(H) takes O(H) time per path.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree.
 * - Memory used by the call stack and `currentPath` vector is bounded by H.
 * - Worst case: O(n) for a skewed tree, O(log n) for a balanced tree (excluding result output).
 */

void findPathsDFS(TreeNode* root, int remainingSum, vector<int>& currentPath, vector<vector<int>>& allPaths) {
    if (root == nullptr) {
        return;
    }

    // Push state
    currentPath.push_back(root->val);
    remainingSum -= root->val;

    // Check leaf condition
    if (root->left == nullptr && root->right == nullptr && remainingSum == 0) {
        allPaths.push_back(currentPath);
    } else {
        // Explore children
        findPathsDFS(root->left, remainingSum, currentPath, allPaths);
        findPathsDFS(root->right, remainingSum, currentPath, allPaths);
    }

    // Backtrack state
    currentPath.pop_back();
}

vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> allPaths;
    vector<int> currentPath;
    findPathsDFS(root, targetSum, currentPath, allPaths);
    return allPaths;
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
    cout << "--- Path Sum II (Backtracking Path Search) ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    int targetSum;
    cout << "Enter target sum: ";
    cin >> targetSum;

    // Find all paths
    vector<vector<int>> paths = pathSum(root, targetSum);

    // Output Result
    cout << "Paths matching target sum " << targetSum << ":" << endl;
    cout << "[" << endl;
    for (size_t i = 0; i < paths.size(); ++i) {
        cout << "  [ ";
        for (size_t j = 0; j < paths[i].size(); ++j) {
            cout << paths[i][j] << (j + 1 < paths[i].size() ? ", " : " ");
        }
        cout << "]" << (i + 1 < paths.size() ? "," : "") << endl;
    }
    cout << "]" << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}