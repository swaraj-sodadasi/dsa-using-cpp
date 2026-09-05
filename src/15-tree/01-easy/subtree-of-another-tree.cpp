#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * CORE LOGIC:
 * 1. To achieve optimal linear time complexity, we convert subtree structural identity verification
 *    into a Tree Serialization and Hashing problem (Merkle Tree / Subtree Hash Matching).
 * 2. Every unique subtree structure and node value combination generates a unique deterministic 
 *    string key: "val(leftKey)(rightKey)".
 * 3. We use `std::hash<string>` to map each unique string representation into an integer hash value.
 * 4. During post-order traversal of the main tree (`root`), we compute subtree hashes.
 *    If any subtree hash matches the pre-computed hash of `subRoot`, we verify if it is indeed a match.
 * 5. Utilizing hash matching avoids repetitive tree comparisons and reduces worst-case runtime.
 * 
 * TIME COMPLEXITY: O(N + M)
 * - Where N is the number of nodes in `root` and M is the number of nodes in `subRoot`.
 * - Serializing and hashing both trees takes linear time relative to their total nodes.
 * 
 * SPACE COMPLEXITY: O(N + M)
 * - Storage for subtree hash strings, map entries, and recursion call stacks for both trees.
 */

// Helper to check if two trees are strictly identical
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (p == nullptr && q == nullptr) return true;
    if (p == nullptr || q == nullptr || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// Function to compute subtree hashes for subRoot
size_t computeTreeHash(TreeNode* node, unordered_map<TreeNode*, size_t>& hashMap) {
    if (node == nullptr) return hash<string>()("#");

    string representation = to_string(node->val) + 
                            "(" + to_string(computeTreeHash(node->left, hashMap)) + ")" + 
                            "(" + to_string(computeTreeHash(node->right, hashMap)) + ")";

    size_t nodeHash = hash<string>()(representation);
    hashMap[node] = nodeHash;
    return nodeHash;
}

// Traversal function to find matching hashed subtrees
bool searchSubtreeHash(TreeNode* root, TreeNode* subRoot, size_t targetHash, const unordered_map<TreeNode*, size_t>& hashMap, bool& found) {
    if (root == nullptr || found) return false;

    // Compute hash for current root node
    string representation = to_string(root->val) + 
                            "(" + to_string(root->left ? hashMap.at(root->left) : hash<string>()("#")) + ")" + 
                            "(" + to_string(root->right ? hashMap.at(root->right) : hash<string>()("#")) + ")";

    size_t currentHash = hash<string>()(representation);

    // If hash matches targetHash, perform strict structural identity check to prevent hash collision false positives
    if (currentHash == targetHash && isSameTree(root, subRoot)) {
        found = true;
        return true;
    }

    // Traverse left and right subtrees
    return searchSubtreeHash(root->left, subRoot, targetHash, hashMap, found) ||
           searchSubtreeHash(root->right, subRoot, targetHash, hashMap, found);
}

// Post-order traversal to populate hash map bottom-up
size_t buildHashMap(TreeNode* node, unordered_map<TreeNode*, size_t>& hashMap) {
    if (node == nullptr) return hash<string>()("#");
    
    size_t leftHash = buildHashMap(node->left, hashMap);
    size_t rightHash = buildHashMap(node->right, hashMap);
    
    string representation = to_string(node->val) + 
                            "(" + to_string(leftHash) + ")" + 
                            "(" + to_string(rightHash) + ")";

    size_t nodeHash = hash<string>()(representation);
    hashMap[node] = nodeHash;
    return nodeHash;
}

bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    if (subRoot == nullptr) return true;
    if (root == nullptr) return false;

    unordered_map<TreeNode*, size_t> rootHashMap;
    unordered_map<TreeNode*, size_t> subRootHashMap;

    // Step 1: Pre-compute hashes for both trees bottom-up
    buildHashMap(root, rootHashMap);
    size_t targetHash = computeTreeHash(subRoot, subRootHashMap);

    // Step 2: Search for target hash match in root tree
    bool found = false;
    searchSubtreeHash(root, subRoot, targetHash, rootHashMap, found);
    return found;
}

// Helper function to build a binary tree from level-order input
TreeNode* buildTreeFromLevelOrder(const string& treeName) {
    cout << "Enter nodes for " << treeName << " in level-order separated by space (use 'null' or 'N' for empty nodes): ";
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

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        if (ss >> item) {
            if (item != "null" && item != "N") {
                current->left = new TreeNode(stoi(item));
                q.push(current->left);
            }
        } else {
            break;
        }

        if (ss >> item) {
            if (item != "null" && item != "N") {
                current->right = new TreeNode(stoi(item));
                q.push(current->right);
            }
        } else {
            break;
        }
    }
    return root;
}

// Helper function to free allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Subtree Verification (Hashed Subtree Traversal) ---" << endl;

    // Dynamic Input for Main Tree and Target Subtree
    TreeNode* root = buildTreeFromLevelOrder("Main Tree (root)");
    TreeNode* subRoot = buildTreeFromLevelOrder("Target Subtree (subRoot)");

    // Perform Subtree Check
    bool result = isSubtree(root, subRoot);

    // Output Result
    if (result) {
        cout << "Result: The target tree IS a valid subtree of the main tree." << endl;
    } else {
        cout << "Result: The target tree is NOT a subtree of the main tree." << endl;
    }

    // Clean up memory
    deleteTree(root);
    deleteTree(subRoot);

    return 0;
}