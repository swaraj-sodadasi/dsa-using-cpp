#include <iostream>

using namespace std;

// Definition for a Node in a cyclic sorted list structure.
class Node {
public:
    int val;
    Node* next;

    Node() {
        val = 0;
        next = nullptr;
    }

    Node(int _val) {
        val = _val;
        next = nullptr;
    }

    Node(int _val, Node* _next) {
        val = _val;
        next = _next;
    }
};

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: THREE-CASE POINTER INVERSION INSERTION GEOMETRY
     * -------------------------------------------------------------------------------------
     * The objective is to insert a fresh value into a continuously expanding cyclic sorted 
     * list such that the structural integrity of the sorted circular ring remains perfect.
     * 
     * 1. THE BOUNDARY EMPTY CASE:
     *    If the input anchor pointer is `nullptr`, we allocate a new standalone node object. 
     *    To satisfy the circular contract immediately, we configure its `next` pointer field 
     *    to loop back to itself (`node->next = node`) and return it.
     * 
     * 2. GEOMETRICAL ITERATION LOOP RULES:
     *    We maintain a trailing cursor pair, `curr` and `next`, initialized to `head` and 
     *    `head->next`. We traverse the list exactly once. Insertion happens when one of 
     *    three structural cases matches the value constraints:
     * 
     *    CASE A: STANDARD INTERIOR INSERTION ZONE
     *      The insertion value fits perfectly between the current elements:
     *      `curr->val <= insertVal` AND `insertVal <= next->val`
     *      (e.g., Inserting 3 into ... -> 2 -> 4 -> ...)
     * 
     *    CASE B: BOUNDARY OVERFLOW / UNDERFLOW INFLECTION POINT
     *      The cursor crosses the inflection boundary where the sequence drops from max back to min 
     *      (`curr->val > next->val`). The new value is either an absolute maximum or absolute minimum:
     *      `insertVal >= curr->val` (Maximum element overflow) OR `insertVal <= next->val` (Minimum element underflow)
     *      (e.g., Inserting 5 or 0 into ... -> 4 -> 1 -> ...)
     * 
     *    CASE C: FULL DUPLICATE EQUALITY MONOLITH
     *      The loop wraps all the way back to the starting anchor node without hitting Case A or B. 
     *      This means all elements in the cyclic list share the exact same value. The new node 
     *      can safely be spliced anywhere, so we append it right after `curr`.
     */
    Node* insert(Node* head, int insertVal) {
        // Case 1: Handle structural empty edge condition
        if (head == nullptr) {
            Node* newNode = new Node(insertVal);
            newNode->next = newNode;
            return newNode;
        }

        Node* curr = head;
        Node* nextNode = head->next;
        bool insertPositionFound = false;

        while (true) {
            // Case A: Standard interior insertion zone match
            if (curr->val <= insertVal && insertVal <= nextNode->val) {
                insertPositionFound = true;
            }
            // Case B: Boundary inflection point crossing check
            else if (curr->val > nextNode->val) {
                if (insertVal >= curr->val || insertVal <= nextNode->val) {
                    insertPositionFound = true;
                }
            }

            if (insertPositionFound) {
                curr->next = new Node(insertVal, nextNode);
                return head;
            }

            curr = nextNode;
            nextNode = nextNode->next;

            // Case C: Full cyclic traversal wrap loop complete (monolith matching)
            if (curr == head) {
                curr->next = new Node(insertVal, nextNode);
                return head;
            }
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the cyclic structure. 
 *                  The system evaluates geometry bounds in a single circular pass.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The new element is spliced directly in-place using existing 
 *                   pointer allocations, consuming zero extra runtime stack frames.
 */
int main() {
    int elementsCount;
    cout << "=== Cyclic Sorted List Three-Case Insertion Engine ===\n";
    cout << "Enter the total number of initial sorted nodes in the cyclic ring: ";
    if (!(cin >> elementsCount) || elementsCount < 0) return 1;

    Node* headNode = nullptr;
    Node* tailNode = nullptr;

    if (elementsCount > 0) {
        cout << "Enter the sorted integer values sequentially (e.g., 1 3 4):\n";
        for (int i = 0; i < elementsCount; ++i) {
            int uniqueVal; cin >> uniqueVal;
            Node* freshNode = new Node(uniqueVal);
            if (!headNode) {
                headNode = freshNode;
                tailNode = freshNode;
            } else {
                tailNode->next = freshNode;
                tailNode = freshNode;
            }
        }
        // Secure circular structural invariant contract links
        if (tailNode) {
            tailNode->next = headNode;
        }
    }

    int targetInsertValue;
    cout << "Enter the value to inject dynamically into the cyclic ring: ";
    if (!(cin >> targetInsertValue)) return 1;

    // Display state before insertion mutation loop
    if (headNode) {
        cout << "\nOriginal Ring Layout Matrix (Starting from Head Element):\n";
        Node* scan = headNode;
        for (int i = 0; i < elementsCount; ++i) {
            cout << "[" << scan->val << "] -> ";
            scan = scan->next;
        }
        cout << "(Loops back to [" << scan->val << "])\n";
    } else {
        cout << "\nOriginal Ring Layout Matrix: [Empty Elements Structure]\n";
    }

    Solution solver;
    cout << "\nEvaluating geometric boundaries and mapping insertion case matrices..." << endl;
    Node* updatedHeadNode = solver.insert(headNode, targetInsertValue);

    // Display state after processing execution steps
    cout << "\nAltered Ring Outcome Matrix:\n";
    Node* printCursor = updatedHeadNode;
    int resultingNodeCount = elementsCount + 1;
    for (int i = 0; i < resultingNodeCount; ++i) {
        cout << "[" << printCursor->val << "] -> ";
        printCursor = printCursor->next;
    }
    cout << "(Loops back to [" << printCursor->val << "])\n";

    // Clean up heap allocation blocks without entering an infinite circular trap loop
    if (updatedHeadNode) {
        Node* currNode = updatedHeadNode;
        for (int i = 0; i < resultingNodeCount; ++i) {
            Node* nextNodeToDelete = currNode->next;
            delete currNode;
            currNode = nextNodeToDelete;
        }
    }

    return 0;
}