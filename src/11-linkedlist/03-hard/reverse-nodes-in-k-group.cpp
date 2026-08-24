#include <iostream>

using namespace std;

// Definition for singly-linked list node structure.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
private:
    /*
     * HELPER FUNCTION: POINTER-INVERSION SUB-LIST REVERSAL
     * Inverts a specific sub-list segment in-place.
     * Reverses up to the 'end' boundary node (exclusive) and returns the new sub-list head.
     */
    ListNode* reverseSubList(ListNode* start, ListNode* end) {
        ListNode* prevNode = end; // The first inverted node will point forward to the 'end' anchor
        ListNode* currNode = start;
        ListNode* nextNode = nullptr;

        while (currNode != end) {
            nextNode = currNode->next;
            currNode->next = prevNode;
            prevNode = currNode;
            currNode = nextNode;
        }
        return prevNode; // Returns the new head pointer of this reversed block segment
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: K-LENGTH BLOCK VALIDITY CHECK & IN-PLACE HEAD-TAIL REVERSAL CHAINS
     * -------------------------------------------------------------------------------------
     * The objective is to reverse the nodes of a singly-linked list $k$ at a time and 
     * return its modified head. If the number of nodes left is not a multiple of $k$, 
     * the remaining left-out nodes at the end must maintain their original structural order.
     * 
     * 1. THE DUMMY PREDECESSOR SEGMENT ENGINE:
     *    Reversing the very first $k$-group alters the absolute head entry pointer of the list. 
     *    To handle this smoothly without messy conditional exceptions, we allocate a stack-bound 
     *    `dummy` node pointing directly to `head`. We maintain a `groupPrev` tracking pointer 
     *    initialized to this dummy node.
     * 
     * 2. THE K-LENGTH BLOCK VALIDITY CHECK:
     *    Before attempting any structural modifications on a group, we run a quick look-ahead 
     *    scan. We advance a tracking pointer `groupEnd` exactly $k$ steps forward from `groupPrev`. 
     *    If `groupEnd` hits `nullptr` before reaching $k$ steps, it means the remaining subsegment 
     *    is shorter than $k$. Following the problem rules, we leave it untouched and stop processing.
     * 
     * 3. IN-PLACE SEGMENT REVERSAL CHAIN STITCHING:
     *    When a block is confirmed valid, we mark its boundaries:
     *      - `groupStart` = `groupPrev->next` (The current head of this subsegment)
     *      - `nextGroupStart` = `groupEnd->next` (The head of the next subsegment downstream)
     * 
     *    We pass `groupStart` and `nextGroupStart` into our sub-list reversal function. This function 
     *    flips the internal links of the $k$-group in-place. Finally, we stitch the reversed block 
     *    back into the main sequence by pointing `groupPrev->next` to the new head of the reversed group, 
     *    and we update `groupPrev` to `groupStart` to get ready for the next group.
     */
    ListNode* reverseKGroup(ListNode* head, int k) {
        // Base Boundary Condition: Empty lists, single entries, or size-1 group requests need no changes
        if (!head || k <= 1) {
            return head;
        }

        // Step 1: Initialize the dummy framework anchor on the stack frame
        ListNode dummy(0);
        dummy.next = head;
        ListNode* groupPrev = &dummy;

        while (true) {
            // Step 2: Run the look-ahead validity check to find the kth node from the current group position
            ListNode* groupEnd = groupPrev;
            for (int i = 0; i < k; ++i) {
                groupEnd = groupEnd->next;
                if (groupEnd == nullptr) {
                    // Remaining node fragment is shorter than k; keep original order and terminate
                    return dummy.next;
                }
            }

            // Capture boundary anchors before mutating the chain links
            ListNode* groupStart = groupPrev->next;
            ListNode* nextGroupStart = groupEnd->next;

            // Step 3: Execute the in-place subsegment inversion pass
            // groupEnd->next is passed as the exclusive upper bound to terminate inversion precisely
            groupPrev->next = reverseSubList(groupStart, nextGroupStart);

            // The old groupStart node has shifted positions to become the tail of the reversed group
            groupPrev = groupStart;
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The look-ahead check visits each node once, and the reversal pass flips 
 *                  each link once, running in strict linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The mutation runs entirely in-place by weaving existing 
 *                   pointer connections on the stack, consuming zero additional heap allocations.
 */
int main() {
    int elementsCount;
    int groupSizeK;

    cout << "=== K-Group Block In-Place Reversal Chain Engine ===\n";
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

    cout << "Enter the group capacity size constraint value (k): ";
    if (!(cin >> groupSizeK) || groupSizeK <= 0) {
        cout << "Invalid group size constraint matrix parameter configuration." << endl;
        // Clean up memory before failure exit
        while (listHead) { ListNode* t = listHead; listHead = listHead->next; delete t; }
        return 1;
    }

    // Print initial list structure configuration
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nScanning look-ahead blocks and executing segment pointer inversions..." << endl;
    ListNode* modifiedListHead = solver.reverseKGroup(listHead, groupSizeK);

    // Print resulting list sequence after execution
    cout << "\nReversed K-Group Outcome Matrix:\nHead -> ";
    displayCursor = modifiedListHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (modifiedListHead) {
        ListNode* tempNode = modifiedListHead;
        modifiedListHead = modifiedListHead->next;
        delete tempNode;
    }

    return 0;
}