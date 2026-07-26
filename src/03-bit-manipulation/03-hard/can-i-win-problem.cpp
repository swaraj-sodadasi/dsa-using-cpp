#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BITMASK MEMOIZATION / MINIMAX GAME THEORY DP):
- The problem asks us to determine if the first player can force a win in a game where players take turns choosing numbers from 1 to `maxChoosableInteger`. The first player whose accumulated choices sum to at least `desiredTotal` wins.
- **Game Representation via Bitmasks**:
  - Since `maxChoosableInteger` is small (typically $\le 20$), we can represent the pool of available/chosen integers using a 32-bit bitmask. 
  - The $i$-th bit of the mask is set to 1 if the number $i$ has already been chosen, and 0 if it is still available.
- **Minimax Game State Logic**:
  - A game state can be uniquely identified by the `stateMask` showing which numbers are gone. 
  - On any player's turn, if they can pick *any* available integer that either immediately matches or exceeds the remaining total, they win.
  - Alternatively, if they pick an available integer and the resulting state forces the *other* player into a guaranteed losing configuration, the current player wins.
  - If all available choices lead to a winning state for the opponent, the current player loses.
- **Edge Case / Early Boundary Filtering**:
  - If the sum of all choosable integers is less than `desiredTotal`, no player can ever reach the goal. The game ends in an effective draw/loss, so we return `false`.
  - If `desiredTotal` is 0, the first player wins immediately before making a move, so we return `true`.

TIME COMPLEXITY: O(2^N * N) - Where N is `maxChoosableInteger`. There are $2^N$ unique subset combinations tracked by the mask, and at each state, we check up to N choices.
SPACE COMPLEXITY: O(2^N) auxiliary - The size of the memoization table required to store the boolean outcome states of all possible subsets.
*/

class Solution {
private:
    // Memoization array tracking game states: 0 = unvisited, 1 = true (win), 2 = false (loss)
    vector<char> memo;

    bool canWinHelper(int maxChoosable, int currentTotal, int stateMask) {
        // If the current game state combination has been computed before, return the cached result
        if (memo[static_cast<size_t>(stateMask)] != 0) {
            return memo[static_cast<size_t>(stateMask)] == 1;
        }

        // Try selecting every available number from the remaining pool
        for (int i = 1; i <= maxChoosable; ++i) {
            int bitPosition = 1 << i;
            
            // Check if the number i has NOT been chosen yet
            if ((stateMask & bitPosition) == 0) {
                // If picking this number immediately reaches the desired total, it's a winning move
                if (currentTotal - i <= 0) {
                    memo[static_cast<size_t>(stateMask)] = 1;
                    return true;
                }
                
                // If picking this number leaves the opponent in a forced losing configuration
                if (!canWinHelper(maxChoosable, currentTotal - i, stateMask | bitPosition)) {
                    memo[static_cast<size_t>(stateMask)] = 1;
                    return true;
                }
            }
        }

        // If no choice guarantees a victory, this state is a loss
        memo[static_cast<size_t>(stateMask)] = 2;
        return false;
    }

public:
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        // If the goal is 0, the first player wins immediately
        if (desiredTotal <= 0) return true;
        
        // Calculate the maximum possible sum if all numbers are chosen
        int totalSum = (maxChoosableInteger * (maxChoosableInteger + 1)) / 2;
        if (totalSum < desiredTotal) return false;

        // Size of the memo array is 2^(maxChoosableInteger + 1) to accommodate bit offsets up to maxChoosableInteger
        size_t totalStates = 1ULL << (maxChoosableInteger + 1);
        memo.assign(totalStates, 0);

        return canWinHelper(maxChoosableInteger, desiredTotal, 0);
    }
};

int main() {
    int maxChoosableInteger, desiredTotal;
    cout << "Enter the maximum choosable integer: ";
    if (!(cin >> maxChoosableInteger) || maxChoosableInteger < 1 || maxChoosableInteger > 20) {
        cout << "Invalid input. Max choosable integer must be between 1 and 20." << endl;
        return 1;
    }
    cout << "Enter the desired total: ";
    if (!(cin >> desiredTotal) || desiredTotal < 0) {
        cout << "Invalid input. Desired total must be non-negative." << endl;
        return 1;
    }

    Solution game;
    if (game.canIWin(maxChoosableInteger, desiredTotal)) {
        cout << "\nThe first player can force a win!" << endl;
    } else {
        cout << "\nThe first player cannot guarantee a win." << endl;
    }

    return 0;
}