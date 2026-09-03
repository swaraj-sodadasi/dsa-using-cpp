#include <iostream>
#include <deque>

using namespace std;

class FrontMiddleBackQueue {
private:
    deque<int> leftQueue;
    deque<int> rightQueue;

    // Balance the two deques such that `leftQueue` holds either the same number 
    // of elements as `rightQueue` or exactly one element fewer.
    void balance() {
        if (leftQueue.size() > rightQueue.size() + 1) {
            rightQueue.push_front(leftQueue.back());
            leftQueue.pop_back();
        } else if (leftQueue.size() < rightQueue.size()) {
            leftQueue.push_back(rightQueue.front());
            rightQueue.pop_front();
        }
    }

public:
    FrontMiddleBackQueue() {}

    void pushFront(int val) {
        leftQueue.push_front(val);
        balance();
    }

    void pushMiddle(int val) {
        if (leftQueue.size() < rightQueue.size()) {
            leftQueue.push_back(val);
        } else {
            rightQueue.push_front(val);
        }
        balance();
    }

    void pushBack(int val) {
        rightQueue.push_back(val);
        balance();
    }

    int popFront() {
        if (leftQueue.empty() && rightQueue.empty()) {
            return -1;
        }
        int val;
        if (!leftQueue.empty() && leftQueue.size() == rightQueue.size()) {
            val = leftQueue.front();
            leftQueue.pop_front();
        } else if (!leftQueue.empty()) {
            val = leftQueue.front();
            leftQueue.pop_front();
        } else {
            val = rightQueue.front();
            rightQueue.pop_front();
        }
        balance();
        return val;
    }

    int popMiddle() {
        if (leftQueue.empty() && rightQueue.empty()) {
            return -1;
        }
        int val;
        if (leftQueue.size() == rightQueue.size()) {
            val = leftQueue.back();
            leftQueue.pop_back();
        } else {
            val = rightQueue.front();
            rightQueue.pop_front();
        }
        balance();
        return val;
    }

    int popBack() {
        if (leftQueue.empty() && rightQueue.empty()) {
            return -1;
        }
        int val;
        if (!rightQueue.empty()) {
            val = rightQueue.back();
            rightQueue.pop_back();
        } else {
            val = leftQueue.back();
            leftQueue.pop_back();
        }
        balance();
        return val;
    }

    // Helper method to display current contents without emptying the queue
    void display() const {
        if (leftQueue.empty() && rightQueue.empty()) {
            cout << "Queue contents: []\n";
            return;
        }
        cout << "Queue contents: [ ";
        for (int val : leftQueue) {
            cout << val << " ";
        }
        for (int val : rightQueue) {
            cout << val << " ";
        }
        cout << "]\n";
    }
};

int main() {
    FrontMiddleBackQueue q;
    int choice = 0;
    int val = 0;

    cout << "=== Interactive FrontMiddleBackQueue Engine ===\n";

    while (true) {
        cout << "\n-----------------------------\n";
        cout << "1. Push Front\n";
        cout << "2. Push Middle\n";
        cout << "3. Push Back\n";
        cout << "4. Pop Front\n";
        cout << "5. Pop Middle\n";
        cout << "6. Pop Back\n";
        cout << "7. Display Queue State\n";
        cout << "8. Exit\n";
        cout << "Enter your choice (1-8): ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Exiting program.\n";
            break;
        }

        switch (choice) {
            case 1:
                cout << "Enter value to push front: ";
                cin >> val;
                q.pushFront(val);
                cout << "Pushed " << val << " to front.\n";
                break;

            case 2:
                cout << "Enter value to push middle: ";
                cin >> val;
                q.pushMiddle(val);
                cout << "Pushed " << val << " to middle.\n";
                break;

            case 3:
                cout << "Enter value to push back: ";
                cin >> val;
                q.pushBack(val);
                cout << "Pushed " << val << " to back.\n";
                break;

            case 4:
                val = q.popFront();
                if (val == -1) cout << "Queue is empty! (Returned -1)\n";
                else cout << "Popped from front: " << val << "\n";
                break;

            case 5:
                val = q.popMiddle();
                if (val == -1) cout << "Queue is empty! (Returned -1)\n";
                else cout << "Popped from middle: " << val << "\n";
                break;

            case 6:
                val = q.popBack();
                if (val == -1) cout << "Queue is empty! (Returned -1)\n";
                else cout << "Popped from back: " << val << "\n";
                break;

            case 7:
                q.display();
                break;

            case 8:
                cout << "Exiting system. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice! Please select an option between 1 and 8.\n";
                break;
        }
    }

    return 0;
}