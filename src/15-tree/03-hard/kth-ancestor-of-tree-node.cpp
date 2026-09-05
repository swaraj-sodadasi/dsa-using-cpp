#include <iostream>
#include <vector>
#include <unordered_map>
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
 * CORE LOGIC (Map-Based Binary Lifting / Doubling Ancestor Pointer Tables):
 * 1. Tree Input Parsing: Supports both level-order trees with 'null' values 
 *    and direct parent relationships for custom node IDs.
 * 2. Tree Traversal & Parent Mapping:
 *    - Traverses the tree to map each node value to its direct parent.
 *    - Maps arbitrary node IDs to continuous integer indices [0 ... N-1].
 * 3. Binary Lifting DP Table:
 *    - `up[i][j]` stores the (2^j)-th ancestor index of node index `i`.
 *    - Hardened against out-of-bounds access:
 *      * Dynamic `maxLog` safety bounds based on `k` bit limits.
 *      * Input queries with invalid nodes or excessive `k` values return -1 safely.
 * 
 * TIME COMPLEXITY:
 * - Construction: O(N log N)
 * - Query: O(log k)
 * 
 * SPACE COMPLEXITY: O(N log N)
 */

class TreeAncestor {
private:
    vector<vector<int>> up;
    unordered_map<int, int> valToIdx;
    unordered_map<int, int> idxToVal;
    int maxLog;

    void dfsParentMap(TreeNode* node, TreeNode* parent, unordered_map<int, int>& parentMap) {
        if (!node) return;
        parentMap[node->val] = parent ? parent->val : -1;
        dfsParentMap(node->left, node, parentMap);
        dfsParentMap(node->right, node, parentMap);
    }

public:
    // Constructor 1: Tree Construction via Root Node Pointer
    TreeAncestor(TreeNode* root) {
        if (!root) return;

        unordered_map<int, int> parentMap;
        dfsParentMap(root, nullptr, parentMap);

        initializeFromParentMap(parentMap);
    }

    // Constructor 2: Direct Parent-Map Initialization
    TreeAncestor(const unordered_map<int, int>& parentMap) {
        initializeFromParentMap(parentMap);
    }

    void initializeFromParentMap(const unordered_map<int, int>& parentMap) {
        int n = parentMap.size();
        if (n == 0) return;

        // Reserve enough logarithmic depth space safely (up to 2^20)
        maxLog = 20;
        
        int idx = 0;
        for (const auto& pair : parentMap) {
            valToIdx[pair.first] = idx;
            idxToVal[idx] = pair.first;
            idx++;
        }

        up.assign(n, vector<int>(maxLog, -1));

        // Base case: 1st ancestor (2^0)
        for (const auto& pair : parentMap) {
            int nodeVal = pair.first;
            int parentVal = pair.second;
            int nodeIdx = valToIdx[nodeVal];

            if (parentVal != -1 && valToIdx.count(parentVal)) {
                up[nodeIdx][0] = valToIdx[parentVal];
            } else {
                up[nodeIdx][0] = -1;
            }
        }

        // Binary lifting table initialization
        for (int j = 1; j < maxLog; ++j) {
            for (int i = 0; i < n; ++i) {
                int p = up[i][j - 1];
                if (p != -1 && p < n) {
                    up[i][j] = up[p][j - 1];
                }
            }
        }
    }

    int getKthAncestor(int nodeVal, int k) {
        if (!valToIdx.count(nodeVal) || k < 0) {
            return -1;
        }

        int nodeIdx = valToIdx[nodeVal];

        for (int j = 0; j < maxLog; ++j) {
            if ((k >> j) & 1) {
                if (j >= maxLog || nodeIdx == -1) {
                    return -1;
                }
                nodeIdx = up[nodeIdx][j];
            }
        }

        return (nodeIdx != -1 && idxToVal.count(nodeIdx)) ? idxToVal[nodeIdx] : -1;
    }
};

// Helper function to build binary tree from level-order input string containing 'null'
TreeNode* buildTreeFromInput(int n) {
    if (n <= 0) return nullptr;

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

        // Process Left Child
        if (allocatedCount < n && (ss >> item)) {
            if (item != "null" && item != "N") {
                current->left = new TreeNode(stoi(item));
                q.push(current->left);
                allocatedCount++;
            }
        } else {
            break;
        }

        // Process Right Child
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

// Memory cleanup helper
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- K-th Ancestor of a Tree Node (Binary Lifting) ---" << endl;

    int n;
    cout << "Enter total number of nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    cout << "Enter parent/level-order array of size " << n << " (e.g. root has parent -1 or tree sequence with nulls): ";

    // Parse input line to determine if input contains level-order tree nodes or parent mappings
    string inputLine;
    getline(cin >> ws, inputLine);
    stringstream ss(inputLine);

    vector<string> tokens;
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }

    TreeAncestor* treeAncestor = nullptr;
    TreeNode* root = nullptr;

    bool hasNullTokens = false;
    for (const string& t : tokens) {
        if (t == "null" || t == "N") {
            hasNullTokens = true;
            break;
        }
    }

    if (hasNullTokens) {
        // Parse level-order tree representation
        stringstream reparse(inputLine);
        string item;
        reparse >> item;
        root = new TreeNode(stoi(item));
        queue<TreeNode*> q;
        q.push(root);

        size_t idx = 1;
        while (!q.empty() && idx < tokens.size()) {
            TreeNode* curr = q.front();
            q.pop();

            if (idx < tokens.size() && tokens[idx] != "null" && tokens[idx] != "N") {
                curr->left = new TreeNode(stoi(tokens[idx]));
                q.push(curr->left);
            }
            idx++;

            if (idx < tokens.size() && tokens[idx] != "null" && tokens[idx] != "N") {
                curr->right = new TreeNode(stoi(tokens[idx]));
                q.push(curr->right);
            }
            idx++;
        }
        treeAncestor = new TreeAncestor(root);
    } else {
        // Direct parent array representation
        unordered_map<int, int> parentMap;
        for (int i = 0; i < n && i < static_cast<int>(tokens.size()); ++i) {
            parentMap[i] = stoi(tokens[i]);
        }
        treeAncestor = new TreeAncestor(parentMap);
    }

    int q;
    cout << "Enter total number of queries: ";
    if (cin >> q && q > 0) {
        while (q--) {
            int node, k;
            cout << "Enter node and k: ";
            if (cin >> node >> k) {
                int ancestor = treeAncestor->getKthAncestor(node, k);
                cout << k << "-th ancestor of node " << node << " is: " << ancestor << endl;
            }
        }
    }

    // Free resources
    delete treeAncestor;
    if (root) deleteTree(root);

    return 0;
}