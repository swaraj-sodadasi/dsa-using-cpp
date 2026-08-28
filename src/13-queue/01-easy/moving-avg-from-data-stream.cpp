#include <iostream>
#include <queue>
#include <string>

using namespace std;

class MovingAverage {
private:
    queue<int> window;
    int capacity;
    double runningSum;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: FIXED-CAPACITY FIFO FLOATING POINT WINDOW ACCUMULATOR
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the moving average of a stream of integers within a 
     * sliding window of a specified maximum size.
     * 
     * 1. O(1) TIME COMPLEXITY ACCUMULATOR:
     *    To achieve constant time lookup, we maintain a running sum (`runningSum`) of all 
     *    elements currently in the sliding window. This avoids recalculating the sum of 
     *    the window elements during every insertion pass.
     * 
     * 2. EVICTION POLICY (FIFO):
     *    We push incoming elements to the back of a standard FIFO queue. If the queue 
     *    size exceeds the fixed capacity boundary:
     *      - We subtract the front element of the queue from our running sum.
     *      - We pop (evict) that front element to slide the window forward.
     * 
     * 3. RATIO COMPILATION:
     *    The moving average is calculated by dividing the updated running sum by the 
     *    current active size of the queue.
     */
    MovingAverage(int size) {
        capacity = size;
        runningSum = 0.0;
    }

    double next(int val) {
        window.push(val);
        runningSum += val;

        // Evict the oldest element if the window exceeds its maximum capacity limits
        if (static_cast<int>(window.size()) > capacity) {
            runningSum -= window.front();
            window.pop();
        }

        return runningSum / window.size();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) per next() call - Both queue insertion/eviction and running sum 
 *                  modifications are executed in constant time.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Where K represents the fixed capacity size limit 
 *                   allocated to store sliding window elements.
 */
int main() {
    cout << "=== Moving Average from Data Stream Engine ===\n";
    cout << "Enter the maximum window capacity: ";
    int capacity;
    if (!(cin >> capacity) || capacity <= 0) {
        cout << "Error: Invalid window capacity value.\n";
        return 1;
    }

    MovingAverage ma(capacity);
    cout << "Sliding window initialized with capacity " << capacity << ".\n";
    cout << "Commands: 'next [integer]', 'exit'\n\n";

    string command;
    while (cout << "> ", cin >> command) {
        if (command == "next") {
            int val;
            if (cin >> val) {
                double avg = ma.next(val);
                cout.precision(5);
                cout << "Inserted: [" << val << "] -> Active Moving Average: [" << fixed << avg << "]\n";
            }
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