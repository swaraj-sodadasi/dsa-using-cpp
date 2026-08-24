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
     * CORE LOGIC: ITERATIVE K-STEP INVERSION INTERLEAVED WITH K-STEP SKIPS
     * -------------------------------------------------------------------------------------
     * The objective is to reverse alternate blocks of $k$ nodes in a singly-linked list.
     * For example, the first $k$ nodes are reversed, the next $k$ nodes are left in their 
     * original order, the next $k$ nodes are reversed, and so on.
     * 
     * 1. THE DUMMY PREDECESSOR FRAMEWORK:
     *    Reversing the very first $k$-group alters the absolute head pointer of the list.
     *    To handle this without special-casing the front of the list, we allocate a stack-bound
     *    `dummy` node pointing directly to `head`. We maintain a `prev` tracking pointer
     *    initialized to this dummy node.
     * 
     * 2. K-STEP INVERSION SECTOR:
     *    We first try to find the end of the upcoming $k$-group by advancing a `curr` pointer 
     *    $k$ steps forward from `prev`. If there are fewer than $k$ nodes left, we reverse 
     *    whatever remaining nodes are available (or leave them based on variant constraints—here we 
     *    robustly handle varying residual lengths by reversing the available segment up to `nullptr`).
     *    We decouple the segment, invert the inner pointer bonds via `reverseSubList`, and stitch 
     *    the newly inverted head onto `prev->next`.
     * 
     * 3. K-STEP SKIP SECTOR:
     *    After reversing a $k$-group, our tracking cursor shifts to the tail of that reversed segment. 
     *    From there, we must skip the next alternate $k$ nodes. We advance the `prev` pointer 
     *    forward by exactly $k$ steps over this look-ahead block, keeping its original sequence 
     *    order perfectly intact. We then repeat the process until the entire list is exhausted.
     */
    ListNode* reverseAlternateKNodes(ListNode* head, int k) {
        // Base Boundary Condition: Empty lists, single nodes, or size-1 blocks need no structural alterations
        if (!head || k <= 1) {
            return head;
        }

        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;

        while (prev != nullptr && prev->next != nullptr) {
            // ---- SECTOR 1: IDENTIFY AND REVERSE THE CURRENT K-GROUP ----
            ListNode* curr = prev->next;
            ListNode* groupStart = curr;
            
            // Step forward k times to identify the exclusive tail boundary node
            for (int i = 0; i < k && curr != nullptr; ++i) {
                curr = curr->next;
            }
            ListNode* nextGroupStart = curr; // The boundary pointer for the trailing nodes

            // Execute the in-place pointer subsegment inversion pass
            prev->next = reverseSubList(groupStart, nextGroupStart);
            
            // Move our trailing cursor to the end of the reversed group (which is groupStart)
            prev = groupStart;

            // ---- SECTOR 2: SKIP THE ALTERNATING NEXT K nodes ----
            for (int i = 0; i < k && prev != nullptr; ++i) {
                prev = prev->next;
            }
        }

        return dummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  Every node link is traversed or flipped a constant number of times throughout 
 *                  the alternating inversion and skip blocks.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The mutation operates entirely in place by shifting pointer 
 *                   connections on the runtime stack, requiring zero additional heap allocations.
 */
int main() {
    int elementsCount;
    int blockSizeK;

    cout << "=== Alternate K-Node Block Inversion and Skip Engine ===\n";
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

    cout << "Enter the alternating block capacity value (k): ";
    if (!(cin >> blockSizeK) || blockSizeK <= 0) {
        cout << "Invalid structural step calculation constraints." << endl;
        // Clean up memory before failure exit
        while (listHead) { ListNode* t = listHead; listHead = listHead->next; delete t; }
        return 1;
    }

    // Print initial list sequence layout
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nExecuting alternating inversions and skipping look-ahead blocks..." << endl;
    ListNode* processedListHead = solver.reverseAlternateKNodes(listHead, blockSizeK);

    // Print resulting list sequence after execution
    cout << "\nAltered Outcome Matrix:\nHead -> ";
    displayCursor = processedListHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up remaining allocated heap memory nodes
    while (processedListHead) {
        ListNode* tempNode = processedListHead;
        processedListHead = processedListHead->next;
        delete tempNode;
    }

    return 0;
}