#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;

class Solution {
private:
    // Custom hash function for pair<int, int> to use in unordered_set
    struct PairHash {
        size_t operator()(const pair<int, int>& p) const {
            // Standard hashing optimization for 2D coordinate pairs
            return (static_cast<size_t>(p.first) << 32) ^ static_cast<size_t>(p.second);
        }
    };

    /*
     * BOUNDED EXPANSION PATH CHECK:
     * This function runs a BFS from the 'source' towards the 'target'.
     * Since the grid is extremely large (10^6 x 10^6), a standard BFS would Time Out (TLE).
     * 
     * However, the number of blocked cells is small (at most B = 200).
     * The maximum area that B blocked cells can seal against a corner or wall is bounded.
     * The maximum number of coordinates that can be completely enclosed by B blocks is 
     * given by the triangular area calculation: B * (B - 1) / 2.
     * 
     * If the BFS queue manages to expand and visit MORE than this threshold of states 
     * without hitting a block or boundary closure, the search has escaped the barrier 
     * and reached the open, infinite region of the grid.
     */
    bool isEscapePossible(const unordered_set<pair<int, int>, PairHash>& blockedSet, 
                            const pair<int, int>& source, 
                            const pair<int, int>& target, 
                            int maxBlocks) {
        
        // Maximum unique coordinates that can be sealed off by N blocks
        size_t maxVisitedLimit = static_cast<size_t>(maxBlocks * (maxBlocks - 1) / 2);

        queue<pair<int, int>> q;
        unordered_set<pair<int, int>, PairHash> visited;

        q.push(source);
        visited.insert(source);

        int dRow[] = {-1, 1, 0, 0};
        int dCol[] = {0, 0, -1, 1};

        const int GRID_LIMIT = 1000000;

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            // Case 1: Target is reached directly
            if (r == target.first && c == target.second) {
                return true;
            }

            // Case 2: Visited size exceeds the maximum possible sealed area.
            // This guarantees we have successfully broken out into the open region.
            if (visited.size() > maxVisitedLimit) {
                return true;
            }

            for (int i = 0; i < 4; ++i) {
                int nr = r + dRow[i];
                int nc = c + dCol[i];

                // Ensure coordinates stay within the 10^6 x 10^6 boundary
                if (nr >= 0 && nr < GRID_LIMIT && nc >= 0 && nc < GRID_LIMIT) {
                    pair<int, int> nextPos = {nr, nc};

                    if (blockedSet.find(nextPos) == blockedSet.end() && visited.find(nextPos) == visited.end()) {
                        visited.insert(nextPos);
                        q.push(nextPos);
                    }
                }
            }
        }

        // BFS terminated without reaching target or breaking past the visited limit.
        // The source is completely enclosed by blocked cells.
        return false;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ESCAPE A LARGE MAZE (BOUNDED-GROWTH BARRIER VERIFICATION BFS)
     * -------------------------------------------------------------------------------------
     * The objective is to determine if we can move from `source` to `target` on a 
     * 1 million x 1 million grid with at most 200 blocked cells.
     * 
     * 1. THE DOUBLE-SIDED VERIFICATION PRINCIPLE:
     *    - If only the source is blocked off: Return false.
     *    - If only the target is blocked off: Return false.
     *    - To verify escape, we must execute the bounded BFS twice:
     *      1. From source -> target (verifies source is not trapped).
     *      2. From target -> source (verifies target is not trapped).
     * 
     * 2. CHRONOLOGICAL COMPLEXITY PRUNING:
     *    By bounding visited limits to O(B^2), the runtime decreases from O(GRID_SIZE) 
     *    to O(B^2), where B is the number of blocks.
     */
    bool isEscapePossible(const vector<vector<int>>& blocked, const vector<int>& source, const vector<int>& target) {
        if (blocked.empty()) return true;

        unordered_set<pair<int, int>, PairHash> blockedSet;
        for (const auto& block : blocked) {
            blockedSet.insert({block[0], block[1]});
        }

        pair<int, int> src = {source[0], source[1]};
        pair<int, int> tgt = {target[0], target[1]};

        int maxBlocks = static_cast<int>(blocked.size());

        // Both start and destination positions must prove they can reach the infinite open plane
        return isEscapePossible(blockedSet, src, tgt, maxBlocks) && 
               isEscapePossible(blockedSet, tgt, src, maxBlocks);
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(B^2) - Where B is the number of blocked cells (at most 200). 
 *                  The BFS halts after exploring at most B*(B-1)/2 states.
 *
 * SPACE COMPLEXITY: O(B^2) Auxiliary - To hold visited coordinates in memory.
 */
int main() {
    cout << "=== Bounded-Growth Escape Large Maze BFS ===\n";
    cout << "Enter the number of blocked cells: ";
    int numBlocks;
    if (!(cin >> numBlocks) || numBlocks < 0) return 1;

    vector<vector<int>> blocked;
    if (numBlocks > 0) {
        cout << "Enter the blocked cells coordinates (row col):\n";
        for (int i = 0; i < numBlocks; ++i) {
            int r, c;
            cin >> r >> c;
            blocked.push_back({r, c});
        }
    }

    cout << "Enter source coordinates (row col): ";
    vector<int> source(2);
    cin >> source[0] >> source[1];

    cout << "Enter target coordinates (row col): ";
    vector<int> target(2);
    cin >> target[0] >> target[1];

    Solution solver;
    cout << "\nExecuting double-sided bounded barrier sweeps..." << endl;
    bool status = solver.isEscapePossible(blocked, source, target);

    if (status) {
        cout << "Path verification successful! Escape is POSSIBLE. Output: [true]\n";
    } else {
        cout << "Path verification failed. Source or Target is trapped. Output: [false]\n";
    }

    return 0;
}