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
     * CORE LOGIC: LESS-THAN VS. GREATER-THAN SPLIT BOUNDARY SPLICING
     * -------------------------------------------------------------------------------------
     * The objective is to partition a singly-linked list around a value `x`, such that
     * all nodes less than `x` come before all nodes greater than or equal to `x`. The
     * original relative order of the nodes in each of the two partitions must be preserved.
     * 
     * 1. THE STABILITY CONSTRAINTS CHALLENGE:
     *    Sorting algorithms or quick-sort style dual-swap partitions lose structural element 
     *    stability, scrambling the original sequence order. 
     *    Instead of shifting or swapping values within the original structure, we maintain two
     *    independent, stable sub-list component systems using independent boundary trackers.
     * 
     * 2. TWO DUMMY HEAD TRACKING PIPELINES:
     *    We instantiate two separate dummy nodes on the stack to act as clean anchors:
     *      - `lessDummy`: Anchors the sub-list containing all elements strictly less than `x`.
     *      - `greaterDummy`: Anchors the sub-list containing elements greater than or equal to `x`.
     *    We map separate operational tracking cursors (`lessTail` and `greaterTail`) initialized 
     *    to these anchors.
     * 
     * 3. SINGLE-PASS DECOUPLING AND BOUNDARY STITCHING:
     *    We traverse the list using a `curr` pointer. If `curr->val < x`, we append it to 
     *    `lessTail->next` and advance `lessTail`. Otherwise, it goes to `greaterTail->next`.
     *    Crucially, we clear out the final terminal node of the upper tier (`greaterTail->next = nullptr`) 
     *    to break any residual cyclic pointer trails. Finally, we splice the two halves together 
     *    by pointing `lessTail->next` to `greaterDummy.next`.
     */
    ListNode* partition(ListNode* head, int x) {
        // Base Boundary Condition: Empty lists or single elements require no structural change
        if (!head || !head->next) {
            return head;
        }

        // Step 2: Initialize the dual dummy anchors on the stack frame
        ListNode lessDummy(0);
        ListNode greaterDummy(0);
        
        ListNode* lessTail = &lessDummy;
        ListNode* greaterTail = &greaterDummy;
        ListNode* curr = head;

        // Step 3: Single-pass stable splitting sweep
        while (curr != nullptr) {
            if (curr->val < x) {
                lessTail->next = curr;
                lessTail = lessTail->next;
            } else {
                greaterTail->next = curr;
                greaterTail = greaterTail->next;
            }
            curr = curr->next;
        }

        // Sever the terminal trail to guard against infinite pointer recycling loops
        greaterTail->next = nullptr;

        // Splice the less-than tail pointer onto the head node of the upper partition
        lessTail->next = greaterDummy.next;

        // The real head of the partitioned sequence starts right after the lower dummy placeholder
        return lessDummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The system evaluates each node condition exactly once in a single forward pass.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The reconfiguration executes strictly in-place by weaving
 *                   existing node links, allocating zero additional heap node blocks.
 */
int main() {
    int elementsCount;
    int partitionThresholdValue;

    cout << "=== Stable Split Boundary List Partition Engine ===\n";
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

    cout << "Enter the partition target threshold integer (x): ";
    if (!(cin >> partitionThresholdValue)) {
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
    cout << "\nLaunching dual dummy anchors and splicing stable boundaries..." << endl;
    ListNode* partitionedHeadNode = solver.partition(listHead, partitionThresholdValue);

    // Print resulting list sequence after execution
    cout << "\nPartitioned Outcome Matrix:\nHead -> ";
    displayCursor = partitionedHeadNode;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (partitionedHeadNode) {
        ListNode* tempNode = partitionedHeadNode;
        partitionedHeadNode = partitionedHeadNode->next;
        delete tempNode;
    }

    return 0;
}