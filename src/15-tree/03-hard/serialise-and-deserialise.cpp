#include <iostream>
#include <string>
#include <sstream>
#include <queue>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * CORE LOGIC (Pre-Order Token Character Stream Architecture):
 * 1. Serialization (Tree -> String Stream):
 *    - Uses Pre-Order Traversal (Root -> Left -> Right).
 *    - Null nodes are represented by a delimiter token (e.g., "#").
 *    - Values and tokens are separated by a space or comma delimiter.
 * 2. Deserialization (String Stream -> Tree):
 *    - Reads tokens sequentially from the serialized stream using `stringstream`.
 *    - Reconstructs the exact binary tree topology using recursive pre-order building:
 *      * If token is "#", return nullptr.
 *      * Otherwise, create a new node with `stoi(token)`.
 *      * Recursively build `node->left` and `node->right` from subsequent tokens in the stream.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of nodes (including null leaves) processed.
 * - Each node is visited once during string stream creation and token parsing.
 * 
 * SPACE COMPLEXITY: O(n)
 * - String storage for serialized representation takes O(n) space.
 * - Recursion call stack during serialization and deserialization takes O(H) space, 
 *   where H is the height of the tree (O(n) worst case, O(log n) best case).
 */

class Codec {
public:
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if (root == nullptr) {
            return "# ";
        }
        return to_string(root->val) + " " + serialize(root->left) + serialize(root->right);
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeHelper(ss);
    }

private:
    TreeNode* deserializeHelper(stringstream& ss) {
        string token;
        if (!(ss >> token) || token == "#") {
            return nullptr;
        }

        TreeNode* root = new TreeNode(stoi(token));
        root->left = deserializeHelper(ss);
        root->right = deserializeHelper(ss);

        return root;
    }
};

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

// Helper function to print level-order traversal for verification
void printLevelOrder(TreeNode* root) {
    if (root == nullptr) {
        cout << "Empty Tree" << endl;
        return;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        if (current != nullptr) {
            cout << current->val << " ";
            q.push(current->left);
            q.push(current->right);
        } else {
            cout << "# ";
        }
    }
    cout << endl;
}

// Helper function to free dynamically allocated memory
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Serialize and Deserialize Binary Tree ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    Codec serde;

    // Serialize
    string serializedData = serde.serialize(root);
    cout << "Serialized Character Stream: " << serializedData << endl;

    // Deserialize
    TreeNode* deserializedRoot = serde.deserialize(serializedData);

    // Verification
    cout << "Deserialized Tree Level Order Verification: ";
    printLevelOrder(deserializedRoot);

    // Clean up memory
    deleteTree(root);
    deleteTree(deserializedRoot);

    return 0;
}