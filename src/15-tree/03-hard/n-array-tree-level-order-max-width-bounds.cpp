#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

// Definition for an N-ary tree node.
struct Node {
    int val;
    vector<Node*> children;

    Node(int _val) : val(_val) {}
    Node(int _val, vector<Node*> _children) : val(_val), children(_children) {}
};

/*
 * CORE LOGIC (N-ary Level-Order Traversal & Width-Tracking):
 * 1. Base Case: If the root is nullptr, return an empty 2D vector.
 * 2. Standard Breadth-First Search (BFS) using a FIFO queue (`queue<Node*>`).
 * 3. Level Segregation & Maximum Width Computation:
 *    - For each level, determine `levelSize = q.size()`.
 *    - Track max level width bounds (`maxWidth = max(maxWidth, static_cast<long long>(levelSize))`).
 *    - Process all `levelSize` nodes at the current level, collecting values in a `currentLevel` vector.
 *    - Enqueue all non-null children of each node for the next level traversal.
 * 4. Return the complete 2D level-order traversal result grid.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the N-ary tree.
 * - Every node and its child pointers are traversed and enqueued exactly once.
 * 
 * SPACE COMPLEXITY: O(W)
 * - Where W is the maximum width (number of nodes at any single level) of the N-ary tree.
 * - Queue holds at most O(W) nodes at the widest level.
 */

vector<vector<int>> levelOrder(Node* root, long long& maxWidth) {
    vector<vector<int>> result;
    maxWidth = 0;

    if (root == nullptr) {
        return result;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        size_t levelSize = q.size();
        
        // Track maximum level width bounds using long long
        maxWidth = max(maxWidth, static_cast<long long>(levelSize));

        vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (size_t i = 0; i < levelSize; ++i) {
            Node* curr = q.front();
            q.pop();

            currentLevel.push_back(curr->val);

            // Enqueue all children of current N-ary node
            for (Node* child : curr->children) {
                if (child != nullptr) {
                    q.push(child);
                }
            }
        }

        result.push_back(currentLevel);
    }

    return result;
}

// Helper function to dynamically construct N-ary tree for 'n' total non-null nodes
// Input format: Null-separated level-order children lists (LeetCode standard N-ary tree format)
// e.g., "1 null 3 2 4 null 5 6"
Node* buildNaryTreeForN(int n) {
    if (n <= 0) return nullptr;

    cout << "Enter values in null-separated N-ary level-order (e.g. '1 null 3 2 4 null 5 6'): ";
    string inputLine;
    getline(cin >> ws, inputLine);

    stringstream ss(inputLine);
    string item;

    if (!(ss >> item) || item == "null" || item == "N") {
        return nullptr;
    }

    Node* root = new Node(stoi(item));
    queue<Node*> q;
    q.push(root);

    int allocatedCount = 1;

    // Skip the delimiter null following the root
    if (ss >> item && (item == "null" || item == "N")) {
        while (!q.empty() && allocatedCount < n) {
            Node* parent = q.front();
            q.pop();

            // Read children for current parent until 'null' delimiter or string end
            while (ss >> item) {
                if (item == "null" || item == "N") {
                    break;
                }
                Node* child = new Node(stoi(item));
                parent->children.push_back(child);
                q.push(child);
                allocatedCount++;
                if (allocatedCount >= n) break;
            }
        }
    }

    return root;
}

// Helper function to free dynamically allocated memory for N-ary tree
void deleteNaryTree(Node* root) {
    if (root == nullptr) return;
    for (Node* child : root->children) {
        deleteNaryTree(child);
    }
    delete root;
}

int main() {
    cout << "--- N-ary Tree Level Order Traversal & Bounds Computation ---" << endl;

    int n;
    cout << "Enter total number of non-null nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    // Dynamic Tree Construction bounded by exact n nodes
    Node* root = buildNaryTreeForN(n);

    long long maxWidth = 0;
    vector<vector<int>> levels = levelOrder(root, maxWidth);

    // Output Result Grid
    cout << "N-ary Level Order Traversal Result:" << endl;
    cout << "[" << endl;
    for (size_t i = 0; i < levels.size(); ++i) {
        cout << "  [ ";
        for (size_t j = 0; j < levels[i].size(); ++j) {
            cout << levels[i][j] << (j + 1 < levels[i].size() ? ", " : " ");
        }
        cout << "]" << (i + 1 < levels.size() ? "," : "") << endl;
    }
    cout << "]" << endl;

    cout << "Maximum Level Width Bounds: " << maxWidth << endl;

    // Clean up memory
    deleteNaryTree(root);

    return 0;
}