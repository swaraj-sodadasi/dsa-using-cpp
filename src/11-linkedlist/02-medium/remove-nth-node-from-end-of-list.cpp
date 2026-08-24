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
     * CORE LOGIC: TWO-POINTER FIXED GAP LOOK-AHEAD WINDOW
     * -------------------------------------------------------------------------------------
     * The objective is to remove the N-th node from the end of a singly-linked list
     * and return its head pointer.
     * 
     * 1. THE TRADITIONAL DUAL-PASS AVOIDANCE:
     *    A naive solution requires a first traversal to count the total length of the list, 
     *    followed by a second pass to step forward to the node right before the target. 
     *    Instead, we use a single-pass look-ahead window mechanism with a fixed gap size of N.
     * 
     * 2. THE DUMMY HEAD PATTERN:
     *    If the target node to be removed is the very first node (the head), the algorithm 
     *    would lack a preceding node to patch links. To eliminate edge cases cleanly, we anchor 
     *    a stack-bound `dummy` node pointing directly to `head`.
     * 
     * 3. FIXED GAP WINDOW MECHANICS:
     *    We maintain two tracking pointers initialized at the `dummy` anchor node:
     *      - `fast` (Look-Ahead): Advances forward exactly `N + 1` steps first. This creates 
     *        a structural gap window of width `N` between the two pointers.
     *      - `slow` (Trailing): Once the gap is established, both `fast` and `slow` advance 
     *        forward at the same speed (1 node per step).
     * 
     *    When `fast` hits the terminal `nullptr` boundary at the end of the list, the trailing 
     *    `slow` pointer will have landed precisely on the node *right before* the target node. 
     *    We then splice the node out (`slow->next = slow->next->next`) and deallocate it safely.
     */
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // Step 2: Initialize the dummy framework anchor on the stack
        ListNode dummy(0);
        dummy.next = head;
        ListNode* slow = &dummy;
        ListNode* fast = &dummy;

        // Step 3: Advance the fast pointer forward by N + 1 steps to build the look-ahead window gap
        for (int i = 0; i <= n; ++i) {
            if (fast == nullptr) return head; // Safety fallback check
            fast = fast->next;
        }

        // Concurrently advance both pointers forward until the look-ahead cursor hits null space
        while (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }

        // Target found: bypass the node and clean up its heap allocation
        ListNode* nodeToDelete = slow->next;
        slow->next = slow->next->next;
        delete nodeToDelete;

        // The true altered head begins directly after our dummy placeholder anchor
        return dummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The look-ahead window sweeps across the nodes in a single linear pass.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The deletion runs strictly in place using only two localized 
 *                   pointer variables on the stack, consuming zero additional heap configurations.
 */
int main() {
    int elementsCount;
    int targetNodeFromEnd;

    cout << "=== Fixed Gap Look-Ahead Window List Deletion Engine ===\n";
    cout << "Enter the total number of node elements to assign: ";
    if (!(cin >> elementsCount) || elementsCount < 0) return 1;

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (elementsCount > 0) {
        cout << "Enter the integer node values sequentially:\n";
        for (int i = 0; i < elementsCount; ++i) {
            int uniqueVal; cin >> uniqueVal;
            ListNode* freshNode = new ListNode(uniqueVal);
            if (!listHead) {
                listHead = freshNode;
                trackingTail = freshNode;
            } else {
                trackingTail->next = freshNode;
                trackingTail = freshNode;
            }
        }
    }

    cout << "Enter the value of N (1-indexed node position from the end to remove): ";
    if (!(cin >> targetNodeFromEnd) || targetNodeFromEnd <= 0 || targetNodeFromEnd > elementsCount) {
        cout << "Invalid structural index boundary input." << endl;
        // Clean up memory before failure exit
        while (listHead) { ListNode* t = listHead; listHead = listHead->next; delete t; }
        return 1;
    }

    // Print initial list sequence
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nEngaging dummy anchors and shifting the fixed gap window..." << endl;
    ListNode* alteredListHead = solver.removeNthFromEnd(listHead, targetNodeFromEnd);

    // Print resulting list sequence
    cout << "\nAltered Link Outcome Matrix:\nHead -> ";
    displayCursor = alteredListHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up remaining allocated heap memory nodes
    while (alteredListHead) {
        ListNode* tempNode = alteredListHead;
        alteredListHead = alteredListHead->next;
        delete tempNode;
    }

    return 0;
}