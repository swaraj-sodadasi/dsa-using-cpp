#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: COORDINATE COMMAND ACCUMULATION
     * -------------------------------------------------------------------------------------
     * The objective is to determine if a robot returns to its starting location (0, 0) 
     * after executing a sequence of movement commands given as a string.
     * 
     * 1. 2D CARTESIAN SPACE MAPPING:
     *    Instead of allocating spatial matrix layouts or recording the robot's complete 
     *    historical path, we map movements onto a 2D coordinate system using two scalar variables:
     *      - `x`: Tracks horizontal alignment (Left/Right balance).
     *      - `y`: Tracks vertical alignment (Up/Down balance).
     * 
     * 2. DIRECTIONAL NET STATE ACCUMULATION:
     *    We iterate through the character string array and update the coordinate balances:
     *      - 'U' (Up): Increments vertical placement (`y++`).
     *      - 'D' (Down): Decrements vertical placement (`y--`).
     *      - 'R' (Right): Increments horizontal placement (`x++`).
     *      - 'L' (Left): Decrements horizontal placement (`x--`).
     * 
     * 3. THE ZERO-BOUND RETURN CRITERION:
     *    The robot returns to its origin if and only if the net structural displacement values 
     *    cancel out perfectly. This means both `x == 0` and `y == 0` must hold true at the 
     *    end of the traversal.
     */
    bool judgeCircle(string moves) {
        int x = 0;
        int y = 0;

        // Step 2: Linear sweep to accumulate coordinate displacements
        for (char move : moves) {
            switch (move) {
                case 'U': y++; break;
                case 'D': y--; break;
                case 'R': x++; break;
                case 'L': x--; break;
                default: 
                    // Ignore unrecognized movement commands safely
                    break;
            }
        }

        // Step 3: Origin check validation
        return (x == 0 && y == 0);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total length of the moves command string. 
 *                  The single pass loop evaluates each character exactly once using a constant-time 
 *                  conditional switch block.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The system processes the trajectory entirely in place, using 
 *                   only two primitive integer registers to track position coordinates.
 */
int main() {
    string moveSequence;
    cout << "=== Robot Return to Origin Movement Evaluator Console ===\n";
    cout << "Enter the movement sequence commands string (using characters U, D, R, L):\n";
    if (!(cin >> moveSequence)) return 1;

    // Validate command formatting early
    for (char c : moveSequence) {
        if (c != 'U' && c != 'D' && c != 'R' && c != 'L') {
            cout << "Warning: Character entry '" << c << "' is an invalid command signature.\n";
        }
    }

    Solution solver;
    cout << "\nAccumulating 2D vector coordinate updates across path sweeps..." << endl;
    bool backAtOrigin = solver.judgeCircle(moveSequence);

    if (backAtOrigin) {
        cout << "Result: The robot successfully returned to its origin position! (true)\n";
    } else {
        cout << "Result: The robot ended its journey away from the origin position. (false)\n";
    }

    return 0;
}