#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: NUMBER OF STUDENTS UNABLE TO EAT LUNCH
     * -------------------------------------------------------------------------------------
     * The objective is to find the number of students who cannot eat lunch.
     * `students`: Queue of student preferences (0 = circular, 1 = square).
     * `sandwiches`: Stack of sandwiches available (top of stack is at index 0).
     * 
     * 1. COUNT-BASED SIMULATION OPTIMIZATION:
     *    Simulating explicit circular rotations with a `std::deque` can lead to unnecessary 
     *    O(N^2) or unbounded rotations.
     *    Notice that the order of students in the queue does NOT restrict which student eventually 
     *    reaches the front, EXCEPT when NO remaining student prefers the sandwich at the top 
     *    of the stack.
     * 
     * 2. O(N) COUNT MATCHER:
     *    - Count total students preferring circular (0) and square (1) sandwiches.
     *    - Iterate through the sandwiches array from top to bottom (index 0 to N-1).
     *    - If the current sandwich is 0 and count[0] > 0, decrement count[0].
     *    - If the current sandwich is 1 and count[1] > 0, decrement count[1].
     *    - If the current sandwich cannot be matched (count[sandwich] == 0), no remaining student 
     *      will ever take this sandwich. The process immediately terminates.
     * 
     * 3. RESULT:
     *    The number of unable students is simply the remaining sum of unmatched preferences:
     *    `count[0] + count[1]`.
     */
    int countStudents(const vector<int>& students, const vector<int>& sandwiches) {
        int count[2] = {0, 0};

        for (int pref : students) {
            count[pref]++;
        }

        for (int sandwich : sandwiches) {
            if (count[sandwich] > 0) {
                count[sandwich]--;
            } else {
                // No student left prefers this top sandwich; process stops immediately
                break;
            }
        }

        return count[0] + count[1];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Single linear pass over students array to count preferences 
 *                  and single pass over sandwiches.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - Fixed frequency array count[2].
 */
int main() {
    cout << "=== Number of Students Unable to Eat Lunch Engine ===\n";
    cout << "Enter the number of students/sandwiches: ";
    int n;
    if (!(cin >> n) || n <= 0) return 1;

    vector<int> students(static_cast<size_t>(n));
    cout << "Enter student preferences (0 = Circular, 1 = Square):\n";
    for (int i = 0; i < n; ++i) {
        cin >> students[static_cast<size_t>(i)];
    }

    vector<int> sandwiches(static_cast<size_t>(n));
    cout << "Enter sandwich stack order from top to bottom (0 = Circular, 1 = Square):\n";
    for (int i = 0; i < n; ++i) {
        cin >> sandwiches[static_cast<size_t>(i)];
    }

    Solution solver;
    int unableCount = solver.countStudents(students, sandwiches);

    cout << "Number of students unable to eat lunch: [" << unableCount << "]\n";

    return 0;
}