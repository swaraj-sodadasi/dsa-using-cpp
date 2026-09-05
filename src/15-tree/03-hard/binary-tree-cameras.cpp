#include <iostream>
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
 * CORE LOGIC (Greedy Post-Order Tri-State Selection):
 * 1. Tri-State Node Flag Definitions:
 *    - State 0: HAS_CAMERA (A camera is placed at this node).
 *    - State 1: COVERED (Node has no camera, but is monitored/covered by a child or parent).
 *    - State 2: UNCOVERED (Node is not monitored by any camera and requires coverage).
 * 2. Bottom-Up Post-Order Traversal Strategy:
 *    - Base Case: Null nodes are treated as state COVERED (1) so they don't demand cameras.
 *    - Evaluate left and right subtrees first.
 *    - Priority 1 (Greedy Placement): If either child is UNCOVERED (2), current node MUST place 
 *      a camera (returns 0) to cover its children efficiently. Increment `cameras` count.
 *    - Priority 2: If either child HAS_CAMERA (0), the current node is automatically 
 *      COVERED (returns 1).
 *    - Priority 3: Otherwise (both children are COVERED), current node is UNCOVERED (returns 2), 
 *      deferring camera placement to its parent.
 * 3. Root Edge Case: If root node returns UNCOVERED (2) after post-order traversal, 
 *    place one final camera at the root.
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node is visited exactly once during bottom-up traversal.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to recursion call stack frames.
 * - Worst case: O(n) for a completely skewed tree.
 * - Best/Average case: O(log n) for a balanced tree.
 */

enum NodeState {
    HAS_CAMERA = 0,
    COVERED = 1,
    UNCOVERED = 2
};

NodeState minCameraCoverDFS(TreeNode* root, int& cameras) {
    if (root == nullptr) {
        return COVERED; // Null nodes are inherently covered
    }

    NodeState leftState = minCameraCoverDFS(root->left, cameras);
    NodeState rightState = minCameraCoverDFS(root->right, cameras);

    // Rule 1: If any child is uncovered, place camera at current node
    if (leftState == UNCOVERED || rightState == UNCOVERED) {
        cameras++;
        return HAS_CAMERA;
    }

    // Rule 2: If any child has a camera, current node is covered
    if (leftState == HAS_CAMERA || rightState == HAS_CAMERA) {
        return COVERED;
    }

    // Rule 3: Both children are covered without cameras, current node is uncovered
    return UNCOVERED;
}

int minCameraCover(TreeNode* root) {
    int cameras = 0;
    if (minCameraCoverDFS(root, cameras) == UNCOVERED) {
        cameras++; // Root remains uncovered, requires a camera
    }
    return cameras;
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
    cout << "--- Binary Tree Cameras (Greedy Post-Order Tri-State DP) ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid node count." << endl;
        return 0;
    }

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Compute Minimum Cameras Needed
    int minCameras = minCameraCover(root);

    // Output Result
    cout << "Minimum Cameras Required to Monitor All Nodes: " << minCameras << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}