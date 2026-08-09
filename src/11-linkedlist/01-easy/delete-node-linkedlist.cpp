#include <iostream>

using namespace std;

// Definition for singly-linked list node structure.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: VALUE DISPLACEMENT OVERWRITE MUTATION
     * -------------------------------------------------------------------------------------
     * The objective is to delete a given node from a singly-linked list without having 
     * access to the head pointer of the list. You are only given direct access to the 
     * node that needs to be deleted.
     * 
     * 1. THE STRUCTURAL ISOLATION CHALLENGE:
     *    Standard node deletion requires changing the `next` pointer of the preceding node 
     *    to skip over the target node. Without the head pointer, we cannot traverse to find 
     *    the predecessor. 
     * 
     * 2. THE CHAMELEON OVERWRITE METRIC:
     *    Instead of shifting the prior link, we perform a value displacement overwrite. 
     *    We copy the value of the next node (`node->next->val`) directly into the target `node`. 
     *    This transforms the target node into a structural clone of its successor.
     * 
     * 3. LINK BYPASS AND DEALLOCATION:
     *    Now that the target node holds the data of the next node, the original next node 
     *    becomes redundant. We store a temporary pointer to it (`temp`), update the target 
     *    node's pointer (`node->next = node->next->next`) to bypass it entirely, and then 
     *    safely deallocate `temp` to avoid memory leaks.
     */
    void deleteNode(ListNode* node) {
        // Base Boundary Condition: The problem guarantees the node is valid and not the tail node
        if (node == nullptr || node->next == nullptr) {
            return;
        }

        // Step 2: Overwrite the active node's value with its successor's value
        ListNode* nextNode = node->next;
        node->val = nextNode->val;

        // Step 3: Shift the link structure forward to skip and bypass the redundant next node
        node->next = nextNode->next;

        // Clean up the bypassed node from heap memory
        delete nextNode;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Constant - Value displacement and link patching run immediately 
 *                  without list traversals or loop iterations.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The mutation operates entirely in place using a 
 *                   single localized cursor reference on the stack.
 */
int main() {
    int nodeCount;
    int targetDeleteIndex;

    cout << "=== Value Displacement Node Mutation Deletion Engine ===\n";
    cout << "Enter the total number of elements to construct the list (min 2): ";
    if (!(cin >> nodeCount) || nodeCount < 2) {
        cout << "Invalid sizing constraints." << endl;
        return 1;
    }

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;
    ListNode* nodeToDeleteRef = nullptr;

    cout << "Enter the integer node values sequentially:\n";
    for (int i = 0; i < nodeCount; ++i) {
        int val;
        cin >> val;
        ListNode* freshNode = new ListNode(val);
        
        if (!listHead) {
            listHead = freshNode;
            trackingTail = freshNode;
        } else {
            trackingTail->next = freshNode;
            trackingTail = freshNode;
        }
    }

    cout << "Enter the 0-indexed position of the node to delete (0 to " << nodeCount - 2 << "): ";
    if (!(cin >> targetDeleteIndex) || targetDeleteIndex < 0 || targetDeleteIndex >= nodeCount - 1) {
        cout << "Invalid deletion index configuration (cannot delete the tail node)." << endl;
        // Clean up list before exiting
        while (listHead) {
            ListNode* temp = listHead;
            listHead = listHead->next;
            delete temp;
        }
        return 1;
    }

    // Locate the internal target pointer node reference
    ListNode* searchCursor = listHead;
    for (int i = 0; i < targetDeleteIndex; ++i) {
        searchCursor = searchCursor->next;
    }
    nodeToDeleteRef = searchCursor;

    // Print list structure before execution
    cout << "\nOriginal Link Structure:\nHead -> ";
    searchCursor = listHead;
    while (searchCursor) {
        cout << "[" << searchCursor->val << "] -> ";
        searchCursor = searchCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nExecuting value displacement mutation on target node index [" << targetDeleteIndex << "]..." << endl;
    solver.deleteNode(nodeToDeleteRef);

    // Print resulting list structure after execution
    cout << "\nMutated Link Outcome Matrix:\nHead -> ";
    searchCursor = listHead;
    while (searchCursor) {
        cout << "[" << searchCursor->val << "] -> ";
        searchCursor = searchCursor->next;
    }
    cout << "NULL\n";

    // Clean up remaining allocated heap memory nodes
    while (listHead) {
        ListNode* temp = listHead;
        listHead = listHead->next;
        delete temp;
    }

    return 0;
}