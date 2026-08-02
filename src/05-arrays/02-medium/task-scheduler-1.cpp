#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

class Solution {
public:
    /*
     * GREEDY FREQUENCY INTERLEAVING / IDLE SLOT COMPUTATION ENGINE
     * ------------------------------------------------------------
     * The objective is to find the minimum number of CPU intervals required to complete
     * all tasks, given a cooling period 'n' where identical tasks must be separated by 
     * at least 'n' intervals.
     *
     * Mathematical & Algorithmic Mechanics:
     * - The absolute bottleneck is the task with the highest frequency. Let its frequency be `maxFreq`.
     * - If we have a task with frequency `maxFreq`, it will create `maxFreq - 1` chunks of idle time. 
     *   Each chunk must have a width of at least `n` units.
     * - The base layout size created by this maximum task is: `baseLayout = (maxFreq - 1) * (n + 1)`.
     * - If multiple tasks share this same maximum frequency, each one will extend the final chunk's length.
     *   Let `maxFreqCount` be the number of tasks that have an identical frequency equal to `maxFreq`.
     * - This yields the theoretical minimum time frame: `calculatedTime = (maxFreq - 1) * (n + 1) + maxFreqCount`.
     *
     * Invariance Properties:
     * - If we have a massive pool of diverse tasks, the calculated time might be less than the total count 
     *   of tasks. However, the total execution time can never be shorter than the actual number of tasks. 
     * - Therefore, the optimal solution is the maximum of the total task count and our calculated layout:
     *   `Result = max(totalTasks, (maxFreq - 1) * (n + 1) + maxFreqCount)`.
     */
    int leastInterval(vector<char>& tasks, int n) {
        if (tasks.empty()) return 0;
        if (n == 0) return static_cast<int>(tasks.size());

        // Step 1: Count frequencies of each task type
        vector<int> frequencies(26, 0);
        for (char task : tasks) {
            frequencies[static_cast<size_t>(task - 'A')]++;
        }

        // Step 2: Find the maximum frequency value
        int maxFreq = 0;
        for (int freq : frequencies) {
            maxFreq = max(maxFreq, freq);
        }

        // Step 3: Count how many distinct task variants share this exact max frequency
        int maxFreqCount = 0;
        for (int freq : frequencies) {
            if (freq == maxFreq) {
                maxFreqCount++;
            }
        }

        // Step 4: Compute the minimum theoretical intervals using the greedy slot framework
        int totalTasksSize = static_cast<int>(tasks.size());
        int optimalIntervalsCount = (maxFreq - 1) * (n + 1) + maxFreqCount;

        // Return the larger value between the layout constraint and the raw number of elements
        return max(totalTasksSize, optimalIntervalsCount);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(T) - Where T represents the total number of tasks in the processing pool.
 *                  The logic populates the frequency counts in a single linear pass over the dataset. 
 *                  The subsequent lookup pass over the fixed alphabet map runs in constant O(1) time.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The frequency array uses a fixed spatial footprint of 26 
 *                   slots to record task metadata, utilizing no dynamic memory layout extensions.
 */
int main() {
    string inputLine;
    cout << "Enter task items as capital letters (e.g., A A A B B B) separated by spaces:\n";
    if (!getline(cin, inputLine)) {
        return 1;
    }

    stringstream ss(inputLine);
    char taskLabel;
    vector<char> tasks;
    while (ss >> taskLabel) {
        if (taskLabel >= 'A' && taskLabel <= 'Z') {
            tasks.push_back(taskLabel);
        }
    }

    if (tasks.empty()) {
        cout << "Invalid parameter. The task processing pool cannot be empty." << endl;
        return 1;
    }

    int coolingPeriod;
    cout << "Enter the cooling period constraint limit factor (n): ";
    if (!(cin >> coolingPeriod) || coolingPeriod < 0) {
        cout << "Invalid cooling period metric assignment." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting greedy frequency interleaving scheduling calculation..." << endl;
    int result = solver.leastInterval(tasks, coolingPeriod);

    cout << "Minimum CPU clock execution intervals required: " << result << endl;

    return 0;
}