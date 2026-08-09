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
     * CORE LOGIC: FLOYD'S TORTOISE AND HARE POINTER DISCREPANCY TRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a singly-linked list contains a structural cycle.
     * 
     * 1. THE ARCHITECTURAL MEMORY TRADEOFF:
     *    A simple verification algorithm tracks visited nodes using a hash table or set. 
     *    However, this drains O(N) extra memory, which can cause allocation overhead for long lists.
     *    Instead, Floyd's Cycle-Finding algorithm uses two pointers that traverse the list at 
     *    different speeds, running in perfect O(1) auxiliary space.
     * 
     * 2. THE CHASER VELOCITY MECHANICS:
     *    - `slow` (Tortoise): Advances forward exactly 1 node per step.
     *    - `fast` (Hare): Advances forward exactly 2 nodes per step.
     * 
     *    If the list contains no loop, the `fast` pointer will eventually hit the terminal 
     *    nullptr boundary, letting us exit early. 
     *    If a cycle exists, both pointers will get trapped inside the loop forever. Because the 
     *    relative speed difference reduces the distance between them by exactly 1 node per iteration, 
     *    the `fast` pointer is guaranteed to overtake and meet the `slow` pointer from behind.
     */
    bool hasCycle(ListNode *head) {
        // Base Boundary Condition: Empty lists or single elements cannot contain cycles
        if (!head || !head->next) {
            return false;
        }

        ListNode* slow = head;
        ListNode* fast = head;

        // Step 2: Loop until the fast pointer reaches the end of the list
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;       // Steps forward 1 node
            fast = fast->next->next; // Steps forward 2 nodes

            // Discrepancy Match: If they point to the exact same node address, a cycle exists
            if (slow == fast) {
                return true;
            }
        }

        // Fast pointer reached the end of the list safely, meaning no cycle exists
        return false;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the list. 
 *                  If no loop exists, the fast pointer reaches the end in N/2 steps. 
 *                  If a loop exists, the pointers will meet within a total traversal distance 
 *                  proportional to the list length.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The detection logic uses only two pointer variables 
 *                   on the stack, avoiding additional heap allocations.
 */
int main() {
    int nodeCount;
    int cyclePositionIndex;

    cout << "=== Floyd's Tortoise and Hare Cycle Detection Console ===\n";
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
    cout << "Running twin-pointer relative velocity checks..." << endl;
    bool cycleDetectedResult = solver.hasCycle(listHead);

    if (cycleDetectedResult) {
        cout << "\nDetection Result: STRUCTURAL LOOP VERIFIED! (true)\n";
    } else {
        cout << "\nDetection Result: Clean linear list, zero loops encountered. (false)\n";
    }

    // Safely break the loop before cleaning up heap memory to prevent infinite loops
    if (trackingTail) {
        trackingTail->next = nullptr;
    }
    for (ListNode* node : allocatedNodes) {
        delete node;
    }

    return 0;
}