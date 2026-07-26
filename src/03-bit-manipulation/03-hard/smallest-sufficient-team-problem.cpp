#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

/*
CORE LOGIC (BITMASK DYNAMIC PROGRAMMING / SET COVER PROBLEM):
- The problem asks us to find the smallest sufficient team containing a subset of people such that the union of their skills covers all required skills.
- **State Representation via Bitmasks**:
  - Let $M$ be the number of required skills (typically $M \le 16$). We can map each skill to a unique bit position from 0 to $M-1$.
  - Any subset of skills can be uniquely represented by an $M$-bit integer `skillMask`. A bit value of 1 means that skill is covered.
  - The goal is to reach the target mask `(1 << M) - 1` (where all bits are 1) using the minimum number of people.
- **Dynamic Programming Array Mapping**:
  - We create an array `dp` of size $2^M$ to store the indices of people forming the optimal team for each specific skill combination.
  - We initialize `dp[0]` as an empty vector (0 skills covered requires 0 people). All other states are initialized to hold a sentinel list of infinite size to signify they are unreachable.
- **State Transition Strategy**:
  - We convert each person's skill set into a bitmask (`personSkillMask`).
  - For each person, we iterate through all existing configurations in the `dp` array.
  - If a state `mask` is reachable, adding the current person generates a new state: `nextMask = mask | personSkillMask`.
  - If the size of the team at `dp[mask]` plus 1 is strictly less than the current size of `dp[nextMask]`, we update `dp[nextMask]` with this smaller, more efficient team.

TIME COMPLEXITY: O(N * 2^M) - Where N is the total number of people available and M is the total number of required skills. We perform a full subset transition evaluation pass for each person.
SPACE COMPLEXITY: O(2^M * N) - Storage allocated for the DP array tracking list configurations across all $2^M$ states.
*/

vector<int> smallestSufficientTeam(const vector<string>& req_skills, const vector<vector<string>>& people) {
    size_t numSkills = req_skills.size();
    size_t totalStates = 1ULL << numSkills;
    
    // Map skill names to unique bit positions
    unordered_map<string, int> skillToBit;
    for (int i = 0; i < static_cast<int>(numSkills); ++i) {
        skillToBit[req_skills[static_cast<size_t>(i)]] = i;
    }
    
    // Initialize DP table tracking teams for each skill configuration state
    // We use a large sentinel size to represent an unreached/infinite path
    vector<vector<int>> dp(totalStates, vector<int>(people.size() + 1)); 
    vector<bool> reached(totalStates, false);
    
    dp[0] = {}; // Base state: 0 skills requires an empty team
    reached[0] = true;
    
    // Process each person one by one
    for (int i = 0; i < static_cast<int>(people.size()); ++i) {
        int personSkillMask = 0;
        for (const string& skill : people[static_cast<size_t>(i)]) {
            if (skillToBit.count(skill)) {
                personSkillMask |= (1 << skillToBit[skill]);
            }
        }
        
        // Skip people who possess no relevant required skills
        if (personSkillMask == 0) continue;
        
        // Traverse existing states backwards to prevent using the same person multiple times in a single transition
        for (int mask = static_cast<int>(totalStates) - 1; mask >= 0; --mask) {
            if (!reached[static_cast<size_t>(mask)]) continue;
            
            int nextMask = mask | personSkillMask;
            // If the target state hasn't been reached, or the new team combination is smaller
            if (!reached[static_cast<size_t>(nextMask)] || dp[static_cast<size_t>(mask)].size() + 1 < dp[static_cast<size_t>(nextMask)].size()) {
                dp[static_cast<size_t>(nextMask)] = dp[static_cast<size_t>(mask)];
                dp[static_cast<size_t>(nextMask)].push_back(i);
                reached[static_cast<size_t>(nextMask)] = true;
            }
        }
    }
    
    return dp[totalStates - 1];
}

int main() {
    int m, n;
    cout << "Enter the total number of required skills: ";
    if (!(cin >> m) || m <= 0 || m > 16) {
        cout << "Invalid skills scale constraint configuration boundaries." << endl;
        return 1;
    }
    
    vector<string> req_skills(static_cast<size_t>(m));
    cout << "Enter the required skill names separated by spaces:\n";
    for (int i = 0; i < m; ++i) {
        cin >> req_skills[static_cast<size_t>(i)];
    }
    
    cout << "Enter the total number of people available: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid pool constraint tracking scale parameter boundaries." << endl;
        return 1;
    }
    
    vector<vector<string>> people(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        int k;
        cout << "Enter the number of skills for person " << i << ": ";
        cin >> k;
        vector<string> currentPersonSkills(static_cast<size_t>(k));
        if (k > 0) {
            cout << "Enter the skill names for person " << i << " separated by spaces: ";
            for (int j = 0; j < k; ++j) {
                cin >> currentPersonSkills[static_cast<size_t>(j)];
            }
        }
        people[static_cast<size_t>(i)] = currentPersonSkills;
    }
    
    vector<int> team = smallestSufficientTeam(req_skills, people);
    
    cout << "\nThe indices of the people chosen for the smallest sufficient team are:\n[ ";
    for (int index : team) {
        cout << index << " ";
    }
    cout << "]\n";
    cout << "Total team size: " << team.size() << " individuals." << endl;
    
    return 0;
}