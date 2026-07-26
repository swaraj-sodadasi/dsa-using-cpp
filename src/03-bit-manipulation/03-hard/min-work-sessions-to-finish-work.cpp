#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BITMASK DP SUBSETS):
- The problem asks us to find the minimum number of work sessions needed to complete all tasks, 
  where each task takes a certain amount of time and a single work session cannot exceed `sessionTime`.
- **State Representation via Bitmasks**:
  - Since the number of tasks $N$ is small (typically $\le 14$ or $15$), we can represent the subset 
    of completed tasks using an $N$-bit integer mask. 
  - A bit value of 1 at the $i$-th position indicates that `tasks[i]` has been completed, while 0 means it is outstanding.
- **Dynamic Programming Array Mapping**:
  - We create an array `dp` of size $2^N$. For any given configuration `mask`:
    - `dp[mask]` stores a `pair<int, int>` tracking:
      1. `.first`: The minimum number of complete work sessions used so far.
      2. `.second`: The minimum time consumed within the *current ongoing* work session.
- **State Transition Strategy**:
  - We start at the baseline state `dp[0] = {1, 0}` (1 session active, 0 time used).
  - For each mask from 0 up to $2^N - 1$, if the state is reachable, we attempt to add any uncompleted task `i`.
  - When adding `tasks[i]` to the current state:
    - If `current_session_time + tasks[i] <= sessionTime`, the task fits into the active session. 
      The state updates to `{sessions, current_session_time + tasks[i]}`.
    - If it exceeds the limit, we are forced to open a new work session. 
      The state updates to `{sessions + 1, tasks[i]}`.
  - We greedily minimize the pair values across all valid paths using standard lexicographical comparison.

TIME COMPLEXITY: O(N * 2^N) - We iterate through all $2^N$ state mask variations, trying up to $N$ structural transitions at each step.
SPACE COMPLEXITY: O(2^N) auxiliary - The size of the DP array required to store the optimal pairs for each subset combination.
*/

pair<int, int> minSessions(const vector<int>& tasks, int sessionTime) {
    size_t n = tasks.size();
    size_t totalStates = 1ULL << n;
    
    // dp[mask] = {min_sessions_needed, time_used_in_current_session}
    // Initialize with a high sentinel value representing infinity
    vector<pair<int, int>> dp(totalStates, {1e9, 1e9});
    
    // Base case: 0 tasks completed requires 1 session with 0 time consumed
    dp[0] = {1, 0};
    
    // Iterate through all subset combinations sequentially
    for (size_t mask = 0; mask < totalStates; ++mask) {
        if (dp[mask].first == 1e9) continue; // Skip unreachable states
        
        int currentSessions = dp[mask].first;
        int currentTimeUsed = dp[mask].second;
        
        // Attempt to complete an outstanding task
        for (size_t i = 0; i < n; ++i) {
            // Check if the i-th task has not been added to this mask yet
            if ((mask & (1ULL << i)) == 0) {
                size_t nextMask = mask | (1ULL << i);
                pair<int, int> nextState;
                
                if (currentTimeUsed + tasks[i] <= sessionTime) {
                    // The task fits within the current session limits
                    nextState = {currentSessions, currentTimeUsed + tasks[i]};
                } else {
                    // The task requires opening a fresh work session
                    nextState = {currentSessions + 1, tasks[i]};
                }
                
                // Keep the path that minimizes sessions first, and current session time second
                if (nextState < dp[nextMask]) {
                    dp[nextMask] = nextState;
                }
            }
        }
    }
    
    return dp[totalStates - 1];
}

int main() {
    int n, sessionTime;
    cout << "Enter the total number of tasks: ";
    if (!(cin >> n) || n <= 0 || n > 15) {
        cout << "Invalid input. The number of tasks must be between 1 and 15." << endl;
        return 1;
    }
    
    vector<int> tasks(static_cast<size_t>(n));
    cout << "Enter the time duration for each task separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> tasks[static_cast<size_t>(i)];
    }
    
    cout << "Enter the maximum time limit per session: ";
    if (!(cin >> sessionTime) || sessionTime <= 0) {
        cout << "Invalid session time constraint." << endl;
        return 1;
    }
    
    // Simple validation check to ensure no individual task exceeds the maximum session capacity
    for (int t : tasks) {
        if (t > sessionTime) {
            cout << "\nError: Task duration " << t << " exceeds the allowed session capacity of " << sessionTime << "." << endl;
            return 1;
        }
    }
    
    pair<int, int> result = minSessions(tasks, sessionTime);
    cout << "\nMinimum work sessions required: " << result.first << endl;
    cout << "Time spent in the final open session: " << result.second << " units." << endl;
    
    return 0;
}