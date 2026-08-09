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
     * CORE LOGIC: SINGLE-PASS VALUE CONTINUITY CHECK
     * -------------------------------------------------------------------------------------
     * The objective is to remove all duplicate elements from a sorted singly-linked list
     * such that each element appears exactly once.
     * 
     * 1. THE SORTED ADVANTAGE PARADIGM:
     *    Because the input linked list is pre-sorted, all duplicate values are guaranteed
     *    to be adjacent to one another. This structural continuity allows us to completely
     *    bypass complex hash tracking sets or nested lookups, enabling a clean single-pass
     *    deduplication process.
     * 
     * 2. LOOK-AHEAD BYPASS MECHANICS:
     *    We maintain a single tracking pointer `curr` initialized at `head`. At each loop step,
     *    we look ahead at `curr->next`:
     *      - Duplicate Encountered: If `curr->val == curr->next->val`, we have found a value 
     *        continuity match. We capture a reference to the duplicate node (`temp = curr->next`), 
     *        update `curr->next = curr->next->next` to bypass it, and immediately deallocate 
     *        `temp` to protect against leaks. We do NOT advance `curr` yet, because the next 
     *        downstream element might also be a duplicate.
     *      - Unique Value: If `curr->val != curr->next->val`, the next node is unique. We safely 
     *        advance `curr = curr->next`.
     */
    ListNode* deleteDuplicates(ListNode* head) {
        // Base Boundary Condition: Empty structures or single-element tracks have zero duplicates
        if (!head || !head->next) {
            return head;
        }

        ListNode* curr = head;

        // Step 2: Single-pass scanning continuity loop
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->val == curr->next->val) {
                // Duplicate detected: capture link, bypass, and clean heap allocation
                ListNode* duplicateNode = curr->next;
                curr->next = curr->next->next;
                delete duplicateNode;
            } else {
                // Unique value element sequence: slide pointer cursor forward
                curr = curr->next;
            }
        }

        return head;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The continuity check evaluates every list edge exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Deduplication executes strictly in place using a single 
 *                   register pointer on the stack, consuming zero additional heap allocations.
 */
int main() {
    int elementsCount;
    cout << "=== Single-Pass Value Continuity Deduplication Console ===\n";
    cout << "Enter the total number of elements to populate the sorted list: ";
    if (!(cin >> elementsCount) || elementsCount < 0) return 1;

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (elementsCount > 0) {
        cout << "Enter " << elementsCount << " sorted integer node values (e.g., 1 1 2 3 3):\n";
        for (int i = 0; i < elementsCount; ++i) {
            int val;
            cin >> val;
            ListNode* freshNode = new ListNode(val);
            if (!listHead) {
                listHead = freshNode;
                trackingTail = freshNode;
            } else {
                trackingTail->next = freshNode;
                trackingTail = freshNode;
            }
        }
    }

    // Print initial list sequence
    cout << "\nOriginal Sorted Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nScanning value blocks and executing inline node bypass mutations..." << endl;
    ListNode* processedHead = solver.deleteDuplicates(listHead);

    // Print filtered list sequence
    cout << "\nDeduplicated Outcomes Pattern Matrix:\nHead -> ";
    displayCursor = processedHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up remaining allocated heap memory nodes
    while (processedHead) {
        ListNode* tempNode = processedHead;
        processedHead = processedHead->next;
        delete tempNode;
    }

    return 0;
}