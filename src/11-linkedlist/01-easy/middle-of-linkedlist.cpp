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
     * CORE LOGIC: FAST AND SLOW MULTIPLIER POINTER REALIGNMENT
     * -------------------------------------------------------------------------------------
     * The goal is to find the middle node of a singly-linked list. If the list has an even 
     * number of nodes, we return the second middle node.
     * 
     * 1. THE CONVENTIONAL DUAL-PASS OVERHEAD:
     *    A simple solution requires a first pass to count the total number of elements, 
     *    followed by a second pass to step forward exactly `N / 2` times. This forces us 
     *    to touch nodes repeatedly.
     *    Instead, we use Floyd's tortoise-and-hare pointer realignment technique, which 
     *    identifies the middle node in a single clean pass.
     * 
     * 2. THE MULTIPLIER TRAVERSAL MECHANICS:
     *    We launch two pointer references initialized at the list head:
     *      - `slow` (Tortoise): Steps forward exactly 1 node per iteration.
     *      - `fast` (Hare): Steps forward exactly 2 nodes per iteration.
     * 
     *    Because the `fast` pointer moves at exactly twice the speed of the `slow` pointer, 
     *    the ratio between their distances remains constantly proportional. The moment the 
     *    `fast` pointer reaches the terminal end of the list (where either `fast` or 
     *    `fast->next` hits `nullptr`), the `slow` pointer will have traveled exactly half 
     *    that distance, placing it perfectly at the midpoint node.
     */
    ListNode* middleNode(ListNode* head) {
        // Base Boundary Condition: Empty lists or single elements are their own middle structures
        if (!head || !head->next) {
            return head;
        }

        ListNode* slow = head;
        ListNode* fast = head;

        // Step 2: Loop until the fast pointer or its immediate successor hits null space
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;       // Advance 1 unit
            fast = fast->next->next; // Advance 2 units (multiplier speed)
        }

        // Slow pointer now references the exact midpoint node configuration frame
        return slow;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list. 
 *                  The multiplier search engine scans the list topology in a single linear pass.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The midpoint calculation runs entirely on the stack using 
 *                   only two pointer registers, requiring zero extra dynamic memory allocations.
 */
int main() {
    int elementsCount;
    cout << "=== Midpoint Multiplier List Realignment Engine ===\n";
    cout << "Enter the total number of elements to construct the list: ";
    if (!(cin >> elementsCount) || elementsCount < 0) {
        cout << "Invalid dynamic allocation sizing input." << endl;
        return 1;
    }

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (elementsCount > 0) {
        cout << "Enter the integer node values sequentially:\n";
        for (int i = 0; i < elementsCount; ++i) {
            int uniqueVal;
            cin >> uniqueVal;
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

    Solution solver;
    cout << "\nLaunching parallel slow-fast scans and resolving midpoint anchors..." << endl;
    ListNode* calculatedMiddle = solver.middleNode(listHead);

    if (calculatedMiddle) {
        cout << "\nMidpoint Segment Identification Achieved!\n";
        cout << "Value at target middle node: [" << calculatedMiddle->val << "]\n";
        
        // Print remaining trailing subsegment for confirmation
        cout << "Trailing Subsequence Path: Head -> ";
        ListNode* displayCursor = calculatedMiddle;
        while (displayCursor) {
            cout << "[" << displayCursor->val << "] -> ";
            displayCursor = displayCursor->next;
        }
        cout << "NULL\n";
    } else {
        cout << "\nOutcome: The provided linked list structure is completely empty.\n";
    }

    // Clean up allocated heap memory nodes
    while (listHead) {
        ListNode* tempNode = listHead;
        listHead = listHead->next;
        delete tempNode;
    }

    return 0;
}