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
     * CORE LOGIC: DUAL POINTER TRAVEL-LENGTH OFFSET ALIGNMENT
     * -------------------------------------------------------------------------------------
     * The objective is to find the exact node where two singly-linked lists intersect. 
     * If there is no intersection, we return nullptr.
     * 
     * 1. THE DIFFERENCE IN LENGTH CONUNDRUM:
     *    If the two lists have different lengths, we cannot simply step through them at the 
     *    same time to find the intersection node, because the pointers will reach the intersection 
     *    at completely different times. A typical solution requires a first pass to calculate 
     *    both lengths, and then shifting the longer list's pointer forward by the difference. 
     * 
     * 2. THE TRAVEL-LENGTH OFFSET ALIGNMENT ELEGANCE:
     *    Instead of tracking lengths explicitly, we use a beautifully compact dual-pointer strategy. 
     *    We launch pointer `pA` at `headA` and pointer `pB` at `headB`. They advance forward 
     *    exactly 1 node per step.
     *      - When pointer `pA` reaches the end of list A (`nullptr`), we redirect it to start over 
     *        at the head of list B (`headB`).
     *      - Similarly, when pointer `pB` reaches the end of list B (`nullptr`), we redirect it to 
     *        start over at the head of list A (`headA`).
     * 
     * 3. THE MATHEMATICAL CONVERGENCE:
     *    By switching paths, each pointer travels a total distance equal to the length of list A 
     *    plus the length of list B ($L_A + L_B$). This naturally aligns their paths, eliminating the 
     *    length difference offset. During the second pass, both pointers are guaranteed to meet 
     *    exactly at the intersection node. If the lists do not intersect, both pointers will 
     *    hit `nullptr` at the same time, ending the loop cleanly.
     */
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // Base Boundary Condition: If either list is completely empty, no intersection can exist
        if (headA == nullptr || headB == nullptr) {
            return nullptr;
        }

        ListNode* pA = headA;
        ListNode* pB = headB;

        // Loop runs until pA and pB converge on the exact same node reference address
        while (pA != pB) {
            // Redirect pA to headB upon hitting terminal boundary, otherwise step forward
            pA = (pA == nullptr) ? headB : pA->next;
            
            // Redirect pB to headA upon hitting terminal boundary, otherwise step forward
            pB = (pB == nullptr) ? headA : pB->next;
        }

        // Returns the convergence node address if intersection exists, or nullptr if they hit the end together
        return pA;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M) - Where N represents the total node count inside list A, and M represents 
 *                  the total node count inside list B. In the absolute worst-case scenario, both pointers 
 *                  traverse exactly L_A + L_B steps before converging.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The convergence calculations run entirely on the stack using 
 *                   only two pointer variables, requiring zero extra dynamic heap allocations.
 */
int main() {
    int commonCount, listACount, listBCount;
    cout << "=== Travel-Length Alignment List Intersection Engine ===\n";
    
    cout << "Enter the node count for the shared common intersecting suffix segment (0 if none): ";
    if (!(cin >> commonCount) || commonCount < 0) return 1;

    cout << "Enter the additional unique node count belonging exclusively to List A: ";
    if (!(cin >> listACount) || listACount < 0) return 1;

    cout << "Enter the additional unique node count belonging exclusively to List B: ";
    if (!(cin >> listBCount) || listBCount < 0) return 1;

    // Allocate the shared common intersection list segment first
    ListNode* commonHead = nullptr;
    ListNode* commonTail = nullptr;
    if (commonCount > 0) {
        cout << "Enter " << commonCount << " integer values for the shared common segment: ";
        for (int i = 0; i < commonCount; ++i) {
            int val; cin >> val;
            ListNode* freshNode = new ListNode(val);
            if (!commonHead) { commonHead = freshNode; commonTail = freshNode; }
            else { commonTail->next = freshNode; commonTail = freshNode; }
        }
    }

    // Allocate unique leading elements for List A
    ListNode* headA = nullptr;
    ListNode* tailA = nullptr;
    if (listACount > 0) {
        cout << "Enter " << listACount << " integer values unique to List A: ";
        for (int i = 0; i < listACount; ++i) {
            int val; cin >> val;
            ListNode* freshNode = new ListNode(val);
            if (!headA) { headA = freshNode; tailA = freshNode; }
            else { tailA->next = freshNode; tailA = freshNode; }
        }
        tailA->next = commonHead; // Attach the shared intersecting suffix segment
    } else {
        headA = commonHead;
    }

    // Allocate unique leading elements for List B
    ListNode* headB = nullptr;
    ListNode* tailB = nullptr;
    if (listBCount > 0) {
        cout << "Enter " << listBCount << " integer values unique to List B: ";
        for (int i = 0; i < listBCount; ++i) {
            int val; cin >> val;
            ListNode* freshNode = new ListNode(val);
            if (!headB) { headB = freshNode; tailB = freshNode; }
            else { tailB->next = freshNode; tailB = freshNode; }
        }
        tailB->next = commonHead; // Attach the shared intersecting suffix segment
    } else {
        headB = commonHead;
    }

    Solution solver;
    cout << "\nLaunching dual alignment pointers and calculating convergence tracks..." << endl;
    ListNode* intersectionResult = solver.getIntersectionNode(headA, headB);

    if (intersectionResult != nullptr) {
        cout << "\nIntersection Node Successfully Located!\n";
        cout << "Value at target intersection node: [" << intersectionResult->val << "]\n";
    } else {
        cout << "\nValidation Result: The structural lists are completely disjoint (No intersection found).\n";
    }

    // Clean up memory: Sever unique paths before cleanup to prevent double-deletion of the common segment
    if (tailA) tailA->next = nullptr;
    if (tailB) tailB->next = nullptr;

    while (headA) { ListNode* t = headA; headA = headA->next; delete t; }
    while (headB) { ListNode* t = headB; headB = headB->next; delete t; }
    while (commonHead) { ListNode* t = commonHead; commonHead = commonHead->next; delete t; }

    return 0;
}