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
     * CORE LOGIC: CIRCULAR LINKING, LENGTH BOUNDS COUNTING, AND TARGET SPLITTING
     * -------------------------------------------------------------------------------------
     * The objective is to rotate a singly-linked list to the right by `k` places.
     * 
     * 1. THE REPETITIVE ROTATION DILEMMA:
     *    If `k` is significantly larger than the length of the list, a naive rotation 
     *    mechanism shifting one node at a time leads to severe performance degradation.
     *    Instead, we compute the exact structural length `n` of the list. This lets us 
     *    normalize the shifts down to a highly efficient minimal equivalent step size 
     *    using modulo arithmetic: `k = k % n`.
     * 
     * 2. THE CIRCULAR MATRIX LINKING primitive:
     *    We traverse the list to compute its length. At the same time, we keep track of the 
     *    last terminal tail node. Instead of ending the list there, we immediately link the 
     *    tail node's `next` pointer back onto the original `head` node. This temporarily 
     *    transforms the linear list into a perfectly closed circular ring structure.
     * 
     * 3. THE TARGET SPLITTING EDGE MECHANICS:
     *    Rotating the list to the right by `k` positions means that the new tail node will be 
     *    located exactly `n - k` nodes away from the original head pointer.
     *      - We advance a separation tracking pointer exactly `n - k` steps through the ring.
     *      - The node sitting right after this cursor (`splitCursor->next`) is designated as the 
     *        new absolute `head` of our rotated list.
     *      - Finally, we sever the circular loop by setting `splitCursor->next = nullptr` to 
     *        properly restore the linear sequence.
     */
    ListNode* rotateRight(ListNode* head, int k) {
        // Base Boundary Condition: Empty structures, single elements, or zero steps need no mutations
        if (!head || !head->next || k == 0) {
            return head;
        }

        // Step 1: Calculate list length bounds and locate the terminal tail node
        ListNode* oldTail = head;
        int listLength = 1;
        while (oldTail->next != nullptr) {
            oldTail = oldTail->next;
            listLength++;
        }

        // Normalize k to protect against redundant full ring-cycle passes
        k = k % listLength;
        if (k == 0) {
            return head; // The final position remains unchanged
        }

        // Step 2: Form a circular ring by connecting the tail back to the head node
        oldTail->next = head;

        // Step 3: Advance to the target split index position (n - k steps from head)
        ListNode* splitCursor = head;
        for (int i = 1; i < (listLength - k); ++i) {
            splitCursor = splitCursor->next;
        }

        // Establish the new head from the split boundary and break the circular link
        ListNode* newHead = splitCursor->next;
        splitCursor->next = nullptr;

        return newHead;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The initial length check scans through all N nodes, and the separation pass 
 *                  travels at most N nodes to find the split index point, running in linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The rotation mutates node bounds completely in-place by 
 *                   shifting pointer connections, consuming zero extra dynamic heap allocations.
 */
int main() {
    int elementsCount;
    int rotationOffsetSteps;

    cout << "=== Circular Split List Rotation Engine ===\n";
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

    cout << "Enter the rotation step value (k): ";
    if (!(cin >> rotationOffsetSteps) || rotationOffsetSteps < 0) {
        cout << "Invalid rotation calculation inputs." << endl;
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
    cout << "\nNormalizing translation metrics, mapping circular bounds, and splitting links..." << endl;
    ListNode* rotatedHeadNode = solver.rotateRight(listHead, rotationOffsetSteps);

    // Print resulting list sequence after execution
    cout << "\nRotated Outcome Matrix:\nHead -> ";
    displayCursor = rotatedHeadNode;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (rotatedHeadNode) {
        ListNode* tempNode = rotatedHeadNode;
        rotatedHeadNode = rotatedHeadNode->next;
        delete tempNode;
    }

    return 0;
}