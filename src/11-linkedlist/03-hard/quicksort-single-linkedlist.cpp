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
     * HELPER FUNCTION: POINTER REALIGNMENT PARTITION SECTOR
     * Remaps pointer chains around a selected pivot node without copying values.
     * Uses a stable multi-dummy framework to group nodes into less-than, equal-to, 
     * and greater-than sub-lists.
     */
    ListNode* partitionList(ListNode* head, ListNode* pivot, ListNode*& newHead, ListNode*& newTail) {
        // Instantiate distinct boundary anchors on the stack frame
        ListNode lessDummy(0), equalDummy(0), greaterDummy(0);
        ListNode *lessTail = &lessDummy, *equalTail = &equalDummy, *greaterTail = &greaterDummy;
        
        ListNode* curr = head;
        int pivotVal = pivot->val;
        
        // Single-pass stable splitting sweep
        while (curr != nullptr) {
            if (curr->val < pivotVal) {
                lessTail->next = curr;
                lessTail = lessTail->next;
            } else if (curr->val == pivotVal) {
                equalTail->next = curr;
                equalTail = equalTail->next;
            } else {
                greaterTail->next = curr;
                greaterTail = greaterTail->next;
            }
            curr = curr->next;
        }
        
        // Sever terminal tails to prevent lingering pointer cycles
        lessTail->next = nullptr;
        equalTail->next = nullptr;
        greaterTail->next = nullptr;
        
        // Recursively sort the isolated lower sub-list segment
        ListNode *sortedLessHead = nullptr, *sortedLessTail = nullptr;
        if (lessDummy.next != nullptr) {
            sortedLessHead = quickSortRec(lessDummy.next, sortedLessTail);
        }
        
        // Recursively sort the isolated upper sub-list segment
        ListNode *sortedGreaterHead = nullptr, *sortedGreaterTail = nullptr;
        if (greaterDummy.next != nullptr) {
            sortedGreaterHead = quickSortRec(greaterDummy.next, sortedGreaterTail);
        }
        
        // Stitch the stable segments back together into a single sequence
        // Step A: Initialize the layout boundary based on the less-than sub-list state
        if (sortedLessHead != nullptr) {
            newHead = sortedLessHead;
            sortedLessTail->next = equalDummy.next;
        } else {
            newHead = equalDummy.next;
        }
        
        // Step B: Connect the equal sub-list tail to the sorted greater-than head
        equalTail->next = sortedGreaterHead;
        
        // Step C: Establish the final consolidated tail pointer configuration
        if (sortedGreaterTail != nullptr) {
            newTail = sortedGreaterTail;
        } else {
            newTail = equalTail;
        }
        
        return newHead;
    }

    /*
     * HELPER FUNCTION: QUICK SORT RECURSIVE ENGINE
     * Performs standard divide-and-conquer processing across targeted pointer segments.
     */
    ListNode* quickSortRec(ListNode* head, ListNode*& tail) {
        // Base Case: Sub-lists containing 0 or 1 nodes are already sorted
        if (head == nullptr || head->next == nullptr) {
            tail = head;
            return head;
        }
        
        // Pivot Selection Strategy: Choose the absolute head node as the pivot element
        ListNode* pivot = head;
        
        ListNode *newHead = nullptr, *newTail = nullptr;
        partitionList(head, pivot, newHead, newTail);
        
        tail = newTail;
        return newHead;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: QUICK SORT VIA PIVOT SELECTION NODE SPLICING
     * -------------------------------------------------------------------------------------
     * The objective is to sort a singly-linked list using the Quick Sort design pattern.
     * 
     * 1. THE IN-PLACE POINTER ALTERATION CONTRACT:
     *    A naive Quick Sort on lists swaps element values inside node objects (`swap(nodeA->val, nodeB->val)`).
     *    However, value swapping scales terribly if the node structure encapsulates massive objects 
     *    or heavy data structures. Instead, we rewrite pointer links completely in place, 
     *    moving node objects across memory contexts safely.
     * 
     * 2. STABLE MULTI-DUMMY PARTITION PIPELINES:
     *    To sort duplicate values efficiently and avoid infinite recursion, we use a three-way 
     *    partition framework: `< pivot`, `== pivot`, and `> pivot`.
     *    By using three distinct dummy node anchors (`lessDummy`, `equalDummy`, `greaterDummy`), 
     *    we can dynamically append elements onto separate tracking lanes while keeping their 
     *    original relative order perfectly stable.
     * 
     * 3. DIVIDE-AND-CONQUER STITCHING PATTERN:
     *    Once the sub-lists are partitioned and recursively sorted, we re-link the chains 
     *    using our track coordinates (`sortedLessTail`, `equalTail`, etc.). This builds a 
     *    fully sorted linear array path in linear-logarithmic time.
     */
    ListNode* quickSortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        ListNode* tail = nullptr;
        return quickSortRec(head, tail);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) Average / O(N^2) Worst-Case - Where N represents the total node count. 
 *                  Average time balances out to linear-logarithmic bounds when the pivot divides 
 *                  the sets evenly. The worst-case runtime degrades to quadratic proportions if 
 *                  the input is already sorted and we uniformly select the head node as the pivot.
 *
 * SPACE COMPLEXITY: O(N) Call-Stack Runtime / O(1) Auxiliary - The system shifts node pointers 
 *                   in-place without allocating dynamic heap wrapper structures. The implicit stack 
 *                   space consumes at most N frames under extreme unbalanced split topologies.
 */
int main() {
    int elementsCount;
    cout << "=== Singly-Linked List Quick Sort Splicing Engine ===\n";
    cout << "Enter the total number of node elements to sort: ";
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

    // Print initial list structure configuration
    cout << "\nOriginal Unsorted Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nExecuting 3-way pointer partitioning and in-place segment transformations..." << endl;
    ListNode* sortedListHead = solver.quickSortList(listHead);

    // Print resulting list sequence after execution
    cout << "\nQuick-Sorted Outcome Matrix:\nHead -> ";
    displayCursor = sortedListHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (sortedListHead) {
        ListNode* tempNode = sortedListHead;
        sortedListHead = sortedListHead->next;
        delete tempNode;
    }

    return 0;
}