#include <iostream>
#include <algorithm>
#include <climits>
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
 * CORE LOGIC (Post-Order Dynamic Path Optimization):
 * 1. A path in a binary tree is a sequence of nodes where each pair of adjacent nodes 
 *    has an edge connecting them. Path sum can include negative node values.
 * 2. Post-Order Bottom-Up Traversal:
 *    - For any current node, compute the maximum path sum from its left and right subtrees.
 *    - Ignore negative contribution paths by clamping subtree sums to 0 using `max(0, depthSum)`.
 * 3. Local Extrema Extraction (Split Path):
 *    - The maximum path through the current node as the highest ancestor (split point) is:
 *      `currentMax = root->val + leftMax + rightMax`.
 *    - Update global `maxPathSum` with `max(maxPathSum, currentMax)`.
 * 4. Return Value (Unsplit Path Propagation):
 *    - To extend the path to the parent caller, return the maximum single-branch path:
 *      `return root->val + max(leftMax, rightMax)`.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node is visited once during post-order traversal.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to recursion call stack frames.
 * - Worst case: O(n) for a completely skewed tree.
 * - Best/Average case: O(log n) for a balanced tree.
 */

int maxPathSumDFS(TreeNode* root, int& maxPath) {
    if (root == nullptr) {
        return 0;
    }

    // Compute maximum single-branch path sum from left and right subtrees (clamp negative values to 0)
    int leftMax = max(0, maxPathSumDFS(root->left, maxPath));
    int rightMax = max(0, maxPathSumDFS(root->right, maxPath));

    // Update global maximum path sum taking the current node as the root/apex of the path
    maxPath = max(maxPath, root->val + leftMax + rightMax);

    // Return maximum contribution of single branch extending to parent
    return root->val + max(leftMax, rightMax);
}

int maxPathSum(TreeNode* root) {
    int maxPath = INT_MIN;
    maxPathSumDFS(root, maxPath);
    return maxPath;
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
    cout << "--- Binary Tree Maximum Path Sum ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Compute Maximum Path Sum
    int result = maxPathSum(root);

    // Output Result
    cout << "Maximum Path Sum in Binary Tree: " << result << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}