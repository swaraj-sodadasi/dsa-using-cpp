#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Definition for a Node in a linked list with a random pointer.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = nullptr;
        random = nullptr;
    }
};

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DEEP COPY NODE-TO-NODE ADDRESS MAPPING
     * -------------------------------------------------------------------------------------
     * The objective is to construct a complete deep copy of a linked list where each node 
     * contains an additional 'random' pointer pointing to any node in the list or null.
     */
    Node* copyRandomList(Node* head) {
        if (head == nullptr) {
            return nullptr;
        }

        // Map stores: {Original_Node_Address -> Cloned_Node_Address}
        unordered_map<Node*, Node*> visitedHash;

        // Pass 1: Clone all nodes independently and catalog their address associations
        Node* curr = head;
        while (curr != nullptr) {
            visitedHash[curr] = new Node(curr->val);
            curr = curr->next;
        }

        // Pass 2: Connect structural next and random tracking pointers across the cloned network
        curr = head;
        while (curr != nullptr) {
            visitedHash[curr]->next = visitedHash[curr->next];
            // FIX: Explicitly qualified with 'std::' to avoid potential unqualified lookup warnings
            visitedHash[curr]->random = visitedHash[curr->random];
            curr = curr->next;
        }

        // Return the entry head node of the newly cloned deep copy list structure
        return visitedHash[head];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of nodes in the linked list.
 * SPACE COMPLEXITY: O(N) Auxiliary - Space scaled linearly to track mapping nodes.
 */

// Helper utility function to display the structural contents of the list
void printList(Node* head) {
    Node* curr = head;
    while (curr != nullptr) {
        cout << "[Val: " << curr->val << ", Random Val: ";
        if (curr->random != nullptr) {
            cout << curr->random->val;
        } else {
            cout << "null";
        }
        cout << "] -> ";
        curr = curr->next;
    }
    cout << "nullptr\n";
}

// Helper utility function to clean up allocated node memory space
void freeList(Node* head) {
    Node* curr = head;
    while (curr != nullptr) {
        Node* temp = curr->next;
        delete curr;
        curr = temp;
    }
}

int main() {
    int nodeCount;
    cout << "Enter the total number of nodes to create in the linked list: ";
    if (!(cin >> nodeCount) || nodeCount < 0) {
        cout << "Invalid count parameter configured." << endl;
        return 1;
    }

    if (nodeCount == 0) {
        cout << "Original Linked List Structure: nullptr\n";
        Solution solver;
        Node* clonedHead = solver.copyRandomList(nullptr);
        
        // FIX: Pass clonedHead to printList to clear the unused-variable warning safely
        cout << "Cloned Deep Copy Linked List Result: ";
        printList(clonedHead); 
        return 0;
    }

    // Temporary tracking collection vector to help stitch sequential and random links
    vector<Node*> nodeArray(static_cast<size_t>(nodeCount));
    vector<int> randomIndices(static_cast<size_t>(nodeCount));

    cout << "\nEnter the values for each node sequentially:\n";
    for (int i = 0; i < nodeCount; ++i) {
        int value;
        cout << "Node [" << i << "] value: ";
        cin >> value;
        nodeArray[static_cast<size_t>(i)] = new Node(value);
    }

    // Connect sequential next pointers
    for (int i = 0; i < nodeCount - 1; ++i) {
        nodeArray[static_cast<size_t>(i)]->next = nodeArray[static_cast<size_t>(i + 1)];
    }

    cout << "\nEnter the 0-indexed position for each random pointer (-1 for null):\n";
    for (int i = 0; i < nodeCount; ++i) {
        int randomIndex;
        cout << "Node [" << i << "] random targets index (0 to " << nodeCount - 1 << "): ";
        cin >> randomIndex;

        if (randomIndex >= 0 && randomIndex < nodeCount) {
            nodeArray[static_cast<size_t>(i)]->random = nodeArray[static_cast<size_t>(randomIndex)];
        } else {
            nodeArray[static_cast<size_t>(i)]->random = nullptr;
        }
    }

    Node* originalHead = nodeArray[0];

    cout << "\nOriginal Linked List Structure:\n";
    printList(originalHead);

    Solution solver;
    cout << "\nExecuting deep copy node-to-node address mapping..." << endl;
    Node* clonedHead = solver.copyRandomList(originalHead);

    cout << "\nCloned Deep Copy Linked List Result:\n";
    printList(clonedHead);

    // Verify address uniqueness to confirm a true deep copy was created
    cout << "\nVerification check: " 
         << (clonedHead != originalHead ? "SUCCESS: Different head memory locations verified." : "FAIL") << endl;

    // Clean up memory allocations
    freeList(originalHead);
    freeList(clonedHead);

    return 0;
}