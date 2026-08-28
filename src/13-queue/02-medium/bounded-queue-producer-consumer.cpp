#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

// Mutex to synchronize output stream access across print statements
mutex printMutex;

/*
 * -------------------------------------------------------------------------------------
 * CORE LOGIC: SEMAPHORE-CONTROLLED BOUNDED QUEUE ARCHITECTURE
 * -------------------------------------------------------------------------------------
 * This class implements a thread-safe Bounded Queue (Producer-Consumer) using classical
 * synchronization patterns to mimic Counting Semaphores using Mutex and Condition Variables.
 * 
 * 1. THE DOUBLE-SEMAPHORE MODEL:
 *    We simulate two counting semaphores:
 *      - `emptySlots`: Tracks remaining buffer capacity. Producers wait on this semaphore.
 *      - `fullSlots`: Tracks items ready for consumption. Consumers wait on this semaphore.
 * 
 * 2. STRUCTURAL BOUNDS:
 *    - Push blocks when `emptySlots == 0` (Queue is completely full).
 *    - Pop blocks when `fullSlots == 0` (Queue is completely empty).
 * 
 * 3. EXPLICIT STOP SIGNAL:
 *    A flag `isStopped` allows thread pools to drain and exit gracefully.
 */
template <typename T>
class BoundedQueue {
private:
    queue<T> buffer;
    size_t capacity;
    bool isStopped;

    mutable mutex queueMutex;
    condition_variable cvNotEmpty; // Signals when fullSlots > 0
    condition_variable cvNotFull;  // Signals when emptySlots > 0

public:
    explicit BoundedQueue(size_t maxCapacity) : capacity(maxCapacity), isStopped(false) {}

    ~BoundedQueue() {
        shutdown();
    }

    // Producer Interface: Push elements into the queue. Blocks if full.
    bool push(T item) {
        unique_lock<mutex> lock(queueMutex);
        
        // Wait until emptySlots > 0 (buffer.size() < capacity) or stopped
        cvNotFull.wait(lock, [this]() {
            return buffer.size() < capacity || isStopped;
        });

        if (isStopped) {
            return false;
        }

        buffer.push(std::move(item));
        
        // Notify waiting consumer threads (increment fullSlots)
        cvNotEmpty.notify_one();
        return true;
    }

    // Consumer Interface: Pop elements from the queue. Blocks if empty.
    bool pop(T& item) {
        unique_lock<mutex> lock(queueMutex);

        // Wait until fullSlots > 0 (buffer.size() > 0) or stopped
        cvNotEmpty.wait(lock, [this]() {
            return !buffer.empty() || isStopped;
        });

        if (buffer.empty() && isStopped) {
            return false;
        }

        item = std::move(buffer.front());
        buffer.pop();

        // Notify waiting producer threads (increment emptySlots)
        cvNotFull.notify_one();
        return true;
    }

    void shutdown() {
        {
            lock_guard<mutex> lock(queueMutex);
            isStopped = true;
        }
        cvNotFull.notify_all();
        cvNotEmpty.notify_all();
    }

    size_t size() const {
        lock_guard<mutex> lock(queueMutex);
        return buffer.size();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) - Mutex locking, queue insertions/deletions, and condition 
 *                  variable notifications are constant-time operations.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Where K represents the maximum queue capacity 
 *                   limit allocated to store queued elements.
 */
int main() {
    cout << "=== Semaphore-Controlled Bounded Queue Engine ===\n";
    
    int capacity = 3;
    cout << "Enter the maximum buffer capacity (e.g., 3): ";
    if (!(cin >> capacity) || capacity <= 0) {
        cout << "Invalid capacity. Defaulting to 3.\n";
        capacity = 3;
        cin.clear();
        string dummy;
        getline(cin, dummy);
    }

    int numProducers = 2;
    cout << "Enter the number of Producer threads: ";
    if (!(cin >> numProducers) || numProducers <= 0) {
        numProducers = 2;
    }

    int numConsumers = 2;
    cout << "Enter the number of Consumer threads: ";
    if (!(cin >> numConsumers) || numConsumers <= 0) {
        numConsumers = 2;
    }

    BoundedQueue<string> bq(static_cast<size_t>(capacity));

    // Producer Thread Function
    auto producerFunc = [&bq](int id) {
        for (int i = 1; i <= 5; ++i) {
            string task = "DataBlock-P" + to_string(id) + "-#" + to_string(i);
            
            {
                lock_guard<mutex> lock(printMutex);
                cout << "[Producer #" << id << "] Attempting to push: " << task << " (Buffer Size: " << bq.size() << ")\n" << flush;
            }

            if (bq.push(task)) {
                lock_guard<mutex> lock(printMutex);
                cout << "[Producer #" << id << "] Successfully pushed: " << task << "\n" << flush;
            } else {
                break;
            }
            // Simulate variable generation cycles
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    };

    // Consumer Thread Function
    auto consumerFunc = [&bq](int id) {
        string task;
        while (bq.pop(task)) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << "   [Consumer #" << id << "] Started processing: " << task << "\n" << flush;
            }
            
            // Simulate processing times
            this_thread::sleep_for(chrono::milliseconds(500));

            {
                lock_guard<mutex> lock(printMutex);
                cout << "   [Consumer #" << id << "] Finished processing: " << task << "\n" << flush;
            }
        }
        {
            lock_guard<mutex> lock(printMutex);
            cout << "   [Consumer #" << id << "] Exiting gracefully.\n" << flush;
        }
    };

    // Spawn execution threads
    vector<thread> producers;
    producers.reserve(static_cast<size_t>(numProducers));
    for (int i = 1; i <= numProducers; ++i) {
        producers.emplace_back(producerFunc, i);
    }

    vector<thread> consumers;
    consumers.reserve(static_cast<size_t>(numConsumers));
    for (int i = 1; i <= numConsumers; ++i) {
        consumers.emplace_back(consumerFunc, i);
    }

    // Join producers first to ensure all products are successfully sent
    for (auto& t : producers) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Wait a brief period to let consumers finish processing remaining buffer items
    this_thread::sleep_for(chrono::seconds(2));

    {
        lock_guard<mutex> lock(printMutex);
        cout << "\n[System] All producers complete. Initiating graceful shutdown...\n" << flush;
    }
    bq.shutdown();

    for (auto& t : consumers) {
        if (t.joinable()) {
            t.join();
        }
    }

    cout << "[System] Bounded processing pipeline complete.\n";
    return 0;
}