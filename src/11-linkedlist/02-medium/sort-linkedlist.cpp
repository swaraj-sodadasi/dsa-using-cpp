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
     * HELPER FUNCTION: FLOYD'S TORTOISE & HARE MIDPOINT EXTRACTION
     * Isolates and severs the list into two roughly equal segments, 
     * returning the head pointer of the second sub-list.
     */
    ListNode* splitListAtMidpoint(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        ListNode* prev = nullptr;

        // Fast pointer advances at twice the speed of the slow pointer
        while (fast != nullptr && fast->next != nullptr) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }

        // Sever the link right before the midpoint to decouple the two halves
        if (prev != nullptr) {
            prev->next = nullptr;
        }

        return slow; // Midpoint node becomes the head of the second sub-list
    }

    /*
     * HELPER FUNCTION: ITERATIVE NODE REALIGNMENT MERGER
     * Stitches two pre-sorted lists together in-place using a stack-allocated dummy node.
     */
    ListNode* mergeSortedSubLists(ListNode* list1, ListNode* list2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (list1 != nullptr && list2 != nullptr) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        // Append remaining items in O(1) time
        tail->next = (list1 != nullptr) ? list1 : list2;

        return dummy.next;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DIVIDE AND CONQUER MERGE SORT OVER LIST SPACE
     * -------------------------------------------------------------------------------------
     * The objective is to sort a singly-linked list in O(N log N) time complexity 
     * using a constant O(1) auxiliary space footprint.
     * 
     * 1. WHY MERGE SORT IS OPTIMAL FOR LINKED LISTS:
     *    Unlike arrays where Quick Sort or Heap Sort are often preferred due to random access, 
     *    linked lists perform beautifully under Merge Sort. Elements are accessed sequentially, 
     *    and nodes can be reordered purely by changing pointer links, requiring zero shifting 
     *    overhead or extra buffer arrays.
     * 
     * 2. THE TOP-DOWN ARCHITECTURE:
     *    - Base Case: If the list contains 0 or 1 nodes, it is already sorted; return it immediately.
     *    - Divide Phase: Locate the list midpoint using the `splitListAtMidpoint` pointer mechanism. 
     *      By severing the tail link of the first half, we get two completely separate sub-lists.
     *    - Conquer Phase: Recursively invoke `sortList` on both independent halves.
     *    - Combine Phase: Realign and stitch the sorted sub-lists back together using the 
     *      `mergeSortedSubLists` operation.
     */
    ListNode* sortList(ListNode* head) {
        // Base Boundary Condition: Elements of size <= 1 are already structurally sorted
        if (!head || !head->next) {
            return head;
        }

        // Divide Phase: Extract the midpoint and isolate the two half matrices
        ListNode* secondHalfHead = splitListAtMidpoint(head);

        // Conquer Phase: Recursively process both independent list segments
        ListNode* sortedLeft = sortList(head);
        ListNode* sortedRight = sortList(secondHalfHead);

        // Combine Phase: Interleave and merge the two sorted halves in-place
        return mergeSortedSubLists(sortedLeft, sortedRight);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total node count inside the list. 
 *                  The list is divided in half at each depth level (log N steps), and the 
 *                  merge pass at each level takes O(N) comparisons.
 *
 * SPACE COMPLEXITY: O(log N) Call-Stack Runtime / O(1) Auxiliary - The system shifts node pointers 
 *                   in-place without allocating auxiliary nodes. The recursion stack uses log N 
 *                   frames. (Can be reduced to strict O(1) using an iterative bottom-up approach).
 */
int main() {
    int elementsCount;
    cout << "=== Divide and Conquer List Merge Sort Console ===\n";
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

    // Print initial list layout structure
    cout << "\nOriginal Unsorted Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nExecuting divide-and-conquer splits and realigning node paths..." << endl;
    ListNode* sortedListHead = solver.sortList(listHead);

    // Print resulting list sequence after execution
    cout << "\nSorted Outcome Matrix:\nHead -> ";
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