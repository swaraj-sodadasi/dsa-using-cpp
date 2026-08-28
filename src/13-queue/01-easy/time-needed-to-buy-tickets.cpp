#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: FIFO SIMULATION VIA CYCLIC QUEUE ROTATION
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the time required for the person at index 'k' to 
     * finish buying all their tickets.
     * 
     * 1. CYCLIC LIFO/FIFO SIMULATION MODEL:
     *    We maintain a queue containing pairs of {initial_index, remaining_tickets}.
     *    This accurately simulates the queue line dynamics:
     *      - Every second, we serve the person at the front (remaining tickets count decreases by 1).
     *      - If they still need tickets, they rotate to the back of the queue.
     *      - If their needs are met, they exit the queue.
     * 
     * 2. TERMINATION METRIC:
     *    The execution loops until the person originally at index `k` registers 
     *    a remaining ticket requirement of 0.
     */
    int timeRequiredToBuy(const vector<int>& tickets, int k) {
        // Queue storing pairs: {original_index, remaining_tickets_needed}
        queue<pair<int, int>> lineQueue;
        int n = static_cast<int>(tickets.size());

        for (int i = 0; i < n; ++i) {
            lineQueue.push({i, tickets[static_cast<size_t>(i)]});
        }

        int totalTimeElapsed = 0;

        while (!lineQueue.empty()) {
            auto currentPerson = lineQueue.front();
            lineQueue.pop();

            int originalIndex = currentPerson.first;
            int ticketsLeft = currentPerson.second;

            // Transact one ticket
            ticketsLeft--;
            totalTimeElapsed++;

            // If the target person at index 'k' is finished, terminate immediately
            if (originalIndex == k && ticketsLeft == 0) {
                break;
            }

            // If the person still needs more tickets, push them to the back of the line
            if (ticketsLeft > 0) {
                lineQueue.push({originalIndex, ticketsLeft});
            }
        }

        return totalTimeElapsed;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * M) - Where N represents the total number of people in line,
 *                  and M represents the maximum number of tickets required by any single person.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The queue stores at most N elements during the 
 *                   simulation cycle.
 */
int main() {
    cout << "=== Time Needed to Buy Tickets Cyclic Queue Emulator ===\n";
    cout << "Enter the total number of people in line: ";
    int length;
    if (!(cin >> length) || length <= 0) return 1;

    vector<int> tickets(static_cast<size_t>(length));
    cout << "Enter the integer ticket requirements sequentially:\n";
    for (int i = 0; i < length; ++i) {
        cin >> tickets[static_cast<size_t>(i)];
    }

    cout << "Enter the target index 'k' (0-indexed): ";
    int k;
    if (!(cin >> k) || k < 0 || k >= length) {
        cout << "Error: Target index 'k' is out of line bounds." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nSimulating ticket transaction cycles..." << endl;
    int calculatedTime = solver.timeRequiredToBuy(tickets, k);

    cout << "Total Time Needed for Person [" << k << "] to Finish: [" << calculatedTime << " seconds]\n";

    return 0;
}