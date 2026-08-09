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
     * CORE LOGIC: BITWISE ACCUMULATION ON TRAVERSAL
     * -------------------------------------------------------------------------------------
     * The objective is to convert a binary number represented by a singly-linked list 
     * where each node stores a single bit (0 or 1), most significant bit first, into its 
     * corresponding decimal integer value.
     * 
     * 1. THE REVERSE METRIC AVOIDANCE:
     *    A naive conversion involves reversing the list or storing the bits inside an array 
     *    to compute powers of 2 starting from the least significant bit (LSB). This takes 
     *    unnecessary $O(N)$ space or forces a double pass over the nodes.
     *    Instead, we use Horner's method for polynomial evaluation adapted for base-2 numbers. 
     *    This allows us to construct the integer iteratively in a single forward pass.
     * 
     * 2. BITWISE ACCUMULATION ENGINE:
     *    We maintain an accumulator variable `decimalValue` initialized to 0. As we step through 
     *    the nodes from head to tail:
     *      - Every step forward shift existing bit sequences up by 1 position to the left. 
     *        This multiplies the current partial value by 2.
     *      - We then append the new bit value at the current node via a bitwise OR operation.
     * 
     *    Equation: `decimalValue = (decimalValue << 1) | curr->val`
     */
    int getDecimalValue(ListNode* head) {
        int decimalValue = 0;
        ListNode* curr = head;

        // Step 2: Traverse sequentially and accumulate bits using low-level registers
        while (curr != nullptr) {
            // Shift current bits left by 1 (multiply by 2) and merge the node value
            decimalValue = (decimalValue << 1) | curr->val;
            curr = curr->next;
        }

        return decimalValue;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total node count inside the singly-linked list.
 *                  The accumulation engine processes each bit node exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The transformation runs entirely in place using localized 
 *                   register arithmetic on the stack, consuming zero additional heap allocations.
 */
int main() {
    int bitCount;
    cout << "=== Bitwise Accumulation Binary Conversion Engine ===\n";
    cout << "Enter the total bit width count (number of nodes): ";
    if (!(cin >> bitCount) || bitCount < 0) return 1;

    ListNode* listHead = nullptr;
    ListNode* trackingTail = nullptr;

    if (bitCount > 0) {
        cout << "Enter the binary bits (0 or 1) sequentially from MSB to LSB:\n";
        for (int i = 0; i < bitCount; ++i) {
            int bitVal;
            cin >> bitVal;
            if (bitVal != 0 && bitVal != 1) {
                cout << "Invalid Input: Linked list nodes can only contain binary values (0 or 1).\n";
                // Clean up partial allocations before failure exit
                while (listHead) {
                    ListNode* temp = listHead;
                    listHead = listHead->next;
                    delete temp;
                }
                return 1;
            }
            ListNode* freshNode = new ListNode(bitVal);
            if (!listHead) {
                listHead = freshNode;
                trackingTail = freshNode;
            } else {
                trackingTail->next = freshNode;
                trackingTail = freshNode;
            }
        }
    }

    // Print initial list layout structure
    cout << "\nBinary Linked List Layout:\nHead -> ";
    ListNode* displayCursor = listHead;
    while (displayCursor) {
        cout << "[" << displayCursor->val << "] -> ";
        displayCursor = displayCursor->next;
    }
    cout << "NULL\n";

    Solution solver;
    cout << "\nExecuting inline shift register operations and computing base-10 value..." << endl;
    int evaluatedDecimal = solver.getDecimalValue(listHead);

    cout << "\nCalculated Decimal Outcome Value: " << evaluatedDecimal << "\n";

    // Clean up allocated heap memory nodes
    while (listHead) {
        ListNode* tempNode = listHead;
        listHead = listHead->next;
        delete tempNode;
    }

    return 0;
}