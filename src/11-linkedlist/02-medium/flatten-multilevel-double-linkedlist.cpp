#include <iostream>
#include <vector>

using namespace std;

// Definition for a Node across a multilevel doubly-linked list structure.
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;

    Node(int _val) : val(_val), prev(nullptr), next(nullptr), child(nullptr) {}
};

class Solution {
private:
    /*
     * HELPER RECURSIVE FUNCTION: FLATTEN TAIL DETECTOR
     * Recursively flattens the multilevel structural paths, returning the absolute 
     * tail node of the flattened sub-segment.
     */
    Node* flattenTail(Node* head) {
        Node* curr = head;
        Node* lastNode = head;

        while (curr != nullptr) {
            Node* nextNode = curr->next;

            // Child Node Intervention Step: Check for vertical down-level chains
            if (curr->child != nullptr) {
                // Recursively flatten the child sub-list down to its tail anchor node
                Node* childTail = flattenTail(curr->child);

                // Inject child sub-list between curr and nextNode
                curr->next = curr->child;
                curr->child->prev = curr;

                if (nextNode != nullptr) {
                    childTail->next = nextNode;
                    nextNode->prev = childTail;
                }

                // Crucial cleanup: Clear the child pointer register link field completely
                curr->child = nullptr;

                // Move our tracking cursor frame to the end of the flattened child tail segment
                lastNode = childTail;
            } else {
                lastNode = curr;
            }

            curr = nextNode;
        }

        return lastNode;
    }

public:
    Node* flatten(Node* head) {
        if (head == nullptr) {
            return nullptr;
        }
        flattenTail(head);
        return head;
    }
};

/*
 * HELPER UTILITY: DYNAMIC MULTILEVEL INTERACTIVE CREATION
 * Recursively prompts the user via the console to build list levels.
 */
Node* buildDynamicList(vector<Node*>& masterNodeTracker) {
    int nodeCount;
    cout << "Enter the node count for the current sub-list level (0 to exit level): ";
    if (!(cin >> nodeCount) || nodeCount <= 0) {
        return nullptr;
    }

    Node* levelHead = nullptr;
    Node* levelTail = nullptr;
    vector<Node*> currentLevelNodes;

    cout << "Enter " << nodeCount << " space-separated integer values: ";
    for (int i = 0; i < nodeCount; ++i) {
        int val;
        cin >> val;
        Node* freshNode = new Node(val);
        
        masterNodeTracker.push_back(freshNode);
        currentLevelNodes.push_back(freshNode);

        if (!levelHead) {
            levelHead = freshNode;
            levelTail = freshNode;
        } else {
            levelTail->next = freshNode;
            freshNode->prev = levelTail;
            levelTail = freshNode;
        }
    }

    // Process down-level branches for the nodes created on this level
    for (Node* node : currentLevelNodes) {
        char response;
        cout << "Does Node [" << node->val << "] contain a child sub-list level? (y/n): ";
        cin >> response;
        if (response == 'y' || response == 'Y') {
            cout << "\n--- Moving down one level deeper from Node [" << node->val << "] ---\n";
            node->child = buildDynamicList(masterNodeTracker);
            cout << "--- Returning back to parent level containing Node [" << node->val << "] ---\n";
        }
    }

    return levelHead;
}

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count across all tiers inside the list. 
 *                  Every node link is visited and updated a constant number of times.
 *
 * SPACE COMPLEXITY: O(D) Call-Stack Runtime / O(1) Auxiliary - The mutation operates entirely in place 
 *                   by weaving existing pointer connections. The maximum recursion call stack space 
 *                   is proportional to the maximum nesting depth tier level (D).
 */
int main() {
    cout << "=== Recursive Multilevel Doubly-Linked List Flattening Console ===\n";
    
    // Tracks all dynamic references for robust deletion lookup regardless of topology changes
    vector<Node*> masterNodeTracker;

    cout << "Initialize the primary top-level list:\n";
    Node* dynamicHead = buildDynamicList(masterNodeTracker);

    if (!dynamicHead) {
        cout << "Empty list structure defined. Exiting." << endl;
        return 0;
    }

    cout << "\nMultilevel structural list initialized successfully dynamically.\n";
    cout << "Executing vertical child sweeps and adjusting horizontal links..." << endl;

    Solution solver;
    Node* flattenedHead = solver.flatten(dynamicHead);

    // Print resulting flattened linear list sequence
    cout << "\nFlattened Outcome Matrix (Forward Scan):\nHead -> ";
    Node* curr = flattenedHead;
    Node* last = nullptr;
    while (curr != nullptr) {
        cout << "[" << curr->val << "] <-> ";
        last = curr;
        curr = curr->next;
    }
    cout << "NULL\n";

    // Verification check: Reverse pass ensuring doubly-linked integrity holds up
    cout << "Flattened Outcome Matrix (Reverse Scan):\nTail -> ";
    while (last != nullptr) {
        cout << "[" << last->val << "] <-> ";
        last = last->prev;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes dynamically using our collection tracker
    for (Node* targetNode : masterNodeTracker) {
        delete targetNode;
    }
    masterNodeTracker.clear();

    return 0;
}