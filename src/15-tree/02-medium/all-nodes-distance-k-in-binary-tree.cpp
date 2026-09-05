#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
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
 * CORE LOGIC (Parent Address Injection + Radial BFS):
 * 1. Parent Injection DFS: Traverse the tree and create a parent mapping 
 *    `unordered_map<TreeNode*, TreeNode*> parentMap` to convert the directed 
 *    binary tree into an undirected graph structure.
 * 2. Radial BFS: Start Breadth-First Search from the `target` node:
 *    - Maintain a `visited` set to prevent cycles/revisiting nodes.
 *    - Expand radially outwards layer by layer up to distance `k`.
 *    - Neighbors of any node include: `left`, `right`, and `parentMap[node]`.
 * 3. Stop BFS when `currentDistance == k`. The nodes residing in the queue at 
 *    that exact moment are all nodes at distance k from `target`.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Parent mapping DFS takes O(n) and radial BFS visits each node at most once O(n).
 * 
 * SPACE COMPLEXITY: O(n)
 * - Hash maps/sets (`parentMap`, `visited`) and queue store up to O(n) nodes.
 */

void mapParents(TreeNode* curr, TreeNode* parent, unordered_map<TreeNode*, TreeNode*>& parentMap) {
    if (curr == nullptr) return;
    if (parent != nullptr) {
        parentMap[curr] = parent;
    }
    mapParents(curr->left, curr, parentMap);
    mapParents(curr->right, curr, parentMap);
}

vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
    if (root == nullptr || target == nullptr) return {};

    // Map parent pointers
    unordered_map<TreeNode*, TreeNode*> parentMap;
    mapParents(root, nullptr, parentMap);

    // Radial BFS initialization
    queue<TreeNode*> q;
    unordered_set<TreeNode*> visited;

    q.push(target);
    visited.insert(target);

    int currentDistance = 0;

    while (!q.empty()) {
        if (currentDistance == k) break;

        int size = q.size();
        for (int i = 0; i < size; ++i) {
            TreeNode* curr = q.front();
            q.pop();

            // Check Left Child
            if (curr->left && visited.find(curr->left) == visited.end()) {
                visited.insert(curr->left);
                q.push(curr->left);
            }
            // Check Right Child
            if (curr->right && visited.find(curr->right) == visited.end()) {
                visited.insert(curr->right);
                q.push(curr->right);
            }
            // Check Parent
            if (parentMap.count(curr) && visited.find(parentMap[curr]) == visited.end()) {
                visited.insert(parentMap[curr]);
                q.push(parentMap[curr]);
            }
        }
        currentDistance++;
    }

    // Collect result nodes at distance k
    vector<int> result;
    while (!q.empty()) {
        result.push_back(q.front()->val);
        q.pop();
    }

    return result;
}

// Helper function to find node by value in binary tree
TreeNode* findNode(TreeNode* root, int val) {
    if (root == nullptr) return nullptr;
    if (root->val == val) return root;
    TreeNode* leftSearch = findNode(root->left, val);
    if (leftSearch != nullptr) return leftSearch;
    return findNode(root->right, val);
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
    cout << "--- All Nodes Distance K in Binary Tree ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    int targetVal, k;
    cout << "Enter target node value: ";
    cin >> targetVal;
    cout << "Enter distance k: ";
    cin >> k;

    TreeNode* target = findNode(root, targetVal);

    if (target == nullptr) {
        cout << "Error: Target node value not found in tree." << endl;
    } else {
        vector<int> nodesAtK = distanceK(root, target, k);

        cout << "Nodes at distance " << k << " from target node " << targetVal << ": [ ";
        for (size_t i = 0; i < nodesAtK.size(); ++i) {
            cout << nodesAtK[i] << (i + 1 < nodesAtK.size() ? ", " : " ");
        }
        cout << "]" << endl;
    }

    // Clean up memory
    deleteTree(root);

    return 0;
}