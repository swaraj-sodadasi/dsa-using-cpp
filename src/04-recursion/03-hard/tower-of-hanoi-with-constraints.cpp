#include <iostream>

using namespace std;

class Solution {
private:
    long long totalMovesCount;

    /*
     * RECURSIVE CONSTRAINED STATE TRANSFER ENGINE
     * -------------------------------------------
     * This core framework simulates a highly constrained variant of the classic Tower of Hanoi.
     * In this variation, direct transfers between specific pegs are blocked (e.g., a standard 
     * constraint is that no disk can be moved directly between Peg A and Peg C; every transfer 
     * must pass through the middle peg, Peg B).
     *
     * To transfer 'n' disks from a source anchor to a destination anchor under this rule,
     * the problem can no longer be solved in a single structural operation. Instead, it must
     * be broken down into a multi-step sequence of sub-problems:
     *
     * State Parameters:
     *  - n: The number of active disks currently being shifted.
     *  - sourcePeg: The rod identifying the active home base for this frame.
     *  - middlePeg: The required transit intermediary rod.
     *  - destPeg: The target landing destination rod.
     *
     * Operational Sequence Breakdown:
     *  1. Move the top (n-1) disks from 'sourcePeg' to 'destPeg' using 'middlePeg' as transit.
     *  2. Move the remaining largest disk from 'sourcePeg' to 'middlePeg'.
     *  3. Move the top (n-1) disks from 'destPeg' back to 'sourcePeg' using 'middlePeg' as transit.
     *  4. Move the largest disk from 'middlePeg' to its final destination 'destPeg'.
     *  5. Move the top (n-1) disks from 'sourcePeg' over to 'destPeg' using 'middlePeg' as transit.
     *
     * This sequence increases the recurrence formula complexity from 2^n - 1 steps to 3^n - 1 steps.
     */
    void transferState(int n, char sourcePeg, char middlePeg, char destPeg) {
        // BASE CASE: CONVERGENCE FILTER
        // If the number of disks drops to zero, the operation terminates instantly.
        if (n == 0) {
            return;
        }

        // STEP 1: Shift the top (n-1) stack out of the way to clear the largest disk
        transferState(n - 1, sourcePeg, middlePeg, destPeg);

        // STEP 2: Execute the first structural move of the largest disk to the middle peg
        cout << "  Move disk " << n << " from peg " << sourcePeg << " to peg " << middlePeg << "\n";
        totalMovesCount++;

        // STEP 3: Relocate the (n-1) stack back to the source peg to clear the destination peg
        transferState(n - 1, destPeg, middlePeg, sourcePeg);

        // STEP 4: Move the largest disk from the middle peg to its true destination
        cout << "  Move disk " << n << " from peg " << middlePeg << " to peg " << destPeg << "\n";
        totalMovesCount++;

        // STEP 5: Finalize by shifting the remaining (n-1) stack onto the top of the largest disk
        transferState(n - 1, sourcePeg, middlePeg, destPeg);
    }

public:
    long long solveConstrainedHanoi(int disksCount) {
        totalMovesCount = 0;
        if (disksCount <= 0) return 0;
        
        // Initiate the constrained state transfer frame from Peg A to Peg C via Peg B
        transferState(disksCount, 'A', 'B', 'C');
        
        return totalMovesCount;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(3^N) - Where N represents the total number of disks. Due to the transit
 *                  constraint blocking direct paths between Peg A and Peg C, the recurrence 
 *                  relation expands out to T(N) = 3 * T(N-1) + 2, which yields exactly 3^N - 1 steps.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The depth allocation scale is bound linearly by the 
 *                   maximum execution stack frame layers, peaking at exactly N active contexts.
 */
int main() {
    int disks;
    cout << "Enter the total number of disks (N) for the constrained Hanoi simulation: ";
    if (!(cin >> disks) || disks < 0) {
        cout << "Invalid sizing parameter configuration parameters." << endl;
        return 1;
    }

    // Protection check to keep execution logging safe from extreme screen buffer inundation
    if (disks > 10) {
        cout << "Warning: Sizes greater than 10 produce massive operational steps (> 59,048 moves).\n";
        cout << "To maintain system stability, display outputs are capped at N = 10." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting constrained recursive state transfer optimization sequence:\n\n";
    long long totalOperations = solver.solveConstrainedHanoi(disks);

    cout << "\nTotal execution moves completed: " << totalOperations << endl;

    return 0;
}