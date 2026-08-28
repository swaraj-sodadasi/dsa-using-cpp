#include <iostream>
#include <queue>
#include <string>

using namespace std;

class RecentCounter {
private:
    // Queue buffer storing strictly increasing, chronological ping timestamps
    queue<int> pingQueue;

public:
    RecentCounter() {}

    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: SLIDING WINDOW TIME-STAMP QUEUE BUFFER
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the number of recent requests (pings) that have 
     * occurred within the last 3000 milliseconds (inclusive of the range [t - 3000, t]).
     * 
     * 1. MONOTONIC TIME STREAM:
     *    Requests arrive chronologically with strictly increasing timestamp values 't'.
     *    This monotonicity allows us to treat a standard queue as a sliding window.
     * 
     * 2. CHRONOLOGICAL WINDOW SLIDING / LIQUIDATION:
     *    When a new request arrives at time 't':
     *      - We append 't' to the back of our queue.
     *      - The lower bound of our active sliding window is defined as `t - 3000`.
     *      - We continuously evict (pop) elements from the front of the queue that are 
     *        older than this boundary (i.e., any element strictly less than `t - 3000`).
     *      - Since timestamps are strictly increasing, any expired timestamp popped now 
     *        will also be out-of-bounds for all subsequent, larger values of 't'.
     * 
     * 3. WINDOW WIDTH EVALUATION:
     *    After removing all stale timestamps, the remaining elements in the queue are 
     *    guaranteed to fall within the range [t - 3000, t]. We return the size of the queue.
     */
    int ping(int t) {
        // Step 1: Enqueue the newly arrived timestamp
        pingQueue.push(t);

        // Step 2: Pop out expired timestamps outside the [t - 3000, t] sliding window
        int lowerBound = t - 3000;
        while (!pingQueue.empty() && pingQueue.front() < lowerBound) {
            pingQueue.pop();
        }

        // Step 3: Return the current size of the active window buffer
        return static_cast<int>(pingQueue.size());
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Amortized - Each ping operation invokes pushing an element 
 *                  and potentially popping outdated elements. Since each individual 
 *                  timestamp is pushed exactly once and popped at most once over its 
 *                  entire lifetime, the total operations scale linearly with the number 
 *                  of pings, translating to O(1) average time per operation.
 *
 * SPACE COMPLEXITY: O(W) Worst-case - Where W represents the maximum number of requests 
 *                   occurring within any 3000ms sliding window interval.
 */
int main() {
    cout << "=== Sliding Window Recent Calls Counter Console ===\n";
    RecentCounter counter;

    cout << "Commands: 'ping [timestamp]', 'exit'\n\n";
    string operationalCommand;

    while (cout << "> ", cin >> operationalCommand) {
        if (operationalCommand == "ping") {
            int timestamp;
            if (cin >> timestamp) {
                int activePingsCount = counter.ping(timestamp);
                cout << "Timestamp: [" << timestamp << "] -> Active Window Pings Count: [" << activePingsCount << "]\n";
            }
        } else if (operationalCommand == "exit") {
            break;
        } else {
            cout << "Unknown command configuration. Retry.\n";
            cin.clear();
            string clearBuffer;
            getline(cin, clearBuffer);
        }
    }

    return 0;
}