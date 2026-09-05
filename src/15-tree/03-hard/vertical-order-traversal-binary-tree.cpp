#include <iostream>
#include <vector>
#include <map>
#include <set>
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
 * CORE LOGIC (Coordinate Mapping + Grouped Hash Multi-Set Sorting):
 * 1. We assign coordinates (col, row) to each node:
 *    - Root is at (col = 0, row = 0).
 *    - Left child is at (col - 1, row + 1).
 *    - Right child is at (col + 1, row + 1).
 * 2. Data Structure: `map<int, map<int, multiset<int>>> nodes`
 *    - Outer `map` sorts by column index `col` in ascending order.
 *    - Inner `map` sorts by row index `row` in ascending order.
 *    - `multiset` automatically maintains sorted order for nodes having identical (col, row) coordinates.
 * 3. We use BFS (Level-Order Traversal with Queue storing `{node, {col, row}}`) to populate coordinates.
 * 4. Iterate through the mapped data structure to construct the final 2D vertical order result.
 * 
 * TIME COMPLEXITY: O(n log n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Insertion into `map` and `multiset` takes logarithmic time for each node.
 * 
 * SPACE COMPLEXITY: O(n)
 * - Space allocated for coordinate maps, BFS queue, and result containers storing n nodes.
 */

vector<vector<int>> verticalTraversal(TreeNode* root) {
    vector<vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    // Map structure: col -> row -> multiset of node values
    map<int, map<int, multiset<int>>> nodes;
    
    // BFS Queue storing pair: {TreeNode*, {col, row}}
    queue<pair<TreeNode*, pair<int, int>>> q;
    q.push({root, {0, 0}});

    while (!q.empty()) {
        auto p = q.front();
        q.pop();

        TreeNode* node = p.first;
        int col = p.second.first;
        int row = p.second.second;

        nodes[col][row].insert(node->val);

        if (node->left != nullptr) {
            q.push({node->left, {col - 1, row + 1}});
        }
        if (node->right != nullptr) {
            q.push({node->right, {col + 1, row + 1}});
        }
    }

    // Extract sorted coordinate data into final 2D vector
    for (auto& colPair : nodes) {
        vector<int> colNodes;
        for (auto& rowPair : colPair.second) {
            // Insert all elements from the multiset into the column list
            colNodes.insert(colNodes.end(), rowPair.second.begin(), rowPair.second.end());
        }
        result.push_back(colNodes);
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
    cout << "--- Vertical Order Traversal of a Binary Tree ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Compute Vertical Order Traversal
    vector<vector<int>> verticalGrid = verticalTraversal(root);

    // Output Result Grid
    cout << "Vertical Order Traversal Result:" << endl;
    cout << "[" << endl;
    for (size_t i = 0; i < verticalGrid.size(); ++i) {
        cout << "  [ ";
        for (size_t j = 0; j < verticalGrid[i].size(); ++j) {
            cout << verticalGrid[i][j] << (j + 1 < verticalGrid[i].size() ? ", " : " ");
        }
        cout << "]" << (i + 1 < verticalGrid.size() ? "," : "") << endl;
    }
    cout << "]" << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}