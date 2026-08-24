#include <iostream>
#include <vector>

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
     * CORE LOGIC: FLOYD'S CYCLE LOCATION DISCOVERY ANCHOR FORMULA
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a linked list contains a cycle, and if so, return 
     * the exact node where the cycle begins. If no cycle exists, return nullptr.
     * 
     * 1. PHASE 1: INTERSECTION DETECTON
     *    We deploy a standard Floyd's Tortoise and Hare setup. `slow` steps forward by 1, 
     *    while `fast` steps forward by 2. If `fast` or `fast->next` hits nullptr, the list 
     *    is linear (no cycle). If they meet at an intersection node, a loop is confirmed.
     * 
     * 2. MATHEMATICAL ANCHOR FORMULA DERIVATION:
     *    Let:
     *      - $L_1$: The distance from the head node to the cycle entry start node.
     *      - $L_2$: The distance from the cycle entry start node to the meeting point.
     *      - $C$: The total perimeter length of the cycle loop.
     * 
     *    When the two pointers meet, the distance traveled by `slow` is $L_1 + L_2$.
     *    The distance traveled by `fast` is $L_1 + L_2 + k \times C$ (where $k$ is an integer representing 
     *    the number of full cycle loops completed by the fast pointer).
     * 
     *    Since `fast` travels at exactly twice the speed of `slow`:
     *    $$2 \times (L_1 + L_2) = L_1 + L_2 + k \times C$$
     *    $$L_1 + L_2 = k \times C$$
     *    $$L_1 = k \times C - L_2$$
     *    $$L_1 = (k - 1) \times C + (C - L_2)$$
     * 
     * 3. PHASE 2: ENTRY LOCATION MATCHING
     *    The formula $L_1 = (k - 1) \times C + (C - L_2)$ shows that the distance from the head to 
     *    the cycle start ($L_1$) is exactly equal to the distance from the meeting point to the cycle 
     *    start (plus some number of full laps around the cycle loop).
     * 
     *    Therefore, if we leave `slow` at the meeting point, reset `fast` back to `head`, and advance 
     *    both forward at the *same* speed (1 step each), they are mathematically guaranteed to meet 
     *    exactly at the entry node of the cycle.
     */
    ListNode *detectCycle(ListNode *head) {
        // Base Boundary Condition: Empty structures or single-element tracks cannot contain cycles
        if (!head || !head->next) {
            return nullptr;
        }

        ListNode* slow = head;
        ListNode* fast = head;
        bool hasCycle = false;

        // Phase 1: Establish relative velocity tracking loop to detect an intersection point
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) {
                hasCycle = true;
                break;
            }
        }

        // If no intersection was registered, terminate early
        if (!hasCycle) {
            return nullptr;
        }

        // Phase 2: Reset one pointer to head and step both at equal speed to locate the entry node
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }

        // Convergence node is mathematically proven to be the entry anchor of the cycle loop
        return slow;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  Phase 1 discovers the loop in linear time, and Phase 2 travels at most N nodes
 *                  to locate the entry node.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The detection and isolation operations run purely in place
 *                   using local pointer registers, utilizing zero additional heap allocations.
 */
int main() {
    int nodeCount;
    int cyclePositionIndex;

    cout << "=== Floyd's Cycle entry Location Discovery Engine ===\n";
    cout << "Enter the total number of list nodes to allocate: ";
    if (!(cin >> nodeCount) || nodeCount < 0) return 1;

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;
    vector<ListNode*> allocatedNodes;
    allocatedNodes.reserve(static_cast<size_t>(nodeCount));

    if (nodeCount > 0) {
        cout << "Enter " << nodeCount << " integer node values sequentially:\n";
        for (int i = 0; i < nodeCount; ++i) {
            int uniqueValue;
            cin >> uniqueValue;
            ListNode* freshNode = new ListNode(uniqueValue);
            allocatedNodes.push_back(freshNode);

            if (!listHead) {
                listHead = freshNode;
                trackingTail = freshNode;
            } else {
                trackingTail->next = freshNode;
                trackingTail = freshNode;
            }
        }
    }

    cout << "Enter target node position index to link tail back onto (0-indexed, or -1 for no loop): ";
    if (!(cin >> cyclePositionIndex)) return 1;

    // Inject loop connection mapping if configured
    if (cyclePositionIndex >= 0 && cyclePositionIndex < nodeCount && trackingTail) {
        trackingTail->next = allocatedNodes[static_cast<size_t>(cyclePositionIndex)];
        cout << "\nLoop successfully wired from Tail back to Node index [" << cyclePositionIndex << "].\n";
    } else {
        cout << "\nList constructed linearly with an open terminal NULL tail.\n";
    }

    Solution solver;
    cout << "Running mathematical convergence check..." << endl;
    ListNode* cycleEntryNode = solver.detectCycle(listHead);

    if (cycleEntryNode != nullptr) {
        cout << "\nLoop Verified! Entry node discovered at memory index location val: [" << cycleEntryNode->val << "]\n";
    } else {
        cout << "\nValidation Result: Clean linear list, zero loops encountered. (NULL)\n";
    }

    // Safely break the loop before cleaning up heap memory to prevent infinite loops during deletion
    if (trackingTail) {
        trackingTail->next = nullptr;
    }
    for (ListNode* node : allocatedNodes) {
        delete node;
    }

    return 0;
}