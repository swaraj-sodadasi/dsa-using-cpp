#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <utility>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BINARY TREE LEVEL ORDER TRAVERSAL (FIFO BREADTH-FIRST SEARCH)
     * -------------------------------------------------------------------------------------
     * The objective is to return the level order traversal of a binary tree's nodes' values 
     * (i.e., from left to right, level by level).
     * 
     * 1. BATCH-BY-BATCH LEVEL EXTRACTION:
     *    We utilize a FIFO queue to perform a Breadth-First Search. To separate levels 
     *    distinctly, we capture the queue's size (`levelSize`) at the start of each level iteration.
     *    This size tells us exactly how many elements belong to the current level.
     * 
     * 2. EXPANSION:
     *    For each node popped from the current level block:
     *      - We append its value to a local level container.
     *      - We push its non-null left and right child nodes to the back of the queue.
     *    Once `levelSize` nodes are processed, we save the completed level list and repeat.
     */
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;

        queue<TreeNode*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            int levelSize = static_cast<int>(nodeQueue.size());
            vector<int> currentLevel;
            currentLevel.reserve(static_cast<size_t>(levelSize));

            for (int i = 0; i < levelSize; ++i) {
                TreeNode* currentNode = nodeQueue.front();
                nodeQueue.pop();

                currentLevel.push_back(currentNode->val);

                if (currentNode->left) {
                    nodeQueue.push(currentNode->left);
                }
                if (currentNode->right) {
                    nodeQueue.push(currentNode->right);
                }
            }
            // Explicitly qualified call to std::move to prevent ADL issues or warnings
            result.push_back(std::move(currentLevel));
        }

        return result;
    }
};

// Helper to build a tree from LeetCode format BFS serialization (e.g., "1 2 3 null null 4 5")
TreeNode* buildTree(const string& input) {
    if (input.empty() || input == "null") return nullptr;

    stringstream ss(input);
    string item;
    ss >> item;

    TreeNode* root = new TreeNode(stoi(item));
    queue<TreeNode*> q;
    q.push(root);

    while (ss >> item) {
        TreeNode* current = q.front();
        q.pop();

        if (item != "null") {
            current->left = new TreeNode(stoi(item));
            q.push(current->left);
        }

        if (ss >> item) {
            if (item != "null") {
                current->right = new TreeNode(stoi(item));
                q.push(current->right);
            }
        }
    }
    return root;
}

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of nodes in the binary tree.
 *                  Every node is enqueued and dequeued exactly once.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(W) Avg - Where W is the maximum width of the tree.
 *                   In a complete binary tree, the leaf level contains up to N/2 nodes.
 */
int main() {
    cout << "=== Binary Tree Level Order Traversal BFS Engine ===\n";
    cout << "Enter the binary tree layout serialized sequentially (space-separated, e.g., '3 9 20 null null 15 7'):\n";
    
    string line;
    if (!getline(cin, line) || line.empty()) return 0;

    TreeNode* root = buildTree(line);
    Solution solver;

    cout << "\nScanning layer depths and segmenting child paths..." << endl;
    vector<vector<int>> levels = solver.levelOrder(root);

    cout << "Layer Traversal Output:\n[\n";
    for (const auto& level : levels) {
        cout << "  [";
        for (size_t i = 0; i < level.size(); ++i) {
            cout << level[i] << (i + 1 < level.size() ? ", " : "");
        }
        cout << "]\n";
    }
    cout << "]\n";

    return 0;
}