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
     * CORE LOGIC: DUAL SEGMENT COMPONENT SPLICING VIA STRUCTURAL INDEXES
     * -------------------------------------------------------------------------------------
     * The objective is to reorder a singly-linked list such that all nodes at odd structural
     * indices come first, followed by all nodes at even structural indices (1-indexed).
     * 
     * 1. THE IN-PLACE ARCHITECTURAL CONTRACT:
     *    A simple approach extracts odd and even values into auxiliary vectors or separate 
     *    node configurations, which consumes O(N) memory.
     *    To achieve strict O(1) auxiliary space, we manipulate the existing node links 
     *    in-place, weaving two separate, concurrent sub-lists from the original sequence.
     * 
     * 2. DUAL RUNNING CURSORS AND LOOK-AHEAD STEPPING:
     *    We maintain two independent track pointers along with an anchor reference:
     *      - `odd`: Processes nodes at odd index tiers (starts at `head`).
     *      - `even`: Processes nodes at even index tiers (starts at `head->next`).
     *      - `evenHead`: An immutable anchor holding the start of the even sub-list.
     * 
     *    As we loop, we bypass alternate elements by remapping the pointers:
     *      - `odd->next = even->next` extracts the next odd element.
     *      - We then advance `odd = odd->next`.
     *      - `even->next = odd->next` extracts the next even element.
     *      - We then advance `even = even->next`.
     * 
     * 3. SUB-LIST COMPONENT SPLICING:
     *    Once the loop finishes separating the two segments, the `odd` cursor sits at the 
     *    very end of the odd sub-list. We splice the two sub-lists together by setting 
     *    `odd->next = evenHead`, seamlessly attaching the even sub-list onto the tail of the 
     *    odd sub-list.
     */
    ListNode* oddEvenList(ListNode* head) {
        // Base Boundary Condition: Lists with less than 3 elements require zero structural mutation
        if (!head || !head->next || !head->next->next) {
            return head;
        }

        ListNode* odd = head;
        ListNode* even = head->next;
        ListNode* evenHead = even; // Anchor reference to stitch components together later

        // Step 2: Separate nodes by tracking alternate index bounds look-aheads
        while (even != nullptr && even->next != nullptr) {
            odd->next = even->next;    // Stitch the next odd node onto the odd sub-list
            odd = odd->next;           // Slide the odd pointer cursor forward
            
            even->next = odd->next;    // Stitch the next even node onto the even sub-list
            even = even->next;         // Slide the even pointer cursor forward
        }

        // Step 3: Component splicing — hook the even sub-list onto the tail of the odd sub-list
        odd->next = evenHead;

        return head;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The system processes each list pointer linkage exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The reconfiguration executes purely in-place by altering
 *                   node bonds on the stack, consuming zero additional heap allocations.
 */
int main() {
    int elementsCount;
    cout << "=== Dual Segment Structural Index Splicing Engine ===\n";
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
    cout << "\nLaunching dual pointer sweeps and executing component structural splicing..." << endl;
    ListNode* rearrangedHeadNode = solver.oddEvenList(listHead);

    // Print resulting list sequence after execution
    cout << "\nRearranged Outcome Matrix:\nHead -> ";
    displayCursor = rearrangedHeadNode;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    // Clean up allocated heap memory nodes
    while (rearrangedHeadNode) {
        ListNode* tempNode = rearrangedHeadNode;
        rearrangedHeadNode = rearrangedHeadNode->next;
        delete tempNode;
    }

    return 0;
}