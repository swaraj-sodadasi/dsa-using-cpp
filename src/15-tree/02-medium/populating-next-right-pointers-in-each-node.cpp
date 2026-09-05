#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

// Definition for a Node with a next pointer.
struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node(int _val) : val(_val), left(nullptr), right(nullptr), next(nullptr) {}
};

/*
 * CORE LOGIC (Level Boundary Traversal Splicing - O(1) Auxiliary Space):
 * 1. Start with `leftmost` pointing to the root (which represents the first node of the current level).
 * 2. While `leftmost->left` exists (since it's a perfect binary tree, we iterate until leaf level):
 *    - Use a pointer `head` to traverse across the current level horizontally via `next` pointers.
 *    - For each node along the level:
 *        a. Connect its left child to its right child:
 *           `head->left->next = head->right;`
 *        b. If `head->next` exists, splice across subtrees by connecting the right child 
 *           to the left child of the adjacent neighbor:
 *           `head->right->next = head->next->left;`
 *        c. Move `head` to the next node in the level: `head = head->next;`
 *    - Advance `leftmost` to the next level down: `leftmost = leftmost->left;`
 * 
 * TIME COMPLEXITY: O(n)
 * - Where n is the total number of non-null nodes in the binary tree.
 * - Every node and its child connections are processed exactly once.
 * 
 * SPACE COMPLEXITY: O(1)
 * - Auxiliary space is strictly constant since connections are established using existing 
 *   level pointers without using queues or recursive call stack frames.
 */

Node* connect(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }

    Node* leftmost = root;

    // Outer loop traverses down level by level
    while (leftmost->left != nullptr) {
        Node* head = leftmost;

        // Inner loop traverses horizontally along the current level
        while (head != nullptr) {
            // Connection 1: Children of the same parent
            head->left->next = head->right;

            // Connection 2: Cross-subtree children connection
            if (head->next != nullptr) {
                head->right->next = head->next->left;
            }

            // Move along the level
            head = head->next;
        }

        // Move to the next level down
        leftmost = leftmost->left;
    }

    return root;
}

// Helper function to dynamically construct perfect binary tree for exactly 'n' non-null nodes
Node* buildTreeForN(int n) {
    if (n <= 0) return nullptr;

    cout << "Enter values in level-order space separated (use 'null' or 'N' for empty slots): ";
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

    while (!q.empty() && allocatedCount < n) {
        Node* current = q.front();
        q.pop();

        // Process left child
        if (allocatedCount < n && (ss >> item)) {
            if (item != "null" && item != "N") {
                current->left = new Node(stoi(item));
                q.push(current->left);
                allocatedCount++;
            }
        } else {
            break;
        }

        // Process right child
        if (allocatedCount < n && (ss >> item)) {
            if (item != "null" && item != "N") {
                current->right = new Node(stoi(item));
                q.push(current->right);
                allocatedCount++;
            }
        } else {
            break;
        }
    }

    return root;
}

// Helper function to print tree levels using next pointers
void printTreeByNextPointers(Node* root) {
    Node* levelStart = root;
    while (levelStart != nullptr) {
        Node* curr = levelStart;
        cout << "[ ";
        while (curr != nullptr) {
            cout << curr->val << (curr->next ? " -> " : " ");
            curr = curr->next;
        }
        cout << " -> # ]" << endl;
        levelStart = levelStart->left;
    }
}

// Helper function to free dynamically allocated memory
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "--- Populating Next Right Pointers in Each Node ---" << endl;

    int n;
    cout << "Enter the total number of non-null nodes (n): ";
    cin >> n;

    // Dynamic Tree Construction bounded by exact n nodes
    Node* root = buildTreeForN(n);

    // Populate next pointers
    root = connect(root);

    // Output Result via Next Pointers
    cout << "Level-by-level print using 'next' pointers:" << endl;
    printTreeByNextPointers(root);

    // Clean up memory
    deleteTree(root);

    return 0;
}