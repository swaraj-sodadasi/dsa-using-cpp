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
 * CORE LOGIC (Modified Right-First Depth-First Search):
 * 1. We traverse the tree using DFS visiting the RIGHT child before the LEFT child 
 *    (Root -> Right -> Left).
 * 2. We keep track of the current depth level (`currLevel` as size_t to prevent signed/unsigned compiler warnings).
 * 3. Whenever `currLevel` equals the size of our result vector (`result.size()`), it means 
 *    we are visiting this particular level for the very first time.
 * 4. Since we prioritize the right branch, the first node we encounter at any level is 
 *    guaranteed to be the rightmost visible node. We immediately add its value to `result`.
 * 5. This approach avoids maintaining explicit queue states like standard BFS level-order tail capture,
 *    achieving optimal execution while maintaining linear runtime.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Every node in the tree is visited at most once.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree.
 * - Recursion stack memory is proportional to tree height: O(log n) for balanced, O(n) for skewed.
 */

void rightSideViewDFS(TreeNode* node, size_t currLevel, vector<int>& result) {
    if (node == nullptr) {
        return;
    }

    // First time visiting this depth level -> add rightmost node
    // Uses size_t type matching result.size() to resolve signed/unsigned comparison warnings
    if (currLevel == result.size()) {
        result.push_back(node->val);
    }

    // Prioritize right child exploration first, then left child
    rightSideViewDFS(node->right, currLevel + 1, result);
    rightSideViewDFS(node->left, currLevel + 1, result);
}

vector<int> rightSideView(TreeNode* root) {
    vector<int> result;
    rightSideViewDFS(root, 0, result);
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
    cout << "--- Binary Tree Right Side View ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Compute Right Side View
    vector<int> view = rightSideView(root);

    // Output Result
    cout << "Right Side View values: ";
    for (int val : view) {
        cout << val << " ";
    }
    cout << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}