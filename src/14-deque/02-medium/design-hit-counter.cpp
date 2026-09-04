#include <iostream>
#include <vector>

using namespace std;

class HitCounter {
private:
    static const int TIME_WINDOW = 300; // 5-minute rolling window (300 seconds)
    vector<int> times;
    vector<int> hits;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DESIGN HIT COUNTER (TIME-STAMPED BUCKETS IN CYCLIC BUFFER)
     * -------------------------------------------------------------------------------------
     * The objective is to record hits and return the total number of hits received 
     * in the past 5 minutes (300 seconds) at any given timestamp.
     * 
     * 1. FIXED CYCLIC BUCKET ARRAY (O(1) MEMORY BOUND):
     *    Instead of storing every timestamp explicitly in a queue (which scales poorly 
     *    with high concurrent traffic), we use a fixed cyclic buffer of size 300.
     * 
     * 2. BUCKET INDEXING & TIME SYNCHRONIZATION:
     *    - Index mapping: `idx = timestamp % 300`
     *    - On `hit(timestamp)`:
     *        - If `times[idx] != timestamp`, the bucket contains old data from a previous cycle. 
     *          We update `times[idx] = timestamp` and reset `hits[idx] = 1`.
     *        - If `times[idx] == timestamp`, we simply increment `hits[idx]++`.
     * 
     * 3. O(1) ROLLING WINDOW AGGREGATION:
     *    - On `getHits(timestamp)`:
     *        - We sum up `hits[i]` across all 300 buckets where `timestamp - times[i] < 300`.
     */
    HitCounter() : times(TIME_WINDOW, 0), hits(TIME_WINDOW, 0) {}

    void hit(int timestamp) {
        int idx = timestamp % TIME_WINDOW;
        if (times[static_cast<size_t>(idx)] != timestamp) {
            times[static_cast<size_t>(idx)] = timestamp;
            hits[static_cast<size_t>(idx)] = 1;
        } else {
            hits[static_cast<size_t>(idx)]++;
        }
    }

    int getHits(int timestamp) const {
        int totalHits = 0;
        for (int i = 0; i < TIME_WINDOW; ++i) {
            if (timestamp - times[static_cast<size_t>(i)] < TIME_WINDOW) {
                totalHits += hits[static_cast<size_t>(i)];
            }
        }
        return totalHits;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: 
 *   - hit(timestamp): O(1)
 *   - getHits(timestamp): O(1) - Iterates over fixed 300 buckets.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Fixed vector space of 300 buckets regardless of traffic volume.
 */
int main() {
    cout << "=== Design Hit Counter Cyclic Bucket Engine ===\n";
    HitCounter counter;

    cout << "Enter total number of dynamic operations to perform: ";
    int numOps;
    if (!(cin >> numOps) || numOps <= 0) return 1;

    cout << "\nOperations format:\n";
    cout << "  1 [timestamp] : Record a hit\n";
    cout << "  2 [timestamp] : Get hits in last 300 seconds\n\n";

    for (int i = 0; i < numOps; ++i) {
        cout << "Operation [" << i + 1 << "/" << numOps << "]: ";
        int op;
        cin >> op;

        if (op == 1) {
            int timestamp;
            cin >> timestamp;
            counter.hit(timestamp);
            cout << "  -> Recorded hit at timestamp: " << timestamp << "\n";
        } else if (op == 2) {
            int timestamp;
            cin >> timestamp;
            int total = counter.getHits(timestamp);
            cout << "  -> Hits in 300s window at timestamp " << timestamp << ": " << total << "\n";
        } else {
            cout << "  -> Invalid operation option.\n";
        }
    }

    return 0;
}