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
     * CORE LOGIC: ITERATIVE NODE REALIGNMENT WITH A DUMMY HEAD
     * -------------------------------------------------------------------------------------
     * The objective is to merge two sorted singly-linked lists into a single sorted list
     * by splicing the original nodes directly together, returning the new head.
     * 
     * 1. THE DUMMY HEAD PATTERN:
     *    A common challenge when merging lists is handling the initialization of the return head 
     *    pointer, which requires separate logic to check which list has the smaller first node.
     *    By utilizing a stack-allocated `dummy` node, we provide an immutable anchor point. Our
     *    tracking pointer `tail` can immediately begin attaching elements without checking for null.
     * 
     * 2. ITERATIVE REALIGNMENT PIPELINE:
     *    We maintain a running pointer `tail` initialized to the address of `dummy`.
     *    We compare the nodes at the front of both lists (`list1` and `list2`):
     *      - The node with the smaller value is spliced directly onto `tail->next`.
     *      - We then advance that specific list pointer forward to its next element.
     *      - Finally, we advance `tail` forward to the newly added node.
     * 
     * 3. RESIDUAL TAIL ATTACHMENT:
     *    When one of the list loops terminates, the remaining list contains elements that are 
     *    guaranteed to be greater than or equal to all elements processed so far. Because the 
     *    original lists were pre-sorted, we append the rest of the non-empty list directly onto 
     *    `tail->next` in $O(1)$ constant time.
     */
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        // Step 1: Initialize the dummy node framework anchor on the stack
        ListNode dummy(0);
        ListNode* tail = &dummy;

        // Step 2: Compare front elements and stitch nodes in place
        while (list1 != nullptr && list2 != nullptr) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next; // Move forward along the newly constructed list
        }

        // Step 3: Append the remaining non-empty list segment in O(1) time
        tail->next = (list1 != nullptr) ? list1 : list2;

        // The real head of the merged sorted list begins directly after our dummy placeholder
        return dummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N and M represent the total node counts inside list1 and list2.
 *                  The splice engine steps through each node index at most once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The realignment splices existing nodes in-place using only 
 *                   localized register references, allocating zero additional heap nodes.
 */
int main() {
    int len1, len2;
    cout << "=== Iterative Dummy Head List Merger Console ===\n";
    
    cout << "Enter element count for the first sorted list: ";
    if (!(cin >> len1) || len1 < 0) return 1;
    
    ListNode* head1 = nullptr;
    ListNode* tail1 = nullptr;
    if (len1 > 0) {
        cout << "Enter " << len1 << " sorted integer values: ";
        for (int i = 0; i < len1; ++i) {
            int val; cin >> val;
            ListNode* n = new ListNode(val);
            if (!head1) { head1 = n; tail1 = n; }
            else { tail1->next = n; tail1 = n; }
        }
    }

    cout << "Enter element count for the second sorted list: ";
    if (!(cin >> len2) || len2 < 0) return 1;

    ListNode* head2 = nullptr;
    ListNode* tail2 = nullptr;
    if (len2 > 0) {
        cout << "Enter " << len2 << " sorted integer values: ";
        for (int i = 0; i < len2; ++i) {
            int val; cin >> val;
            ListNode* n = new ListNode(val);
            if (!head2) { head2 = n; tail2 = n; }
            else { tail2->next = n; tail2 = n; }
        }
    }

    Solution solver;
    cout << "\nExecuting node pointer structural realignments..." << endl;
    ListNode* mergedHead = solver.mergeTwoLists(head1, head2);

    // Print resulting list structure
    cout << "\nMerged Sorted Link Structure:\nHead -> ";
    ListNode* cursor = mergedHead;
    while (cursor) {
        cout << "[" << cursor->val << "] -> ";
        cursor = cursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (mergedHead) {
        ListNode* temp = mergedHead;
        mergedHead = mergedHead->next;
        delete temp;
    }

    return 0;
}