#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MyCircularQueue {
private:
    vector<int> buffer;
    int headIndex;
    int tailIndex;
    int capacity;
    int currentSize;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: FIXED-SIZE ARRAY WITH HEAD/TAIL ARRAY INDEX MODULO ARITHMETIC
     * -------------------------------------------------------------------------------------
     * The objective is to design a circular queue using a fixed-size contiguous memory array.
     * 
     * 1. MODULO CIRCULAR STEPPING:
     *    To prevent buffer shifting, we treat the backing array as a ring. When index registers 
     *    exceed the capacity, they wrap around to the beginning using modulo arithmetic:
     *      - Index Step: `(currentIndex + 1) % capacity`
     * 
     * 2. STATE INVARIANTS:
     *    - capacity: The maximum structural boundary limit.
     *    - currentSize: Tracks actual loaded elements, instantly resolving full/empty checks.
     *    - headIndex: Points to the first element (read boundary).
     *    - tailIndex: Points to the last element (write boundary).
     */
    MyCircularQueue(int k) {
        capacity = k;
        buffer.assign(static_cast<size_t>(k), 0);
        headIndex = 0;
        tailIndex = -1;
        currentSize = 0;
    }

    bool enQueue(int value) {
        if (isFull()) {
            return false;
        }
        // Increment the tail pointer and wrap it around using modulo arithmetic
        tailIndex = (tailIndex + 1) % capacity;
        buffer[static_cast<size_t>(tailIndex)] = value;
        currentSize++;
        return true;
    }

    bool deQueue() {
        if (isEmpty()) {
            return false;
        }
        // Increment the head pointer and wrap it around using modulo arithmetic
        headIndex = (headIndex + 1) % capacity;
        currentSize--;
        return true;
    }

    int Front() {
        if (isEmpty()) {
            return -1;
        }
        return buffer[static_cast<size_t>(headIndex)];
    }

    int Rear() {
        if (isEmpty()) {
            return -1;
        }
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
 * TIME COMPLEXITY: O(1) for all operations - Ring buffer tracking does not require elements to be shifted.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Statically allocates a single container array of size K.
 */
int main() {
    cout << "=== Dynamic Circular Queue Emulator ===\n";
    cout << "Enter the capacity of the circular queue: ";
    int k;
    if (!(cin >> k) || k <= 0) {
        cout << "Error: Invalid capacity entered.\n";
        return 1;
    }

    MyCircularQueue cq(k);
    cout << "Queue initialized with capacity " << k << ".\n";
    cout << "Commands: 'enqueue [val]', 'dequeue', 'front', 'rear', 'isempty', 'isfull', 'exit'\n\n";

    string command;
    while (cout << "> ", cin >> command) {
        if (command == "enqueue") {
            int val;
            if (cin >> val) {
                if (cq.enQueue(val)) {
                    cout << "Enqueued [" << val << "] successfully.\n";
                } else {
                    cout << "Enqueue failed. Queue is full.\n";
                }
            }
        } else if (command == "dequeue") {
            if (cq.deQueue()) {
                cout << "Dequeued front element successfully.\n";
            } else {
                cout << "Dequeue failed. Queue is empty.\n";
            }
        } else if (command == "front") {
            int val = cq.Front();
            if (val == -1) {
                cout << "Front: Queue is empty.\n";
            } else {
                cout << "Front element: [" << val << "]\n";
            }
        } else if (command == "rear") {
            int val = cq.Rear();
            if (val == -1) {
                cout << "Rear: Queue is empty.\n";
            } else {
                cout << "Rear element: [" << val << "]\n";
            }
        } else if (command == "isempty") {
            cout << "Is Empty: " << (cq.isEmpty() ? "Yes" : "No") << "\n";
        } else if (command == "isfull") {
            cout << "Is Full: " << (cq.isFull() ? "Yes" : "No") << "\n";
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