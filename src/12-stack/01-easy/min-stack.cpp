#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

using namespace std;

/*
 * =====================================================================================
 * METHOD 1: DUAL-STACK VALUE TRACKING
 * =====================================================================================
 * This approach maintains two separate, synchronized stack containers:
 *   1. mainStack: Stores all elements pushed into the data structure sequentially.
 *   2. minStack: Tracks the historical minimum values. The top element always reflects 
 *      the current minimum state of the cache.
 * 
 * TIME COMPLEXITY: O(1) for all operations.
 * SPACE COMPLEXITY: O(N) space overhead due to the second tracking stack.
 */
class MinStackDual {
private:
    stack<int> mainStack;
    stack<int> minStack;

public:
    void push(int val) {
        mainStack.push(val);
        // If the minStack is empty or the incoming value is less than or equal to the 
        // current minimum, push it onto the minStack.
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }

    void pop() {
        if (mainStack.empty()) {
            throw runtime_error("Stack Underflow: Cannot pop from an empty stack.");
        }
        // If the element leaving the main stack matches the current minimum, pop it from minStack too.
        if (mainStack.top() == minStack.top()) {
            minStack.pop();
        }
        mainStack.pop();
    }

    int top() {
        if (mainStack.empty()) {
            throw runtime_error("Stack Empty Error: No top element available.");
        }
        return mainStack.top();
    }

    int getMin() {
        if (minStack.empty()) {
            throw runtime_error("Stack Empty Error: No minimum value tracked.");
        }
        return minStack.top();
    }
};


/*
 * =====================================================================================
 * METHOD 2: SINGLE-STACK MATH DISPLACEMENT FORMULA
 * =====================================================================================
 * This architecture achieves true constant O(1) auxiliary space optimization by encoding 
 * historical state displacements inside a single stack frame using standard 64-bit integers (`long long`).
 * 
 * MATH DISPLACEMENT LOGIC:
 *   - When pushing a value that is strictly smaller than our current global minimum (`val < currentMin`):
 *     We encode a compressed displacement flag value: $\text{displacement} = 2 \times \text{val} - \text{currentMin}$.
 *     Because $\text{val} < \text{currentMin}$, this flag is guaranteed to be strictly less than $\text{val}$ 
 *     ($2 \times \text{val} - \text{currentMin} < \text{val}$). This unique mathematical property lets us identify 
 *     the exact points where the minimum changes. We then update our tracking metric: $\text{currentMin} = \text{val}$.
 * 
 *   - When popping an element, if the stack top value is less than `currentMin`, it triggers a restore sequence:
 *     We reconstruct the previous minimum state using the inverse equation:
 *     $\text{previousMin} = 2 \times \text{currentMin} - \text{stackTop}$.
 *     We then update `currentMin = previousMin` before popping.
 * 
 * TIME COMPLEXITY: O(1) for all operations.
 * SPACE COMPLEXITY: O(1) Auxiliary space (Excluding the primary data storage stack itself).
 */
class MinStackMath {
private:
    stack<long long> s;
    long long currentMin;

public:
    MinStackMath() : currentMin(0) {}

    void push(int val) {
        long long value = val;
        if (s.empty()) {
            s.push(value);
            currentMin = value;
        } else if (value >= currentMin) {
            s.push(value);
        } else {
            // Encode the displacement flag and update the current minimum
            s.push(2 * value - currentMin);
            currentMin = value;
        }
    }

    void pop() {
        if (s.empty()) {
            throw runtime_error("Stack Underflow: Cannot pop from an empty stack.");
        }
        
        long long topElement = s.top();
        s.pop();

        // If the top element is less than currentMin, it's an encoded displacement flag.
        // We must restore the previous historical minimum state.
        if (topElement < currentMin) {
            currentMin = 2 * currentMin - topElement;
        }
    }

    int top() {
        if (s.empty()) {
            throw runtime_error("Stack Empty Error: No top element available.");
        }

        long long topElement = s.top();
        // If the top element is less than currentMin, the currentMin itself is the actual value.
        if (topElement < currentMin) {
            return static_cast<int>(currentMin);
        }
        return static_cast<int>(topElement);
    }

    int getMin() {
        if (s.empty()) {
            throw runtime_error("Stack Empty Error: No minimum value tracked.");
        }
        return static_cast<int>(currentMin);
    }
};

/*
 * MAIN SIMULATION RUNTIME CONSOLE
 */
int main() {
    cout << "=== Min Stack Multi-Engine Console ===\n";
    cout << "Select Underlying Engine Style:\n[1] Dual-Stack Value Tracking\n[2] Single-Stack Math Displacement Formula\nChoice: ";
    
    int choice;
    if (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Invalid implementation framework selection." << endl;
        return 1;
    }

    MinStackDual dualStack;
    MinStackMath mathStack;

    cout << "\nEngine ready. Commands: 'push [val]', 'pop', 'top', 'getMin', 'exit'\n\n";
    string cmd;

    while (cout << "> ", cin >> cmd) {
        try {
            if (cmd == "push") {
                int val;
                if (cin >> val) {
                    if (choice == 1) dualStack.push(val);
                    else mathStack.push(val);
                    cout << "Pushed value: " << val << "\n";
                }
            } else if (cmd == "pop") {
                if (choice == 1) dualStack.pop();
                else mathStack.pop();
                cout << "Successfully executed pop step.\n";
            } else if (cmd == "top") {
                int topVal = (choice == 1) ? dualStack.top() : mathStack.top();
                cout << "Top Element Value Matrix: [" << topVal << "]\n";
            } else if (cmd == "getMin") {
                int minVal = (choice == 1) ? dualStack.getMin() : mathStack.getMin();
                cout << "Current Minimum State Tracked: [" << minVal << "]\n";
            } else if (cmd == "exit") {
                break;
            } else {
                cout << "Unknown command sequence. Please try again.\n";
                cin.clear();
                string clearBuffer;
                getline(cin, clearBuffer);
            }
        } catch (const exception& ex) {
            cout << "Operational Exception: " << ex.what() << "\n";
        }
    }

    return 0;
}