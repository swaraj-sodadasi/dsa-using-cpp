#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MyCircularDeque {
private:
    vector<int> buffer;
    int headIndex;
    int tailIndex;
    int capacity;
    int currentSize;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DESIGN CIRCULAR DEQUE (DYNAMIC HEAD-TAIL ARRAY BOUNDARY SHIFTS)
     * -------------------------------------------------------------------------------------
     * The objective is to design a double-ended queue (deque) with a fixed-capacity buffer.
     * 
     * 1. THE CIRCULAR RING GEOMETRY:
     *    We implement the buffer as a continuous logical circle using index modulo arithmetic.
     *    This bypasses O(N) array shifts during insert/delete cycles at either boundary.
     * 
     * 2. ELEMENT POSITIONING PROTOCOLS:
     *    - headIndex: Points directly to the current front element.
     *    - tailIndex: Points directly to the current rear element.
     *    - Circular Decrement (Counter-Clockwise): `(index - 1 + capacity) % capacity`
     *    - Circular Increment (Clockwise): `(index + 1) % capacity`
     * 
     * 3. EXPANDED BOUNDARY RULES:
     *    - insertFront: Decrement the front pointer counter-clockwise, then write.
     *    - deleteFront: Increment the front pointer clockwise.
     *    - insertLast: Increment the rear pointer clockwise, then write.
     *    - deleteLast: Decrement the rear pointer counter-clockwise.
     */
    MyCircularDeque(int k) {
        capacity = k;
        buffer.assign(static_cast<size_t>(k), 0);
        headIndex = 0;
        tailIndex = 0;
        currentSize = 0;
    }

    bool insertFront(int value) {
        if (isFull()) return false;
        
        if (isEmpty()) {
            headIndex = 0;
            tailIndex = 0;
            buffer[0] = value;
        } else {
            // Shift headIndex counter-clockwise, then write element
            headIndex = (headIndex - 1 + capacity) % capacity;
            buffer[static_cast<size_t>(headIndex)] = value;
        }
        currentSize++;
        return true;
    }

    bool insertLast(int value) {
        if (isFull()) return false;
        
        if (isEmpty()) {
            headIndex = 0;
            tailIndex = 0;
            buffer[0] = value;
        } else {
            // Shift tailIndex clockwise, then write element
            tailIndex = (tailIndex + 1) % capacity;
            buffer[static_cast<size_t>(tailIndex)] = value;
        }
        currentSize++;
        return true;
    }

    bool deleteFront() {
        if (isEmpty()) return false;
        
        if (currentSize == 1) {
            headIndex = 0;
            tailIndex = 0;
        } else {
            // Shift headIndex clockwise to release reference
            headIndex = (headIndex + 1) % capacity;
        }
        currentSize--;
        return true;
    }

    bool deleteLast() {
        if (isEmpty()) return false;
        
        if (currentSize == 1) {
            headIndex = 0;
            tailIndex = 0;
        } else {
            // Shift tailIndex counter-clockwise to release reference
            tailIndex = (tailIndex - 1 + capacity) % capacity;
        }
        currentSize--;
        return true;
    }

    int getFront() {
        if (isEmpty()) return -1;
        return buffer[static_cast<size_t>(headIndex)];
    }

    int getRear() {
        if (isEmpty()) return -1;
        return buffer[static_cast<size_t>(tailIndex)];
    }

    bool isEmpty() {
        return currentSize == 0;
    }

    bool isFull() {
        return currentSize == capacity;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) for all methods - Index updates and array writes are completed 
 *                  instantly without restructuring the backing storage array.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Statically allocates a single container array of size K.
 */
int main() {
    cout << "=== Circular Deque Array Modulo Arithmetic Emulator ===\n";
    cout << "Enter the capacity of the circular deque: ";
    int k;
    if (!(cin >> k) || k <= 0) {
        cout << "Error: Invalid capacity.\n";
        return 1;
    }

    MyCircularDeque deque(k);
    cout << "Deque initialized with capacity " << k << ".\n";
    cout << "Commands: 'insertfront [val]', 'insertlast [val]', 'deletefront', 'deletelast', 'getfront', 'getrear', 'isempty', 'isfull', 'exit'\n\n";

    string command;
    while (cout << "> ", cin >> command) {
        if (command == "insertfront") {
            int val;
            if (cin >> val) {
                if (deque.insertFront(val)) {
                    cout << "Inserted [" << val << "] at Front.\n";
                } else {
                    cout << "InsertFront failed. Deque is full.\n";
                }
            }
        } else if (command == "insertlast") {
            int val;
            if (cin >> val) {
                if (deque.insertLast(val)) {
                    cout << "Inserted [" << val << "] at Rear.\n";
                } else {
                    cout << "InsertLast failed. Deque is full.\n";
                }
            }
        } else if (command == "deletefront") {
            if (deque.deleteFront()) {
                cout << "Deleted Front element successfully.\n";
            } else {
                cout << "DeleteFront failed. Deque is empty.\n";
            }
        } else if (command == "deletelast") {
            if (deque.deleteLast()) {
                cout << "Deleted Rear element successfully.\n";
            } else {
                cout << "DeleteLast failed. Deque is empty.\n";
            }
        } else if (command == "getfront") {
            int val = deque.getFront();
            if (val == -1) {
                cout << "Front: Deque is empty.\n";
            } else {
                cout << "Front element: [" << val << "]\n";
            }
        } else if (command == "getrear") {
            int val = deque.getRear();
            if (val == -1) {
                cout << "Rear: Deque is empty.\n";
            } else {
                cout << "Rear element: [" << val << "]\n";
            }
        } else if (command == "isempty") {
            cout << "Is Empty: " << (deque.isEmpty() ? "Yes" : "No") << "\n";
        } else if (command == "isfull") {
            cout << "Is Full: " << (deque.isFull() ? "Yes" : "No") << "\n";
        } else if (command == "exit") {
            break;
        } else {
            cout << "Unknown command configuration. Try again.\n";
            cin.clear();
            string clearBuffer;
            getline(cin, clearBuffer);
        }
    }

    return 0;
}