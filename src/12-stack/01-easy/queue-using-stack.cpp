#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>

using namespace std;

class MyQueue {
private:
    stack<int> inputStack;  // Buffered write lane (receives all push operations)
    stack<int> outputStack; // Buffered read lane (handles all peek/pop operations)

    /*
     * HELPER FUNCTION: INTERNAL NODE TRANSFER PRIMITIVE
     * Transfers all elements from the input stack to the output stack.
     * This operation is only triggered when the output stack runs completely empty.
     * By popping from the input stack and pushing onto the output stack, 
     * the elements are reversed, transforming LIFO order into FIFO order.
     */
    void shiftStacks() {
        if (outputStack.empty()) {
            while (!inputStack.empty()) {
                outputStack.push(inputStack.top());
                inputStack.pop();
            }
        }
    }

public:
    MyQueue() {}

    /** Push element x to the back of queue. */
    void push(int x) {
        inputStack.push(x);
    }

    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        if (empty()) {
            throw runtime_error("Queue Underflow Error: Cannot pop from an empty queue structure.");
        }
        
        // Ensure the read lane has elements available
        shiftStacks();
        
        int frontValue = outputStack.top();
        outputStack.pop();
        return frontValue;
    }

    /** Get the front element. */
    int peek() {
        if (empty()) {
            throw runtime_error("Queue Empty Error: Cannot inspect the head of an empty queue.");
        }
        
        // Ensure the read lane has elements available
        shiftStacks();
        
        return outputStack.top();
    }

    /** Returns whether the queue is empty. */
    bool empty() {
        return inputStack.empty() && outputStack.empty();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY:
 *   - push(): O(1) Constant time. Elements are pushed directly onto the input stack.
 *   - pop() / peek(): O(1) Amortized Time. While shifting elements between stacks takes O(N) time, 
 *     each element is shifted at most once throughout its entire lifecycle. The remaining operations 
 *     run in pure O(1) constant time, keeping the average cost per operation low.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary Space - The total elements are shared across the two internal stacks.
 */
int main() {
    cout << "=== Amortized Dual-Stack Queue Simulation Console ===\n";
    MyQueue queue;

    cout << "Commands: 'push [val]', 'pop', 'peek', 'empty', 'exit'\n\n";
    string cmd;

    while (cout << "> ", cin >> cmd) {
        try {
            if (cmd == "push") {
                int val;
                if (cin >> val) {
                    queue.push(val);
                    cout << "Enqueued element: " << val << "\n";
                }
            } else if (cmd == "pop") {
                int poppedVal = queue.pop();
                cout << "Dequeued element value: [" << poppedVal << "]\n";
            } else if (cmd == "peek") {
                int frontVal = queue.peek();
                cout << "Front element tracking value: [" << frontVal << "]\n";
            } else if (cmd == "empty") {
                cout << "Queue State Matrix Evaluation: " << (queue.empty() ? "[Empty]" : "[Contains Elements]") << "\n";
            } else if (cmd == "exit") {
                break;
            } else {
                cout << "Unknown command syntax configuration sequence. Retry.\n";
                cin.clear();
                string clearBuffer;
                getline(cin, clearBuffer);
            }
        } catch (const exception& ex) {
            cout << "Operational Exception Boundary Triggered: " << ex.what() << "\n";
        }
    }

    return 0;
}