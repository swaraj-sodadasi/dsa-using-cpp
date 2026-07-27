#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

/*
CORE LOGIC (SYSTEM STACK BACKTRACKING REPLICATION):
- The objective is to perform an inorder traversal (Left -> Root -> Right) of a binary tree iteratively.
- **Simulating Recursion with an Explicit Stack**:
  - In a standard recursive inorder traversal, the system call stack implicitly keeps track of parent nodes while we dive deep into left subtrees. 
  - We can replicate this behavior manually using an explicit `std::stack` tracking pointers to `TreeNode`.
- **Traversal Strategy**:
  - We maintain a pointer `curr` initialized to the root of the tree.
  - We use a loop that runs as long as `curr` is not null OR our stack is not empty.
  - **Phase 1: Deep Left Dive**: We traverse as far left as possible, pushing every node along the path onto the stack (`stack.push(curr)`), then updating `curr = curr->left`.
  - **Phase 2: Node Processing & Right Transition**: When we hit a null pointer (meaning the left subtree of the current path is fully exhausted):
    - We pop the top node from the stack. This is the deepest unvisited node.
    - We process this node by adding its value to our results vector.
    - We then shift our focus to its right child by updating `curr = curr->right`, restarting Phase 1 for this new branch.

TIME COMPLEXITY: O(N) - Where N is the total number of nodes in the binary tree. Every node is pushed onto and popped from the stack exactly once.
SPACE COMPLEXITY: O(H) auxiliary - Where H is the height of the tree. The explicit stack stores at most H nodes at any given moment.
*/

// Definition for a binary tree node structure
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> s;
    TreeNode* curr = root;

    while (curr != nullptr || !s.empty()) {
        // Reach the leftmost node of the current subtree
        while (curr != nullptr) {
            s.push(curr);
            curr = curr->left;
        }

        // Current must be nullptr at this point, pop from stack
        curr = s.top();
        s.pop();

        // Process the node value
        result.push_back(curr->val);

        // We have visited the node and its left subtree. Now, it's the right subtree's turn
        curr = curr->right;
    }

    return result;
}

// Helper function to build a binary tree dynamically from level-order input strings
TreeNode* buildTreeFromLevelOrder(const string& input) {
    if (input.empty()) return nullptr;

    vector<string> tokens;
    stringstream ss(input);
    string token;
    
    // Tokenize the input string by spaces
    while (ss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty() || tokens[0] == "null") return nullptr;

    // Create root node
    TreeNode* root = new TreeNode(stoi(tokens[0]));
    queue<TreeNode*> q;
    q.push(root);

    size_t i = 1;
    while (!q.empty() && i < tokens.size()) {
        TreeNode* curr = q.front();
        q.pop();

        // Process Left Child
        if (i < tokens.size()) {
            if (tokens[i] != "null") {
                curr->left = new TreeNode(stoi(tokens[i]));
                q.push(curr->left);
            }
            i++;
        }

        // Process Right Child
        if (i < tokens.size()) {
            if (tokens[i] != "null") {
                curr->right = new TreeNode(stoi(tokens[i]));
                q.push(curr->right);
            }
            i++;
        }
    }

    return root;
}

// Helper function to safely deallocate tree memory and prevent resource leaks
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    string inputLine;
    cout << "Enter binary tree nodes in level-order format separated by spaces." << endl;
    cout << "Use 'null' to represent empty children node positions." << endl;
    cout << "Example (1 empty-left 2 child-left-3): 1 null 2 3" << endl;
    cout << "\nInput: ";
    
    if (!getline(cin, inputLine)) {
        cout << "Invalid input reading string parameters." << endl;
        return 1;
    }

    // Dynamically build the tree out of the input sequence
    TreeNode* root = buildTreeFromLevelOrder(inputLine);

    cout << "\nExecuting iterative inorder traversal (System Stack Replication)..." << endl;
    vector<int> traversalOrder = inorderTraversal(root);

    cout << "Inorder Traversal Result:\n[ ";
    for (int val : traversalOrder) {
        cout << val << " ";
    }
    cout << "]\n";

    // Clean up allocated heap memory resources
    freeTree(root);

    return 0;
}