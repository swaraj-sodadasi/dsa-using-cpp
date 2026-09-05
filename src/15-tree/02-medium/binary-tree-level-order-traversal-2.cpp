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
 * CORE LOGIC (Breadth-First Segregation via FIFO Queue with Explicit Dynamic Allocations):
 * 1. Read 'n' representing the exact total number of nodes to be allocated/processed.
 * 2. If n <= 0, return an empty result.
 * 3. Dynamically build the binary tree by consuming up to 'n' valid nodes using level-order input.
 * 4. Process the tree using standard BFS:
 *    - Use a FIFO queue to store active level nodes.
 *    - Process nodes level-by-level using queue size snapshots.
 *    - Construct each level vector dynamically and store in final result grid.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node is enqueued, processed, and popped exactly once.
 * 
 * SPACE COMPLEXITY: O(W)
 * - Auxiliary space consumed by queue is bounded by maximum tree width W <= n.
 * - Result container memory dynamically scales to hold exactly n elements.
 */
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();

            currentLevel.push_back(node->val);

            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }

        result.push_back(currentLevel);
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

    int allocatedCount = 1; // Tracks explicitly allocated nodes

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

// Helper function to free dynamically allocated memory for exact node count
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Binary Tree Level Order Traversal (Explicit n Dynamic Allocation) ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Perform Level Order Traversal
    vector<vector<int>> levels = levelOrder(root);

    // Output Result Grid
    cout << "Level Order Traversal Result:" << endl;
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