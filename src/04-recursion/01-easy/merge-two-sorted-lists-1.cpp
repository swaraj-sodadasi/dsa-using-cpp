#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (RECURSIVE POINTER REALIGNMENT):
- The objective is to merge two sorted singly linked lists into a single, continuous sorted linked list.
- **Recursive Sub-problem Formulation**:
  - The structure of a linked list makes it a natural fit for recursion. At any step, we compare the head nodes of both lists (`list1` and `list2`).
  - **Base Cases**:
    - If `list1` is null, the remaining elements must all come from `list2`, so we return `list2`.
    - If `list2` is null, the remaining elements must all come from `list1`, so we return `list1`.
  - **Recursive Splice Step**:
    - If `list1->val <= list2->val`, then `list1` should be the next node in our merged list. 
      We set `list1->next` to the result of recursively merging the rest of `list1` (`list1->next`) with `list2`. We then return `list1`.
    - If `list2->val < list1->val`, then `list2` should be the next node. 
      We set `list2->next` to the result of recursively merging `list1` with the rest of `list2` (`list2->next`). We then return `list2`.

TIME COMPLEXITY: O(N + M) - Where N and M are the lengths of `list1` and `list2`, respectively. We make exactly one recursive call per node across both lists.
SPACE COMPLEXITY: O(N + M) auxiliary - The call stack grows linearly with the total number of elements, as each splice step adds a frame to the stack.
*/

// Definition for a singly linked list node structure
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    // Base Cases: If either list runs out of elements, return the other list
    if (!list1) return list2;
    if (!list2) return list1;

    // Pick the node with the smaller value and recursively stitch the rest of the list
    if (list1->val <= list2->val) {
        list1->next = mergeTwoLists(list1->next, list2);
        return list1;
    } else {
        list2->next = mergeTwoLists(list1, list2->next);
        return list2;
    }
}

// Helper function to build a linked list from an array of elements
ListNode* buildList(const vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new ListNode(values[i]);
        curr = curr->next;
    }
    return head;
}

// Helper function to safely print and display a linked list structure
void printList(ListNode* head) {
    ListNode* curr = head;
    while (curr) {
        cout << curr->val << " -> ";
        curr = curr->next;
    }
    cout << "NULL" << endl;
}

// Helper function to deallocate memory and prevent memory leaks
void freeList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int n, m;
    
    cout << "Enter the number of elements in the first sorted list: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid list scale allocation parameter." << endl;
        return 1;
    }
    vector<int> values1(static_cast<size_t>(n));
    if (n > 0) {
        cout << "Enter the sorted elements for the first list separated by spaces: ";
        for (int i = 0; i < n; ++i) cin >> values1[static_cast<size_t>(i)];
    }

    cout << "Enter the number of elements in the second sorted list: ";
    if (!(cin >> m) || m < 0) {
        cout << "Invalid list scale allocation parameter." << endl;
        return 1;
    }
    vector<int> values2(static_cast<size_t>(m));
    if (m > 0) {
        cout << "Enter the sorted elements for the second list separated by spaces: ";
        for (int i = 0; i < m; ++i) cin >> values2[static_cast<size_t>(i)];
    }

    ListNode* list1 = buildList(values1);
    ListNode* list2 = buildList(values2);

    cout << "\nOriginal List 1: ";
    printList(list1);
    cout << "Original List 2: ";
    printList(list2);

    ListNode* mergedList = mergeTwoLists(list1, list2);

    cout << "\nFinal Merged Sorted List: ";
    printList(mergedList);

    // Clean up allocated heap memory resources
    freeList(mergedList);

    return 0;
}