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
     * CORE LOGIC: LENGTH ARITHMETIC DYNAMIC ALLOCATION GROUPS
     * -------------------------------------------------------------------------------------
     * The objective is to split a singly-linked list into `k` consecutive parts. The length 
     * of each part should be as equal as possible, with no two parts differing in size by 
     * more than 1 node. Parts occurring earlier must have a size greater than or equal to 
     * parts occurring later.
     * 
     * 1. LENGTH DETERMINATION METRIC:
     *    We perform an initial forward pass to calculate the total length `N` of the list. 
     *    This length is used to compute how nodes should be distributed among the `k` parts.
     * 
     * 2. ARITHMETIC DISTRIBUTION LAWS:
     *    Using integer arithmetic, we break down the allocation pattern:
     *      - `baseSize = N / k`: The minimum number of nodes guaranteed to be in every part.
     *      - `extraNodes = N % k`: The total remainder nodes left over. Following the balanced 
     *        distribution rule, we distribute these remainder nodes one by one to the first 
     *        `extraNodes` parts.
     *    Consequently, the first `extraNodes` parts will have a size of `baseSize + 1`, 
     *    while the remaining parts will have a size of `baseSize`.
     * 
     * 3. IN-PLACE PARTITION SEVERING:
     *    We traverse the list, carving out the exact number of nodes calculated for each part. 
     *    We capture the head of the current segment, step forward to its calculated tail node, 
     *    sever its downstream connection (`tail->next = nullptr`) to isolate the subsegment, 
     *    and advance our cursor to the next part's head. If the nodes run out early, the 
     *    remaining slots in our return vector are filled with `nullptr`.
     */
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        vector<ListNode*> partitionedParts(static_cast<size_t>(k), nullptr);
        
        // Step 1: Compute total length of the list
        int totalLength = 0;
        ListNode* cursor = head;
        while (cursor != nullptr) {
            totalLength++;
            cursor = cursor->next;
        }

        // Step 2: Extract base size and remainder metrics via integer division
        int baseSize = totalLength / k;
        int extraNodes = totalLength % k;

        ListNode* curr = head;

        // Step 3: Segment the list dynamically into k isolated groups
        for (int i = 0; i < k && curr != nullptr; ++i) {
            partitionedParts[static_cast<size_t>(i)] = curr;
            
            // Calculate the exact capacity for the current slot block
            int currentPartSize = baseSize + (i < extraNodes ? 1 : 0);

            // Traverse to the last node of the current part segment
            for (int j = 1; j < currentPartSize; ++j) {
                curr = curr->next;
            }

            // Sever the link trailing behind the tail node to isolate the current part
            ListNode* nextPartHead = curr->next;
            curr->next = nullptr;
            curr = nextPartHead;
        }

        return partitionedParts;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + k) - Where N represents the total node count inside the linked list,
 *                  and k is the requested number of parts. The list is scanned once to compute 
 *                  length, and a second pass isolates the subsegments in linear time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The distribution is performed entirely in-place by updating 
 *                   existing pointer fields, consuming zero extra heap nodes (excluding the output vector).
 */
int main() {
    int elementsCount;
    int partsCountK;

    cout << "=== Length Arithmetic Dynamic Allocation Split Engine ===\n";
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

    cout << "Enter the number of parts to split the list into (k): ";
    if (!(cin >> partsCountK) || partsCountK <= 0) {
        cout << "Invalid part division count configuration." << endl;
        // Clean up memory before failure exit
        while (listHead) { ListNode* t = listHead; listHead = listHead->next; delete t; }
        return 1;
    }

    // Print initial list sequence layout
    cout << "\nOriginal Link Structure:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nRunning length division arithmetic and slicing list boundaries..." << endl;
    vector<ListNode*> splitResult = solver.splitListToParts(listHead, partsCountK);

    // Print resulting subsegment collections
    cout << "\nPartitioned List Parts Output Matrix:\n";
    for (size_t i = 0; i < splitResult.size(); ++i) {
        cout << "Part [" << i << "]: Head -> ";
        ListNode* partCursor = splitResult[i];
        if (!partCursor) {
            cout << "NULL (Empty Slot)";
        }
        while (partCursor) {
            cout << "[" << partCursor->val << "] -> ";
            partCursor = partCursor->next;
        }
        cout << "NULL\n";
    }

    // Clean up allocated heap memory nodes across all parts
    for (ListNode* partHead : splitResult) {
        while (partHead) {
            ListNode* tempNode = partHead;
            partHead = partHead->next;
            delete tempNode;
        }
    }

    return 0;
}