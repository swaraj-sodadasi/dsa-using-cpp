#include <iostream>
#include <vector>

using namespace std;

// Definition for singly-linked list node.
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
private:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr != nullptr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: PALINDROME LINKED LIST (HEAD/TAIL DOUBLE-ENDED COMPARISON SIMULATION)
     * -------------------------------------------------------------------------------------
     * The objective is to check whether a singly-linked list is a palindrome in O(N) time 
     * and O(1) auxiliary space.
     * 
     * 1. FAST & SLOW POINTER MIDPOINT FINDER:
     *    We locate the midpoint of the linked list using two pointers (`slow` and `fast`).
     *    `slow` moves 1 step while `fast` moves 2 steps. When `fast` reaches the end, `slow` 
     *    will sit at the middle node.
     * 
     * 2. REVERSING THE SECOND HALF:
     *    We reverse the second half of the linked list starting from `slow` (or `slow->next`).
     * 
     * 3. TWO-WAY COMPARISON SIMULATION:
     *    We iterate simultaneously with two pointers—one starting at `head` and the other 
     *    at the reversed second half's head (`tail`). If all corresponding values match, 
     *    the list is a palindrome.
     * 
     * 4. RESTORING LIST INTEGRITY:
     *    Before returning, we reverse the second half back to restore the original 
     *    linked list layout.
     */
    bool isPalindrome(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return true;

        // Step 1: Find midpoint using fast and slow pointers
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // Step 2: Reverse the second half of the linked list
        ListNode* secondHalfHead = reverseList(slow->next);

        // Step 3: Compare first and second half values
        ListNode* firstHalfIter = head;
        ListNode* secondHalfIter = secondHalfHead;
        bool isPalin = true;

        while (isPalin && secondHalfIter != nullptr) {
            if (firstHalfIter->val != secondHalfIter->val) {
                isPalin = false;
            }
            firstHalfIter = firstHalfIter->next;
            secondHalfIter = secondHalfIter->next;
        }

        // Step 4: Restore original list structure
        slow->next = reverseList(secondHalfHead);

        return isPalin;
    }
};

// Helper function to build a linked list from dynamic user vector input
ListNode* buildList(const vector<int>& nums) {
    if (nums.empty()) return nullptr;
    ListNode* head = new ListNode(nums[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < nums.size(); ++i) {
        curr->next = new ListNode(nums[i]);
        curr = curr->next;
    }
    return head;
}

// Helper function to clean up allocated linked list nodes
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Single pass to find midpoint, reverse, compare, and restore.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - In-place pointer modifications.
 */
int main() {
    cout << "=== Palindrome Linked List Head/Tail Simulator ===\n";
    cout << "Enter the number of nodes in the linked list: ";
    int nodeCount;
    if (!(cin >> nodeCount) || nodeCount <= 0) return 1;

    vector<int> nums(static_cast<size_t>(nodeCount));
    cout << "Enter node values sequentially:\n";
    for (int i = 0; i < nodeCount; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    ListNode* head = buildList(nums);

    Solution solver;
    bool result = solver.isPalindrome(head);

    if (result) {
        cout << "Linked list is a valid palindrome! Output: [true]\n";
    } else {
        cout << "Linked list is NOT a valid palindrome. Output: [false]\n";
    }

    freeList(head);
    return 0;
}