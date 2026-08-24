#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DIRECTION-BASED CONDITIONAL COLLISION STACK SIMULATION
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the final state of asteroids moving along a line.
     * 
     * 1. COLLISION CRITERIA CONDITIONS:
     *    Asteroids move at equal speeds. Positive values move right (+), negative move left (-).
     *    Collisions happen *only* when a right-moving asteroid is ahead of a left-moving one 
     *    (i.e., top of stack is positive and current asteroid is negative). 
     *    Parallel or diverging trajectories (e.g., left-moving followed by right-moving) never collide.
     * 
     * 2. THE SIMULATION AND ELIMINATION FLOW:
     *    We use a vector as a dynamic stack buffer. For each incoming asteroid:
     *      - We assume it survives initially (`isDestroyed = false`).
     *      - While the stack has items, the top asteroid moves right (> 0), and the incoming moves left (< 0):
     *        - If the top asteroid is smaller than the absolute mass of the incoming one, the top is destroyed.
     *          We pop it and continue checking elements deeper in the stack.
     *        - If the top asteroid equals the incoming mass, both smash into pieces. 
     *          We pop the top, flag the incoming as destroyed, and stop looking.
     *        - If the top asteroid is larger, the incoming one is destroyed on impact. 
     *          We flag it as destroyed and stop.
     *      - If the incoming asteroid survives all potential collisions, we push it onto the stack.
     */
    vector<int> asteroidCollision(const vector<int>& asteroids) {
        vector<int> stableAsteroidsStack;

        for (int incoming : asteroids) {
            bool isDestroyed = false;

            // Collision loop: active when top moves right and incoming moves left
            while (!stableAsteroidsStack.empty() && stableAsteroidsStack.back() > 0 && incoming < 0) {
                int topMass = stableAsteroidsStack.back();
                int incomingMass = abs(incoming);

                if (topMass < incomingMass) {
                    // Top asteroid is crushed; pop it and evaluate against the next stack element
                    stableAsteroidsStack.pop_back();
                    continue;
                } else if (topMass == incomingMass) {
                    // Mutual destruction; pop the top element and mark the incoming as destroyed
                    stableAsteroidsStack.pop_back();
                    isDestroyed = true;
                    break;
                } else {
                    // Incoming asteroid is obliterated; mark it as destroyed and terminate check
                    isDestroyed = true;
                    break;
                }
            }

            // Push onto the stable tracking frame if the asteroid survives the impact phase
            if (!isDestroyed) {
                stableAsteroidsStack.push_back(incoming);
            }
        }

        return stableAsteroidsStack;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total elements inside the asteroids vector.
 *                  Each item is pushed onto the stack at most once and popped at most once.
 *
 * SPACE COMPLEXITY: O(N) Worst-case / O(1) Auxiliary - The stack contains at most N data entries 
 *                   if all elements move in stable, non-colliding layouts (e.g., [5, 10, 15] or [-2, -1, 5]).
 */
int main() {
    cout << "=== Asteroid Collision Conditional Stack Simulation Console ===\n";
    cout << "Enter the total number of asteroids: ";
    int count;
    if (!(cin >> count) || count < 0) return 1;

    vector<int> asteroidsList(static_cast<size_t>(count));
    if (count > 0) {
        cout << "Enter the integer mass values sequentially (positive=right, negative=left):\n";
        for (int i = 0; i < count; ++i) {
            cin >> asteroidsList[static_cast<size_t>(i)];
        }
    }

    Solution solver;
    cout << "\nSimulating kinematic flight trajectories and checking collision weights..." << endl;
    vector<int> finalState = solver.asteroidCollision(asteroidsList);

    cout << "Remaining Asteroids Orbit Space Matrix: [ ";
    for (int val : finalState) cout << val << " ";
    cout << "]\n";

    return 0;
}