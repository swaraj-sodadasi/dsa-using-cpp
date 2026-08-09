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
     * CORE LOGIC: MIDPOINT BREAK, SUFFIX REVERSAL, AND SEQUENTIAL TRAVERSAL
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a singly-linked list forms a valid palindrome sequence.
     * 
     * 1. THE IN-PLACE ALGORITHMIC CONTRACT:
     *    A naive validation clones the list nodes or buffers their values inside an auxiliary array 
     *    or stack frame. This requires O(N) memory, violating strict constant space design constraints.
     *    Instead, we combine three fundamental list manipulation patterns:
     *      - Step A: Find the list midpoint using a fast/slow twin-pointer scan.
     *      - Step B: In-place reverse the entire second half (suffix string) of the list.
     *      - Step C: Concurrently walk both halves using two standard independent pointers.
     * 
     * 2. MIDPOINT DECOUPLING:
     *    We launch a `slow` (tortoise) and `fast` (hare) pointer from `head`. When `fast` reaches 
     *     the terminal boundary, `slow` sits exactly at the midpoint configuration. The suffix 
     *    segment starting right after `slow` is passed into our inline reversal function.
     * 
     * 3. DUAL-POINTER ALIGNMENT MATRIX VALDIATION:
     *    We align `firstHalfCursor` at the start of the list and `secondHalfCursor` at the head of 
     *    the newly inverted suffix segment. We advance both forward, comparing values element by 
     *    element. If all values match perfectly before `secondHalfCursor` hits nullptr, the list 
     *    is verified as a structural palindrome.
     * 
     *    *Note: To leave the source data in its original configuration, we re-reverse the second 
     *    half back to its initial state before returning the validation result.*
     */
    bool isPalindrome(ListNode* head) {
        // Base Boundary Condition: Empty lists or single elements are inherently palindromic
        if (!head || !head->next) {
            return true;
        }

        // Step 1: Find the list midpoint via fast and slow twin pointers
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // Step 2: Invert the right-hand suffix subsegment in place
        ListNode* secondHalfHead = reverseList(slow->next);

        // Step 3: Run synchronized value matching checks across both half vectors
        ListNode* firstHalfCursor = head;
        ListNode* secondHalfCursor = secondHalfHead;
        bool palindromeConfirmed = true;

        while (palindromeConfirmed && secondHalfCursor != nullptr) {
            if (firstHalfCursor->val != secondHalfCursor->val) {
                palindromeConfirmed = false;
            }
            firstHalfCursor = firstHalfCursor->next;
            secondHalfCursor = secondHalfCursor->next;
        }

        // Restoration Step: Re-reverse the second half to repair the list framework
        slow->next = reverseList(secondHalfHead);

        return palindromeConfirmed;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list. 
 *                  Finding the middle takes N/2 steps, reversing the suffix takes N/2 steps, and the 
 *                  value matching checks take another N/2 steps. The system runs in linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The mutation operates entirely in place by shifting pointer links 
 *                   directly, requiring zero additional heap allocations.
 */
int main() {
    int nodeElementCount;
    cout << "=== Palindrome List Suffix Inversion Engine ===\n";
    cout << "Enter the total number of node elements to assign: ";
    if (!(cin >> nodeElementCount) || nodeElementCount < 0) return 1;

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (nodeElementCount > 0) {
        cout << "Enter the integer node values sequentially:\n";
        for (int i = 0; i < nodeElementCount; ++i) {
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

    // Print source structure configuration
    cout << "\nOriginal Configured Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nResolving midpoint breaks, executing suffix pointer inversions, and evaluating symmetric data matrices..." << endl;
    bool processingOutcome = solver.isPalindrome(listHead);

    if (processingOutcome) {
        cout << "\nValidation Result: VALID LINKED LIST PALINDROME IDENTIFIED! (true)\n";
    } else {
        cout << "\nValidation Result: Structural symmetry rejected. Sequence is asymmetric. (false)\n";
    }

    // Dynamic memory cleanup loops
    while (listHead) {
        ListNode* tempNode = listHead;
        listHead = listHead->next;
        delete tempNode;
    }

    return 0;
}