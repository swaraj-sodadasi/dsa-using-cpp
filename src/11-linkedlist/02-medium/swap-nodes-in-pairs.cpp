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
     * CORE LOGIC: ADJACENT POINTER MODIFICATION AND GROUP READJUSTMENT
     * -------------------------------------------------------------------------------------
     * The objective is to swap every two adjacent nodes in a singly-linked list in-place 
     * and return its head pointer.
     * 
     * 1. THE DUMMY HEAD PATTERN:
     *    Swapping the first two nodes alters the head pointer of the entire list. To handle
     *    this cleanly without special cases, we allocate a stack-bound `dummy` node pointing
     *    directly to `head`. This provides a reliable predecessor anchor for the initial pair.
     * 
     * 2. ADJACENT PAIR TRACKING AND LOOK-AHEAD WINDOWS:
     *    We maintain a running tracking pointer `prev` initialized to the address of `dummy`.
     *    At each step, we check if a valid adjacent pair exists downstream (`prev->next` and 
     *    `prev->next->next`). If not, the structural reconfiguration process concludes.
     * 
     * 3. THREE-POINTER LINK INVERSION MATRIX:
     *    For each valid pair, we label our targets:
     *      - `firstNode`  = `prev->next`
     *      - `secondNode` = `prev->next->next`
     * 
     *    We swap their relative positions by mutating three specific pointer fields:
     *      - `firstNode->next = secondNode->next`  (Preserves the trailing sequence path)
     *      - `secondNode->next = firstNode`        (Bends the link backward to execute the swap)
     *      - `prev->next = secondNode`              (Stitches the preceding group to the new front)
     * 
     *    Finally, we advance `prev` forward by two units (`prev = firstNode`) to set up the 
     *    next iteration.
     */
    ListNode* swapPairs(ListNode* head) {
        // Base Boundary Condition: Empty structures or single elements need zero translation
        if (!head || !head->next) {
            return head;
        }

        // Step 1: Initialize the dummy framework anchor on the stack frame
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;

        // Step 2 & 3: Run the adjacent look-ahead adjustment loops
        while (prev->next != nullptr && prev->next->next != nullptr) {
            // Label adjacent node targets inside the active look-ahead window
            ListNode* firstNode = prev->next;
            ListNode* secondNode = firstNode->next;

            // Execute the pointer modification steps
            firstNode->next = secondNode->next;
            secondNode->next = firstNode;
            prev->next = secondNode;

            // Advance the tracking cursor forward to the end of the swapped pair
            prev = firstNode;
        }

        // The true altered head begins directly after our dummy placeholder anchor
        return dummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The group adjustment sweeps across the nodes in a single linear pass.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The swap runs strictly in place by shifting pointer bonds
 *                   on the stack, consuming zero additional heap allocations.
 */
int main() {
    int elementsCount;
    cout << "=== Adjacent Pointer Modification Pair Swapper Console ===\n";
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

    // Print initial list sequence configuration
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nEngaging dummy head anchors and resetting adjacent link pairs..." << endl;
    ListNode* alteredListHead = solver.swapPairs(listHead);

    // Print resulting list sequence after execution
    cout << "\nSwapped Outcome Matrix:\nHead -> ";
    displayCursor = alteredListHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (alteredListHead) {
        ListNode* tempNode = alteredListHead;
        alteredListHead = alteredListHead->next;
        delete tempNode;
    }

    return 0;
}