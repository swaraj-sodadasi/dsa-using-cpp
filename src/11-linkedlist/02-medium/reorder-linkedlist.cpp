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
     * HELPER FUNCTION: POINTER-INVERSION LIST REVERSAL
     * Inverts a singly-linked list segment in-place using O(1) auxiliary space.
     */
    ListNode* reverseList(ListNode* head) {
        ListNode* prevNode = nullptr;
        ListNode* currNode = head;
        ListNode* nextNode = nullptr;

        while (currNode != nullptr) {
            nextNode = currNode->next;
            currNode->next = prevNode;
            prevNode = currNode;
            currNode = nextNode;
        }
        return prevNode;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MIDPOINT SPLIT, SUB-LIST INVERSION, AND INTERLEAVED NODE SPLICING
     * -------------------------------------------------------------------------------------
     * The objective is to reorder a singly-linked list in-place such that it alternates 
     * between nodes from the front and nodes from the back: L0 -> Ln -> L1 -> Ln-1 -> L2 -> ...
     * 
     * 1. THE ARCHITECTURAL REFACTORING ENGINE:
     *    A naive solution copies node pointers into a vector to reshuffle links using random 
     *    access indices. This requires $O(N)$ extra memory.
     *    Instead, we combine three structural linear time primitives that require only $O(1)$ 
     *    auxiliary space:
     *      - Step A: Find the midpoint of the list using a fast-and-slow multiplier scan.
     *      - Step B: Sever the list at the midpoint into two distinct half segments, and 
     *                reverse the entire second sub-list in place.
     *      - Step C: Interleave and stitch the nodes from both sub-lists together one by one.
     * 
     * 2. INTERLEAVING MECHANICS:
     *    We align `firstHalf` at the original head, and `secondHalf` at the head of the newly 
     *    inverted sub-list. We zip them together using look-ahead tracking pointers (`next1` and `next2`) 
     *    to avoid breaking the remaining chains while splicing the nodes.
     */
    void reorderList(ListNode* head) {
        // Base Boundary Condition: Lists with less than 3 nodes are already correctly ordered
        if (!head || !head->next || !head->next->next) {
            return;
        }

        // Step A: Find the list midpoint via fast and slow twin pointers
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // Step B: Split the list into two halves and reverse the second sub-list in place
        ListNode* secondHalfHead = reverseList(slow->next);
        slow->next = nullptr; // Sever the connection to isolate the first sub-list tail

        // Step C: Interleave and stitch nodes from both sub-lists together
        ListNode* firstHalfHead = head;
        while (secondHalfHead != nullptr) {
            // Anchor subsequent downstream segment paths safely before mutating links
            ListNode* next1 = firstHalfHead->next;
            ListNode* next2 = secondHalfHead->next;

            // Splice the node from the second sub-list right after the node from the first sub-list
            firstHalfHead->next = secondHalfHead;
            secondHalfHead->next = next1;

            // Advance running pointers forward to their anchored path states
            firstHalfHead = next1;
            secondHalfHead = next2;
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  Finding the middle takes N/2 steps, reversing the second half takes N/2 steps, 
 *                  and interleaving takes another N/2 steps, running in strict linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The reordering mutates existing pointer links completely 
 *                   in-place, consuming zero additional heap allocations.
 */
int main() {
    int elementsCount;
    cout << "=== Midpoint Split Interleaved Splicing Engine ===\n";
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

    // Print initial list structure configuration
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nResolving midpoint breaks, executing sub-list inversions, and zipping node matrices..." << endl;
    solver.reorderList(listHead);

    // Print resulting list sequence after execution
    cout << "\nReordered Outcome Matrix:\nHead -> ";
    displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (listHead) {
        ListNode* tempNode = listHead;
        listHead = listHead->next;
        delete tempNode;
    }

    return 0;
}