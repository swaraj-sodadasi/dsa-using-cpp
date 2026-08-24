#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Definition for singly-linked list node structure.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Custom comparator structure for priority queue element alignment
struct CompareNodes {
    bool operator()(const ListNode* lhs, const ListNode* rhs) const {
        // Higher values have lower priority to form a Min-Heap layout structure
        return lhs->val > rhs->val;
    }
};

class Solution {
private:
    /*
     * APPROACH 1 HELPER: TWO-WAY ITERATIVE NODE MERGER
     * Stitches two pre-sorted lists together in-place using a stack-allocated dummy node.
     */
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (list1 != nullptr && list2 != nullptr) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        tail->next = (list1 != nullptr) ? list1 : list2;
        return dummy.next;
    }

    /*
     * APPROACH 1 HELPER: DIVIDE-AND-CONQUER SEGMENT SPLITTER
     * Recursively breaks lists down to binary segments before stitching.
     */
    ListNode* divideAndConquerMerge(vector<ListNode*>& lists, int start, int end) {
        if (start > end)   return nullptr;
        if (start == end)  return lists[static_cast<size_t>(start)];
        
        int mid = start + (end - start) / 2;
        ListNode* leftSorted = divideAndConquerMerge(lists, start, mid);
        ListNode* rightSorted = divideAndConquerMerge(lists, mid + 1, end);
        
        return mergeTwoLists(leftSorted, rightSorted);
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * METHOD 1: DIVIDE AND CONQUER MERGE SORTING OVER LIST SPACE
     * -------------------------------------------------------------------------------------
     * Pairwise merges lists in a tree-like topology. Reduces execution passes by 
     * pairing $K$ lists down to $K/2$, then $K/4$, until a single merged list remains.
     * 
     * TIME COMPLEXITY: O(N log K) - Where N is the total node count across all lists, 
     *                  and K is the number of linked lists. There are log K merging stages.
     * SPACE COMPLEXITY: O(log K) - Call-stack frame allocation bounds.
     */
    ListNode* mergeKListsDivideAndConquer(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        return divideAndConquerMerge(lists, 0, static_cast<int>(lists.size()) - 1);
    }

    /*
     * -------------------------------------------------------------------------------------
     * METHOD 2: MIN-HEAP MULTI-POINTER ALIGNMENT
     * -------------------------------------------------------------------------------------
     * We populate a Priority Queue (Min-Heap) with the head nodes of all non-empty lists. 
     * The top of the heap always gives the smallest element across all lists. 
     * We detach it, append it to our result list, and if it has a successor node, we push 
     * that successor back into the heap.
     * 
     * TIME COMPLEXITY: O(N log K) - Each extraction and insertion takes log K time.
     * SPACE COMPLEXITY: O(K) Auxiliary - The Min-Heap stores at most K nodes at any time.
     */
    ListNode* mergeKListsMinHeap(vector<ListNode*>& lists) {
        // Instantiate the Min-Heap priority queue layer
        priority_queue<ListNode*, vector<ListNode*>, CompareNodes> minHeap;

        // Push initial valid head nodes from all paths
        for (ListNode* head : lists) {
            if (head != nullptr) {
                minHeap.push(head);
            }
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        // Extract minimums and dynamically replace them with their successors
        while (!minHeap.empty()) {
            ListNode* smallestNode = minHeap.top();
            minHeap.pop();

            tail->next = smallestNode;
            tail = tail->next;

            // If a successor exists along the extracted path, re-inject it into the heap
            if (smallestNode->next != nullptr) {
                minHeap.push(smallestNode->next);
            }
        }

        return dummy.next;
    }
};

/*
 * MAIN TEST BENCH FUNCTIONALITY
 */
int main() {
    int listCount;
    cout << "=== Merge K Sorted Lists Multi-Engine Console ===\n";
    cout << "Enter the total number of sorted lists (K): ";
    if (!(cin >> listCount) || listCount < 0) return 1;

    vector<ListNode*> lists(static_cast<size_t>(listCount), nullptr);
    vector<ListNode*> listsCopy(static_cast<size_t>(listCount), nullptr);
    
    // Dynamic tracking system to clean up memory correctly later
    vector<ListNode*> masterAllocationList;

    for (int i = 0; i < listCount; ++i) {
        int nodeCount;
        cout << "Enter the node count for sorted list [" << i << "]: ";
        cin >> nodeCount;

        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        ListNode* headCopy = nullptr;
        ListNode* tailCopy = nullptr;

        if (nodeCount > 0) {
            cout << "Enter " << nodeCount << " sorted values for list [" << i << "]: ";
            for (int j = 0; j < nodeCount; ++j) {
                int val; cin >> val;
                
                ListNode* n1 = new ListNode(val);
                ListNode* n2 = new ListNode(val); // Secondary copy for separate test pass
                
                masterAllocationList.push_back(n1);
                masterAllocationList.push_back(n2);

                if (!head) { head = n1; tail = n1; headCopy = n2; tailCopy = n2; }
                else { tail->next = n1; tail = n1; tailCopy->next = n2; tailCopy = n2; }
            }
        }
        lists[static_cast<size_t>(i)] = head;
        listsCopy[static_cast<size_t>(i)] = headCopy;
    }

    Solution solver;

    // Test Run 1: Divide and Conquer
    cout << "\nExecuting Method 1 (Divide and Conquer Merge Sorting)..." << endl;
    ListNode* result1 = solver.mergeKListsDivideAndConquer(lists);
    cout << "Merged Output Structure (Method 1):\nHead -> ";
    ListNode* cursor = result1;
    while (cursor) { cout << "[" << cursor->val << "] -> "; cursor = cursor->next; }
    cout << "NULL\n";

    // Test Run 2: Min-Heap
    cout << "\nExecuting Method 2 (Min-Heap Multi-Pointer Alignment)..." << endl;
    ListNode* result2 = solver.mergeKListsMinHeap(listsCopy);
    cout << "Merged Output Structure (Method 2):\nHead -> ";
    cursor = result2;
    while (cursor) { cout << "[" << cursor->val << "] -> "; cursor = cursor->next; }
    cout << "NULL\n";

    // Dynamic memory cleanup loops using our collection tracker
    for (ListNode* node : masterAllocationList) {
        delete node;
    }
    
    return 0;
}