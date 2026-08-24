#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DEPTH VECTOR LEVEL INTEGER ACCUMULATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine the minimum number of operations needed to return 
     * to the main folder after a series of file system navigation logs.
     * 
     * 1. THE INTEGER ACCUMULATION OVERHEAD AVOIDANCE:
     *    A naive file structure simulation pushes and pops strings onto a literal directory 
     *    stack frame, allocating heavy object allocations. 
     *    Instead, we recognize that folder trajectories can be tracked using a simple integer 
     *    `depthTracker` variable that increments and decrements based on the folder level depth.
     * 
     * 2. LOG TRANSITION MATRICES:
     *    We iterate through the operations vector and evaluate the token patterns:
     *      - "../" : Moves to the parent folder. If we are already in the main folder 
     *                (depthTracker == 0), the action is ignored. Otherwise, we decrement depthTracker.
     *      - "./"  : Remains in the current folder. The depth state is unchanged.
     *      - "x/"  : Moves to a new child folder. We increment depthTracker by 1.
     */
    int minOperations(const vector<string>& logs) {
        int depthTracker = 0;

        for (const string& logToken : logs) {
            if (logToken == "../") {
                if (depthTracker > 0) {
                    depthTracker--;
                }
            } else if (logToken == "./") {
                // Stay within current file structure path context; do nothing
                continue;
            } else {
                // Navigate into an isolated subfolder node chain segment
                depthTracker++;
            }
        }

        return depthTracker;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of entries in the logs array.
 *                  The system evaluates each path log token string exactly once in a linear sweep.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Relies entirely on a single stack-allocated tracking variable, 
 *                   consuming zero dynamic memory structures or extra buffer tracking vectors.
 */
int main() {
    cout << "=== Depth Vector Level Integer Accumulation File Engine ===\n";
    cout << "Enter the total log string operation count: ";
    int logsCount;
    if (!(cin >> logsCount) || logsCount < 0) return 1;

    vector<string> systemLogs(static_cast<size_t>(logsCount));
    if (logsCount > 0) {
        cout << "Enter the directory operational tokens sequentially (e.g., d1/ d2/ ../ ./):\n";
        for (int i = 0; i < logsCount; ++i) {
            cin >> systemLogs[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nAnalyzing log sequences and calculating dynamic folder levels..." << endl;
    int minSteps = solver.minOperations(systemLogs);

    cout << "Minimum Operation Steps Required to Return to Main Head: [" << minSteps << "]\n";

    return 0;
}