#include <iostream>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// Definition for a Node with horizontal (next) and vertical (bottom) tiers.
struct Node {
    int data;
    Node* next;
    Node* bottom;
    Node(int x) : data(x), next(nullptr), bottom(nullptr) {}
};

// Custom comparator structure for organizing slices within the Min-Heap array configuration
struct CompareSlices {
    bool operator()(const Node* lhs, const Node* rhs) const {
        // Enforces low-to-high absolute sort priority alignment bounds
        return lhs->data > rhs->data;
    }
};

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-TIER VERTICAL HEAP SLICES EXTRACTION SORTING
     * -------------------------------------------------------------------------------------
     * The objective is to flatten a multi-tier linked list where each horizontal head node 
     * points to a sorted vertical sub-linked list via its `bottom` pointer. The flattened list 
     * must be completely sorted in ascending order and linked exclusively via `bottom` pointers.
     * 
     * 1. THE RECURSIVE PAIRWISE OVERHEAD AVOIDANCE:
     *    A common strategy relies on recursively merging two vertical chains at a time. 
     *    However, if there are $K$ chains and $N$ total elements, pairwise merging incurs a worst-case 
     *    runtime complexity of $O(K \cdot N)$. 
     *    Instead, we use a Min-Heap (Priority Queue) to extract elements optimally across all 
     *    vertical slices simultaneously, lowering execution steps to $O(N \log K)$.
     * 
     * 2. THE MULTI-TIER SLICE EXTRACTION METHOD:
     *    - Step A: We scan horizontally along the top level using the `next` pointers. 
     *      Every valid head node represents the entry anchor of a sorted vertical slice. We push 
     *      these initial nodes directly into our Min-Heap priority queue.
     *    - Step B: We allocate a stack-bound `dummy` node placeholder to form the clean backbone 
     *      of our flattened chain without handling pointer exceptions for the first element.
     *    - Step C: As long as the heap contains slice nodes, we extract the minimal element (`curr`). 
     *      We detach its `next` pointer to ensure a pure single-level structure and attach it to our 
     *      flattened track (`tail->bottom = curr`).
     *    - Step D: If the extracted node contains a vertical successor node (`curr->bottom != nullptr`), 
     *      we push that successor back into the Min-Heap to keep the slice extraction lane active.
     */
    Node* flatten(Node* root) {
        if (!root) return nullptr;

        // Step A: Priority queue initialized to track dynamic minimum boundaries across tiers
        priority_queue<Node*, vector<Node*>, CompareSlices> minHeap;

        // Traverse the horizontal top lane to push the absolute head pointers of all slices
        Node* horizontalScanner = root;
        while (horizontalScanner != nullptr) {
            minHeap.push(horizontalScanner);
            horizontalScanner = horizontalScanner->next;
        }

        // Step B & C: Build the flattened backbone sequence using a dummy anchor
        Node dummy(0);
        Node* tail = &dummy;

        while (!minHeap.empty()) {
            Node* currentMinNode = minHeap.top();
            minHeap.pop();

            // Stitch onto the vertical bottom chain and isolate it from horizontal links
            tail->bottom = currentMinNode;
            tail = tail->bottom;
            tail->next = nullptr; // Explicitly sever the next track to keep the structure flat

            // Step D: If a vertical child node exists below, push it to fill the slice pipeline
            if (currentMinNode->bottom != nullptr) {
                minHeap.push(currentMinNode->bottom);
            }
        }

        return dummy.bottom;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log K) - Where N represents the total node count across all vertical and 
 *                  horizontal chains, and K is the total number of horizontal sub-list heads. 
 *                  Each insertion and deletion step inside the heap scales logarithmically with list width.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - The priority queue holds at most K elements at any given execution 
 *                   frame (one active pointer candidate per vertical slice), making it highly memory efficient.
 */
int main() {
    cout << "=== Multi-Tier Vertical Slice Heap Sorting Engine ===\n";
    cout << "Enter the number of horizontal head nodes (K): ";
    int kCount;
    if (!(cin >> kCount) || kCount <= 0) return 0;

    Node* mainHead = nullptr;
    Node* mainTail = nullptr;
    
    // Dynamic tracking buffer to ensure all allocations are cleaned up without memory leaks
    vector<Node*> masterAllocationTracker;

    cin.ignore(); // Flush newline buffer residue

    for (int i = 0; i < kCount; ++i) {
        cout << "\nEnter space-separated sorted integers for vertical chain slot [" << i << "]:\n";
        string inputLine;
        getline(cin, inputLine);
        stringstream ss(inputLine);
        
        int val;
        Node* verticalHead = nullptr;
        Node* verticalTail = nullptr;

        while (ss >> val) {
            Node* freshNode = new Node(val);
            masterAllocationTracker.push_back(freshNode);

            if (!verticalHead) {
                verticalHead = freshNode;
                verticalTail = freshNode;
            } else {
                verticalTail->bottom = freshNode;
                verticalTail = freshNode;
            }
        }

        // Connect the newly built vertical segment list to the main horizontal list structure
        if (verticalHead != nullptr) {
            if (!mainHead) {
                mainHead = verticalHead;
                mainTail = verticalHead;
            } else {
                mainTail->next = verticalHead;
                mainTail = verticalHead;
            }
        }
    }

    // Print initial multi-tier layout before restructuring
    cout << "\nInitial Multi-Tier Structured View:\n";
    Node* horiz = mainHead;
    int index = 0;
    while (horiz != nullptr) {
        cout << "Head [" << index++ << "]: ";
        Node* vert = horiz;
        while (vert != nullptr) {
            cout << "[" << vert->data << "] -> ";
            vert = vert->bottom;
        }
        cout << "NULL\n";
        horiz = horiz->next;
    }

    Solution solver;
    cout << "\nExtracting vertical slices and establishing heap-sorted linear alignment..." << endl;
    Node* flattenedOutputHead = solver.flatten(mainHead);

    // Print resulting single-level sequence linked by bottom pointers
    cout << "\nFlattened Outcome Sorted Array Matrix:\nHead -> ";
    Node* printCursor = flattenedOutputHead;
    while (printCursor != nullptr) {
        cout << "[" << printCursor->data << "] -> ";
        printCursor = printCursor->bottom; // Verified using the target bottom axis path
    }
    cout << "NULL\n";

    // Clean up all allocated dynamic heap node blocks safely
    for (Node* activeAllocNode : masterAllocationTracker) {
        delete activeAllocNode;
    }
    masterAllocationTracker.clear();

    return 0;
}