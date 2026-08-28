#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>

using namespace std;

/*
 * -------------------------------------------------------------------------------------
 * APPROACH 1: SINGLE-QUEUE ROTATIONAL LIFO EMULATION (O(N) Push, O(1) Pop)
 * -------------------------------------------------------------------------------------
 * Uses only a single queue. When pushing a new element:
 *   1. Enqueue the element to the back of the queue.
 *   2. Rotate the queue by shifting all previous (N-1) elements to the back.
 * This puts the newest element at the front of the queue, preserving LIFO order.
 * 
 * TIME COMPLEXITY: Push: O(N), Pop/Top/Empty: O(1)
 * SPACE COMPLEXITY: O(N) total space to store the queue elements.
 */
class MyStackSingleQueue {
private:
    queue<int> q;

public:
    MyStackSingleQueue() {}

    void push(int x) {
        q.push(x);
        int currentSize = static_cast<int>(q.size());
        // Rotate the queue to place the newly inserted element at the front
        for (int i = 0; i < currentSize - 1; ++i) {
            q.push(q.front());
            q.pop();
        }
    }

    int pop() {
        if (q.empty()) throw runtime_error("Stack is empty");
        int topVal = q.front();
        q.pop();
        return topVal;
    }

    int top() {
        if (q.empty()) throw runtime_error("Stack is empty");
        return q.front();
    }

    bool empty() {
        return q.empty();
    }
};

/*
 * -------------------------------------------------------------------------------------
 * APPROACH 2: TWO-QUEUE DEFERRED ROTATION (O(1) Push, O(N) Pop)
 * -------------------------------------------------------------------------------------
 * Employs two queues: q1 (primary storage) and q2 (temporary migration buffer).
 * Elements are pushed directly into q1 in O(1) time. 
 * During pop/top operations, we migrate elements to reverse-align the LIFO sequence.
 * 
 * TIME COMPLEXITY: Push: O(1), Pop/Top: O(N), Empty: O(1)
 * SPACE COMPLEXITY: O(N) total space distributed across two queue containers.
 */
class MyStackTwoQueues {
private:
    queue<int> q1;
    queue<int> q2;

public:
    MyStackTwoQueues() {}

    void push(int x) {
        q1.push(x);
    }

    int pop() {
        if (empty()) throw runtime_error("Stack is empty");
        
        // Transfer N-1 elements from q1 to q2, leaving the last pushed element in q1
        while (q1.size() > 1) {
            q2.push(q1.front());
            q1.pop();
        }
        
        int targetElement = q1.front();
        q1.pop(); // Remove the target LIFO element
        
        // Swap names to restore q1 as the primary storage container
        swap(q1, q2);
        
        return targetElement;
    }

    int top() {
        if (empty()) throw runtime_error("Stack is empty");
        
        while (q1.size() > 1) {
            q2.push(q1.front());
            q1.pop();
        }
        
        int targetElement = q1.front();
        q2.push(targetElement); // Retain the top element by moving it to the buffer
        q1.pop();
        
        swap(q1, q2);
        
        return targetElement;
    }

    bool empty() {
        return q1.empty();
    }
};

int main() {
    cout << "=== Queue-Based LIFO Emulator Engine ===\n";
    cout << "Select Implementation Engine:\n";
    cout << "1. Single-Queue Rotational Method (O(N) Push, O(1) Pop)\n";
    cout << "2. Two-Queue Deferred Rotation Method (O(1) Push, O(N) Pop)\n";
    cout << "Selection (1 or 2): ";
    
    int engineChoice;
    if (!(cin >> engineChoice) || (engineChoice != 1 && engineChoice != 2)) {
        cout << "Invalid selection. Exiting.\n";
        return 1;
    }

    MyStackSingleQueue singleQStack;
    MyStackTwoQueues dualQStack;

    cout << "\nEngine #" << engineChoice << " active.\n";
    cout << "Commands: 'push [val]', 'pop', 'top', 'empty', 'exit'\n\n";

    string command;
    while (cout << "> ", cin >> command) {
        try {
            if (command == "push") {
                int val;
                if (cin >> val) {
                    if (engineChoice == 1) {
                        singleQStack.push(val);
                    } else {
                        dualQStack.push(val);
                    }
                    cout << "Pushed [" << val << "] successfully.\n";
                }
            } else if (command == "pop") {
                int poppedVal = (engineChoice == 1) ? singleQStack.pop() : dualQStack.pop();
                cout << "Popped value: [" << poppedVal << "]\n";
            } else if (command == "top") {
                int topVal = (engineChoice == 1) ? singleQStack.top() : dualQStack.top();
                cout << "Top value: [" << topVal << "]\n";
            } else if (command == "empty") {
                bool isEmpty = (engineChoice == 1) ? singleQStack.empty() : dualQStack.empty();
                cout << "Is Empty: " << (isEmpty ? "Yes" : "No") << "\n";
            } else if (command == "exit") {
                break;
            } else {
                cout << "Unknown command. Try again.\n";
                cin.clear();
                string clearBuffer;
                getline(cin, clearBuffer);
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}