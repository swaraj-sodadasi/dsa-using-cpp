#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

using namespace std;

template <typename T>
class ArrayIteratorQueue {
private:
    vector<T> buffer;
    typename vector<T>::iterator head;
    typename vector<T>::iterator tail;
    size_t capacity;
    size_t count;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: QUEUE IMPLEMENTATION VIA ARRAY ITERATORS (TWO-WAY OFFSET TRACKING)
     * -------------------------------------------------------------------------------------
     * The objective is to construct a FIFO queue using contiguous memory iterators for 
     * head (dequeue position) and tail (enqueue position) pointer management.
     * 
     * 1. CONTIGUOUS ITERATOR WRAPAROUND:
     *    To maintain O(1) amortized operations without shifting elements, we maintain a 
     *    circular ring buffer layout. Head and tail iterators advance forward via vector offsets. 
     *    When an iterator reaches `buffer.end()`, it wraps around back to `buffer.begin()`.
     * 
     * 2. TWO-WAY OFFSET POINTER TRACKING:
     *    - Enqueue (push): Insert at `tail` iterator, increment element count, and advance `tail`.
     *    - Dequeue (pop): Fetch value at `head` iterator, decrement element count, and advance `head`.
     * 
     * 3. EXPLICIT NAMESPACE QUALIFICATION:
     *    - Explicitly qualified `std::move` to prevent ADL lookup warnings/issues.
     */
    explicit ArrayIteratorQueue(size_t cap = 10) : capacity(cap), count(0) {
        buffer.resize(capacity);
        head = buffer.begin();
        tail = buffer.begin();
    }

    void enqueue(const T& val) {
        if (count == capacity) {
            // Memory expansion: resize vector and re-align iterators to preserve circular offsets
            size_t newCapacity = capacity * 2;
            vector<T> newBuffer(newCapacity);

            // Copy elements in logical order starting from head
            for (size_t i = 0; i < count; ++i) {
                newBuffer[i] = *head;
                advanceIterator(head);
            }

            // Explicitly qualified std::move call
            buffer = std::move(newBuffer);
            capacity = newCapacity;
            head = buffer.begin();
            tail = buffer.begin() + static_cast<ptrdiff_t>(count);
        }

        *tail = val;
        advanceIterator(tail);
        count++;
    }

    void dequeue() {
        if (empty()) {
            throw runtime_error("Queue Underflow: Attempted to dequeue from an empty queue.");
        }
        advanceIterator(head);
        count--;
    }

    T front() const {
        if (empty()) {
            throw runtime_error("Queue Empty: Attempted to inspect front of an empty queue.");
        }
        return *head;
    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

private:
    void advanceIterator(typename vector<T>::iterator& it) {
        ++it;
        if (it == buffer.end()) {
            it = buffer.begin();
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) amortized per enqueue/dequeue operation using ring buffer iterators.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Capacity of contiguous vector buffer.
 */
int main() {
    cout << "=== Queue via Array Iterators Two-Way Offset Engine ===\n";
    cout << "Enter initial capacity for the queue: ";
    size_t initialCapacity;
    if (!(cin >> initialCapacity) || initialCapacity == 0) return 1;

    ArrayIteratorQueue<int> q(initialCapacity);

    cout << "Enter total number of dynamic operations to perform: ";
    int numOps;
    if (!(cin >> numOps) || numOps <= 0) return 1;

    cout << "\nOperations format:\n";
    cout << "  1 [val] : Enqueue value\n";
    cout << "  2       : Dequeue value\n";
    cout << "  3       : Peek front value\n";
    cout << "  4       : Print current size\n\n";

    for (int i = 0; i < numOps; ++i) {
        cout << "Operation [" << i + 1 << "/" << numOps << "]: ";
        int op;
        cin >> op;

        try {
            if (op == 1) {
                int val;
                cin >> val;
                q.enqueue(val);
                cout << "  -> Enqueued: " << val << "\n";
            } else if (op == 2) {
                int frontVal = q.front();
                q.dequeue();
                cout << "  -> Dequeued: " << frontVal << "\n";
            } else if (op == 3) {
                cout << "  -> Front Element: " << q.front() << "\n";
            } else if (op == 4) {
                cout << "  -> Current Queue Size: " << q.size() << "\n";
            } else {
                cout << "  -> Invalid operation option.\n";
            }
        } catch (const exception& e) {
            cout << "  -> Exception: " << e.what() << "\n";
        }
    }

    return 0;
}