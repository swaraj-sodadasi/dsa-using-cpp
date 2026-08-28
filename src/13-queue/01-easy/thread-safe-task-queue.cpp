#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;

// Mutex to synchronize console output so thread logs and prompts do not garble
mutex printMutex;

template <typename T>
class ThreadSafeQueue {
private:
    queue<T> taskQueue;
    mutable mutex queueMutex;
    condition_variable condVar;
    bool isStopped;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MUTEX AND CONDITION VARIABLE BUFFERING (PRODUCER-CONSUMER)
     * -------------------------------------------------------------------------------------
     * This class implements a thread-safe FIFO task queue using synchronization primitives.
     * 
     * 1. THE MUTEX GUARD:
     *    `std::mutex` guarantees mutual exclusion, ensuring only one thread can modify or 
     *    read the underlying standard `std::queue` container at any given time.
     * 
     * 2. THE CONDITION VARIABLE:
     *    To prevent CPU-intensive busy-waiting, consumer threads use a condition variable 
     *    to block (sleep) until a new task is pushed or the queue is shutdown.
     * 
     * 3. THE STOP SIGNAL:
     *    Allows graceful termination of all blocking consumer threads during shutdown.
     */
    ThreadSafeQueue() : isStopped(false) {}

    ~ThreadSafeQueue() {
        shutdown();
    }

    // Pushes a task into the queue and notifies a waiting consumer thread
    void push(T task) {
        {
            lock_guard<mutex> lock(queueMutex);
            if (isStopped) return;
            taskQueue.push(std::move(task));
        }
        condVar.notify_one();
    }

    // Blocks until a task becomes available or the queue is shut down
    bool pop(T& task) {
        unique_lock<mutex> lock(queueMutex);
        condVar.wait(lock, [this]() {
            return !taskQueue.empty() || isStopped;
        });

        if (taskQueue.empty() && isStopped) {
            return false;
        }

        task = std::move(taskQueue.front());
        taskQueue.pop();
        return true;
    }

    // Signals all threads to stop waiting and prevents further enqueueing
    void shutdown() {
        {
            lock_guard<mutex> lock(queueMutex);
            isStopped = true;
        }
        condVar.notify_all();
    }

    bool empty() const {
        lock_guard<mutex> lock(queueMutex);
        return taskQueue.empty();
    }

    size_t size() const {
        lock_guard<mutex> lock(queueMutex);
        return taskQueue.size();
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) - Mutex locking, queue insertions/deletions, and condition 
 *                  variable notifications are constant-time operations.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Where N represents the maximum number of queued 
 *                   tasks stored in memory.
 */
int main() {
    cout << "=== Thread-Safe Task Queue Engine (Mutex / CondVar) ===\n";
    
    int numConsumers = 1;
    cout << "Enter the number of consumer (worker) threads to deploy: ";
    if (!(cin >> numConsumers) || numConsumers <= 0) {
        cout << "Invalid input. Defaulting to 2 consumers.\n";
        numConsumers = 2;
        cin.clear();
        string dummy;
        getline(cin, dummy);
    }

    ThreadSafeQueue<string> workQueue;

    // Worker (Consumer) thread function
    auto worker = [&workQueue](int id) {
        string task;
        while (workQueue.pop(task)) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << "\n[Consumer #" << id << "] Started task: " << task << "\n> " << flush;
            }
            
            // Simulate variable workload processing
            this_thread::sleep_for(chrono::milliseconds(800)); 

            {
                lock_guard<mutex> lock(printMutex);
                cout << "\n[Consumer #" << id << "] Completed task: " << task << "\n> " << flush;
            }
        }
        {
            lock_guard<mutex> lock(printMutex);
            cout << "\n[Consumer #" << id << "] Exiting gracefully.\n> " << flush;
        }
    };

    // Spawn consumer threads
    vector<thread> consumers;
    consumers.reserve(static_cast<size_t>(numConsumers));
    for (int i = 1; i <= numConsumers; ++i) {
        consumers.emplace_back(worker, i);
    }

    cin.ignore(); // Clear remaining characters in the input stream
    cout << "\n" << numConsumers << " workers are active and waiting for tasks.\n";
    cout << "Commands:\n";
    cout << "  push [task_name] - Enqueue a task (e.g., push TaskAlpha)\n";
    cout << "  status           - Display current queue size\n";
    cout << "  exit             - Stop queue and shutdown all worker threads\n\n";

    string inputLine;
    while (true) {
        {
            lock_guard<mutex> lock(printMutex);
            cout << "> " << flush;
        }
        if (!getline(cin, inputLine)) break;

        stringstream ss(inputLine);
        string command;
        ss >> command;

        if (command == "push") {
            string taskName;
            // Get the rest of the line as the task name
            getline(ss, taskName);
            // Trim leading spaces from the task name
            size_t firstNonSpace = taskName.find_first_not_of(" ");
            if (firstNonSpace != string::npos) {
                taskName = taskName.substr(firstNonSpace);
            }

            if (taskName.empty()) {
                lock_guard<mutex> lock(printMutex);
                cout << "Error: Task name cannot be empty.\n";
            } else {
                workQueue.push(taskName);
                lock_guard<mutex> lock(printMutex);
                cout << "Enqueued: \"" << taskName << "\"\n";
            }
        } else if (command == "status") {
            lock_guard<mutex> lock(printMutex);
            cout << "Queue Size: " << workQueue.size() << " | Active Workers: " << numConsumers << "\n";
        } else if (command == "exit") {
            break;
        } else if (!command.empty()) {
            lock_guard<mutex> lock(printMutex);
            cout << "Unknown command: \"" << command << "\"\n";
        }
    }

    // Initiate shutdown
    {
        lock_guard<mutex> lock(printMutex);
        cout << "\n[Producer] Initiating queue shutdown. Waiting for active workers to drain...\n";
    }
    workQueue.shutdown();

    for (auto& th : consumers) {
        if (th.joinable()) {
            th.join();
        }
    }

    cout << "[System] All threads joined. Safe exit completed.\n";
    return 0;
}