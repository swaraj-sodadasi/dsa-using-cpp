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
     * CORE LOGIC: NODE-BY-NODE SUMMATION WITH CARRY PROPAGATION TRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to add two numbers represented by two non-empty linked lists, where 
     * the digits are stored in reverse order (least significant digit first). We return 
     * the sum as a new linked list.
     * 
     * 1. THE DUMMY HEAD PATTERN:
     *    To handle list initialization cleanly without special-casing the creation of the 
     *    very first node, we allocate a stack-bound `dummy` node. Our active tracking 
     *    pointer `tail` appends new digits directly onto `tail->next`, ensuring unified 
     *    logic across every position alignment tier.
     * 
     * 2. NODE-BY-NODE LOOP MECHANICS:
     *    We maintain a running integer `carry` initialized to 0. We iterate through both lists 
     *    (`l1` and `l2`) until both pointer tracks reach `nullptr` AND `carry` drops to 0.
     *      - Extract digits: If a list pointer is valid, we take its value and advance the pointer. 
     *        If a list finishes early, its contributed value defaults to 0.
     *      - Summation matrix: `sum = val1 + val2 + carry`
     *      - Carry configuration: `carry = sum / 10`
     *      - Node construction: A fresh node holding `sum % 10` is attached to `tail->next`.
     * 
     * 3. RESIDUAL OVERFLOW BOUNDARY:
     *    By including `carry` directly in the loop condition (`while (l1 || l2 || carry)`), 
     *    any leftover overflow digit at the most significant position automatically triggers 
     *    the creation of a final terminal node (e.g., 99 + 1 = 100).
     */
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        int carry = 0;

        // Step 2 & 3: Run synchronized traversal loop across digits and overflow carries
        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            int val1 = 0;
            if (l1 != nullptr) {
                val1 = l1->val;
                l1 = l1->next;
            }

            int val2 = 0;
            if (l2 != nullptr) {
                val2 = l2->val;
                l2 = l2->next;
            }

            // Calculate aggregated column sum metrics
            int columnSum = val1 + val2 + carry;
            carry = columnSum / 10;
            
            // Allocate new digit frame onto the target list track
            tail->next = new ListNode(columnSum % 10);
            tail = tail->next;
        }

        return dummy.next;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(max(N, M)) - Where N and M represent the node counts inside l1 and l2. 
 *                  The system evaluates each digit column exactly once.
 *
 * SPACE COMPLEXITY: O(max(N, M)) Auxiliary - Memory allocation scales directly with the length 
 *                   of the newly generated return sum list (at most max(N, M) + 1 nodes).
 */
int main() {
    int len1, len2;
    cout << "=== Node-by-Node Summation Carry Tracking Engine ===\n";
    
    cout << "Enter digit count for the first reversed list (l1): ";
    if (!(cin >> len1) || len1 <= 0) return 1;
    
    ListNode* l1 = nullptr;
    ListNode* tail1 = nullptr;
    cout << "Enter " << len1 << " digits sequentially from LSB to MSB (e.g., 2 4 3 for 342): ";
    for (int i = 0; i < len1; ++i) {
        int val; cin >> val;
        ListNode* n = new ListNode(val);
        if (!l1) { l1 = n; tail1 = n; }
        else { tail1->next = n; tail1 = n; }
    }

    cout << "Enter digit count for the second reversed list (l2): ";
    if (!(cin >> len2) || len2 <= 0) return 1;

    ListNode* l2 = nullptr;
    ListNode* tail2 = nullptr;
    cout << "Enter " << len2 << " digits sequentially from LSB to MSB (e.g., 5 6 4 for 465): ";
    for (int i = 0; i < len2; ++i) {
        int val; cin >> val;
        ListNode* n = new ListNode(val);
        if (!l2) { l2 = n; tail2 = n; }
        else { tail2->next = n; tail2 = n; }
    }

    Solution solver;
    cout << "\nExecuting digit column addition and mapping carry propagation flags..." << endl;
    ListNode* sumHead = solver.addTwoNumbers(l1, l2);

    // Print resulting list structure
    cout << "\nCalculated Sum Link Structure:\nHead -> ";
    ListNode* cursor = sumHead;
    while (cursor) {
        cout << "[" << cursor->val << "] -> ";
        cursor = cursor->next;
    }
    cout << "NULL\n";

    // Dynamic memory cleanup loops
    while (l1) { ListNode* t = l1; l1 = l1->next; delete t; }
    while (l2) { ListNode* t = l2; l2 = l2->next; delete t; }
    while (sumHead) { ListNode* t = sumHead; sumHead = sumHead->next; delete t; }

    return 0;
}