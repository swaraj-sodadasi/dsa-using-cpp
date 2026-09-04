#include <iostream>
#include <vector>

using namespace std;

class MyCircularDeque {
private:
    vector<int> buffer;
    int head;
    int tail;
    int capacity;
    int count;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DESIGN CIRCULAR DEQUE (MODULO BOUNDS SHIFT TRACKING)
     * -------------------------------------------------------------------------------------
     * The objective is to design a circular double-ended queue (deque) with fixed capacity 
     * where all operations perform in O(1) time complexity.
     * 
     * 1. BOUNDARY INDEXING LAYOUT:
     *    - `head`: Points directly to the current front element index.
     *    - `tail`: Points directly to the current rear element index.
     * 
     * 2. MODULO BOUNDS SHIFT FORMULAS:
     *    - insertFront: `head = (head - 1 + capacity) % capacity`
     *    - insertLast:  `tail = (tail + 1) % capacity`
     *    - deleteFront: `head = (head + 1) % capacity`
     *    - deleteLast:  `tail = (tail - 1 + capacity) % capacity`
     * 
     * 3. INITIALIZATION ORDER:
     *    - Member initialization list order now matches class member declaration order 
     *      (buffer -> head -> tail -> capacity -> count) to eliminate -Wreorder warnings.
     */
    explicit MyCircularDeque(int k) : head(0), tail(k - 1), capacity(k), count(0) {
        buffer.resize(static_cast<size_t>(capacity));
    }

    bool insertFront(int value) {
        if (isFull()) return false;
        head = (head - 1 + capacity) % capacity;
        buffer[static_cast<size_t>(head)] = value;
        count++;
        return true;
    }

    bool insertLast(int value) {
        if (isFull()) return false;
        tail = (tail + 1) % capacity;
        buffer[static_cast<size_t>(tail)] = value;
        count++;
        return true;
    }

    bool deleteFront() {
        if (isEmpty()) return false;
        head = (head + 1) % capacity;
        count--;
        return true;
    }

    bool deleteLast() {
        if (isEmpty()) return false;
        tail = (tail - 1 + capacity) % capacity;
        count--;
        return true;
    }

    int getFront() const {
        if (isEmpty()) return -1;
        return buffer[static_cast<size_t>(head)];
    }

    int getRear() const {
        if (isEmpty()) return -1;
        return buffer[static_cast<size_t>(tail)];
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) for all operations (insert, delete, front, rear, empty, full).
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Buffer allocated for maximum capacity K.
 */
int main() {
    cout << "=== Design Circular Deque Engine ===\n";
    cout << "Enter capacity for circular deque: ";
    int cap;
    if (!(cin >> cap) || cap <= 0) return 1;

    MyCircularDeque deque(cap);

    cout << "Enter total number of dynamic operations to perform: ";
    int numOps;
    if (!(cin >> numOps) || numOps <= 0) return 1;

    cout << "\nOperations format:\n";
    cout << "  1 [val] : Insert Front\n";
    cout << "  2 [val] : Insert Last\n";
    cout << "  3       : Delete Front\n";
    cout << "  4       : Delete Last\n";
    cout << "  5       : Get Front\n";
    cout << "  6       : Get Rear\n\n";

    for (int i = 0; i < numOps; ++i) {
        cout << "Operation [" << i + 1 << "/" << numOps << "]: ";
        int op;
        cin >> op;

        if (op == 1) {
            int val;
            cin >> val;
            bool res = deque.insertFront(val);
            cout << "  -> Insert Front " << val << ": " << (res ? "Success" : "Failed (Full)") << "\n";
        } else if (op == 2) {
            int val;
            cin >> val;
            bool res = deque.insertLast(val);
            cout << "  -> Insert Last " << val << ": " << (res ? "Success" : "Failed (Full)") << "\n";
        } else if (op == 3) {
            bool res = deque.deleteFront();
            cout << "  -> Delete Front: " << (res ? "Success" : "Failed (Empty)") << "\n";
        } else if (op == 4) {
            bool res = deque.deleteLast();
            cout << "  -> Delete Last: " << (res ? "Success" : "Failed (Empty)") << "\n";
        } else if (op == 5) {
            cout << "  -> Front Element: " << deque.getFront() << "\n";
        } else if (op == 6) {
            cout << "  -> Rear Element: " << deque.getRear() << "\n";
        } else {
            cout << "  -> Invalid operation option.\n";
        }
    }

    return 0;
}