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
 * CORE LOGIC (Base-10 Prefix Accumulation DFS):
 * 1. Base Case: If the current node (root) is nullptr, return 0.
 * 2. Prefix Calculation: Update current running number by shifting the accumulated 
 *    sum by one base-10 decimal place and adding current node value:
 *    `currSum = currSum * 10 + root->val`
 * 3. Leaf Node Check: If current node is a leaf (both left and right are nullptr),
 *    return the accumulated `currSum`.
 * 4. Recursive Step: Recursively calculate and return the sum of base-10 path values 
 *    from both left and right subtrees:
 *    `sumNumbersDFS(root->left, currSum) + sumNumbersDFS(root->right, currSum)`
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Each node in the tree is visited exactly once.
 * 
 * SPACE COMPLEXITY: O(H)
 * - Where H is the height of the binary tree due to recursion call stack frames.
 * - Worst case: O(n) for a completely skewed tree.
 * - Best/Average case: O(log n) for a balanced tree.
 */

int sumNumbersDFS(TreeNode* root, int currSum) {
    if (root == nullptr) {
        return 0;
    }

    // Accumulate base-10 digit prefix
    currSum = currSum * 10 + root->val;

    // Return current path value if at leaf node
    if (root->left == nullptr && root->right == nullptr) {
        return currSum;
    }

    // Recurse on left and right subtrees
    return sumNumbersDFS(root->left, currSum) + sumNumbersDFS(root->right, currSum);
}

int sumNumbers(TreeNode* root) {
    return sumNumbersDFS(root, 0);
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
    cout << "--- Sum Root to Leaf Numbers ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    TreeNode* root = buildTreeForN(n);

    // Compute total sum of root-to-leaf numbers
    int totalSum = sumNumbers(root);

    // Output Result
    cout << "Total Sum of Root-to-Leaf Numbers: " << totalSum << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}