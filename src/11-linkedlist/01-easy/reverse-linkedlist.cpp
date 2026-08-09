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
     * CORE LOGIC: ITERATIVE THREE-POINTER POINTER-INVERSION STRATEGY
     * -------------------------------------------------------------------------------------
     * The objective is to take a singly-linked list and reverse the direction of its links
     * completely in-place, returning the new head pointer.
     * 
     * 1. THE IN-PLACE ARCHITECTURAL EDGE:
     *    A naive list reversal allocates a parallel sequence of fresh nodes or pushes elements 
     *    onto a stack frame. This drains O(N) heap/stack memory.
     *    Instead, the three-pointer inversion technique operates purely by shifting structural links.
     *    It mutates the list nodes in a single forward pass using O(1) auxiliary variables, 
     *    eliminating allocation faults and preserving cache locality.
     * 
     * 2. THE THREE-POINTER CURSOR MECHANICS:
     *    We maintain three sliding pointers concurrently during execution:
     *      - `prevNode`: Tracks the element right behind the current scanning index. Initialized to nullptr 
     *                   since the original head node transitions into the new terminal tail node.
     *      - `currNode`: Points directly to the list node undergoing link modification.
     *      - `nextNode`: Acts as a crucial structural anchor. Before we break the link pointing from 
     *                   `currNode` to its successor, we map `nextNode = currNode->next` to prevent losing 
     *                   access to the remaining tail segment.
     * 
     * 3. LINK RETARGETING AND CROSSING:
     *    For each loop step, we disconnect `currNode->next` and bend it backward to point to `prevNode`. 
     *    Once inverted, we advance the state registers: `prevNode` slides up to `currNode`, and `currNode` 
     *    moves forward to `nextNode`. When `currNode` drops to nullptr, `prevNode` stands at the new head.
     */
    ListNode* reverseList(ListNode* head) {
        // Base Boundary Condition: Empty structures or single element lists require zero transformation
        if (!head || !head->next) {
            return head;
        }

        ListNode* prevNode = nullptr;
        ListNode* currNode = head;
        ListNode* nextNode = nullptr;

        // Step 2 & 3: Run the sliding inversion pass across node tracks
        while (currNode != nullptr) {
            // Anchor the subsequent sequence path safely before mutating bonds
            nextNode = currNode->next;
            
            // Pointer Inversion Step: Turn the active node link backwards
            currNode->next = prevNode;
            
            // Advance the state cursor tracking variables forward one unit
            prevNode = currNode;
            currNode = nextNode;
        }

        // When currNode crosses the edge into null space, prevNode lands at the new head node frame
        return prevNode;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list. 
 *                  The loop processes each node element exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The reversal executes using only three stack-allocated pointer registers, 
 *                   requiring zero extra memory space regardless of list length.
 */
int main() {
    int elementsCount;
    cout << "=== Iterative Three-Pointer List Inversion Console ===\n";
    cout << "Enter the total number of elements to construct the list: ";
    if (!(cin >> elementsCount) || elementsCount < 0) {
        cout << "Invalid layout parameters allocation input." << endl;
        return 1;
    }

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (elementsCount > 0) {
        cout << "Enter the integer node values sequentially:\n";
        for (int i = 0; i < elementsCount; ++i) {
            int dynamicVal;
            cin >> dynamicVal;
            ListNode* freshNode = new ListNode(dynamicVal);
            
            if (!listHead) {
                listHead = freshNode;
                trackingTail = freshNode;
            } else {
                trackingTail->next = freshNode;
                trackingTail = freshNode;
            }
        }
    }

    // Print initial list structure
    cout << "\nOriginal Configured Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nEngaging three-pointer sliding sweeps and processing link inversions..." << endl;
    ListNode* invertedListHead = solver.reverseList(listHead);

    // Print resulting list structure
    cout << "\nInverted Outcomes Pattern Matrix:\nNew Head -> ";
    displayCursor = invertedListHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Dynamic memory cleanup loops
    while (invertedListHead) {
        ListNode* tempNode = invertedListHead;
        invertedListHead = invertedListHead->next;
        delete tempNode;
    }

    return 0;
}