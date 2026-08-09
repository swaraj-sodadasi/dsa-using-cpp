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
     * CORE LOGIC: TARGET NODE BYPASS POINTER MANIPULATION WITH A DUMMY LINK
     * -------------------------------------------------------------------------------------
     * The objective is to remove all nodes from a singly-linked list that have a specific 
     * target value `val`, returning the adjusted head pointer.
     * 
     * 1. THE EDGE-NODE ELIMINATION DILEMMA:
     *    Standard node deletion requires updating the `next` pointer of the preceding node. 
     *    However, if the nodes to be removed are at the very front of the list, there is no 
     *    predecessor. 
     *    Instead of writing messy special cases for the head node, we anchor a stack-allocated 
     *    `dummy` node right before `head`. This ensures that every valid node in the list 
     *    consistently has a predecessor.
     * 
     * 2. SINGLE-PASS LOOK-AHEAD BYPASS MECHANICS:
     *    We maintain a trailing cursor pointer `curr` initialized to the address of `dummy`.
     *    We evaluate the downstream node `curr->next`:
     *      - Match Found: If `curr->next->val == val`, we cut out the node. We track it with a 
     *        temporary pointer `temp = curr->next`, update `curr->next = curr->next->next` to 
     *        bypass it, and immediately call `delete temp` to clean up memory. We do not advance 
     *        `curr` here because the new `curr->next` node must also be evaluated.
     *      - Safe Node: If the value does not match, we simply advance `curr` forward to `curr->next`.
     */
    ListNode* removeElements(ListNode* head, int val) {
        // Step 1: Initialize the dummy node structural framework anchor on the stack
        ListNode dummy(0);
        dummy.next = head;
        ListNode* curr = &dummy;

        // Step 2: Traverse and bypass all target nodes
        while (curr->next != nullptr) {
            if (curr->next->val == val) {
                // Target match discovered: detach, bypass, and deallocate the node
                ListNode* tempNode = curr->next;
                curr->next = curr->next->next;
                delete tempNode;
            } else {
                // Safe element: advance the trailing pointer cursor
                curr = curr->next;
            }
        }

        // The true head of the filtered list starts directly after our dummy placeholder anchor
        return dummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The look-ahead scanning engine visits and evaluates each node exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The deletion runs in-place, shifting node bounds using 
 *                   only localized pointer trackers on the stack, requiring zero extra heap allocations.
 */
int main() {
    int elementsCount;
    int targetValueToRemove;

    cout << "=== Target Node Bypass Deletion Engine ===\n";
    cout << "Enter the total number of elements to construct the list: ";
    if (!(cin >> elementsCount) || elementsCount < 0) {
        cout << "Invalid dynamic allocation sizing input." << endl;
        return 1;
    }

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (elementsCount > 0) {
        cout << "Enter the integer node values sequentially:\n";
        for (int i = 0; i < elementsCount; ++i) {
            int uniqueVal;
            cin >> uniqueVal;
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

    cout << "Enter the target integer value to remove from the list: ";
    if (!(cin >> targetValueToRemove)) {
        // Clean up memory before quick exit
        while (listHead) {
            ListNode* temp = listHead;
            listHead = listHead->next;
            delete temp;
        }
        return 1;
    }

    // Print initial list structure before removal
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nEngaging dummy head anchor and executing bypass pointer manipulation..." << endl;
    ListNode* alteredListHead = solver.removeElements(listHead, targetValueToRemove);

    // Print resulting list structure after modifications
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