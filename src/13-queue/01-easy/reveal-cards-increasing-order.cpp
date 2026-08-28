#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: REVEAL CARDS IN INCREASING ORDER (FIFO DECK INDEX SIMULATION)
     * -------------------------------------------------------------------------------------
     * The objective is to rearrange a deck of cards so that when we reveal them:
     *   1. We reveal the top card and take it out of the deck.
     *   2. We move the next top card to the bottom of the deck.
     *   3. We repeat this until all cards are revealed in increasing order.
     * 
     * 1. THE INDEX QUEUE TRANSITION:
     *    Instead of arranging the cards directly, we simulate the exact process using a 
     *    FIFO queue containing the *indices* of the final array: `[0, 1, 2, ..., N-1]`.
     * 
     * 2. RECONSTRUCTION PIPELINE:
     *    - Sort the deck of cards in ascending order.
     *    - For each sorted card:
     *        - Pop the front index from the queue; this index receives the next smallest card.
     *        - If the queue is not empty, pop the next front index and push it to the back.
     *    This maps sorted cards to their correct target positions.
     */
    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        int n = static_cast<int>(deck.size());
        sort(deck.begin(), deck.end());

        queue<int> indexQueue;
        for (int i = 0; i < n; ++i) {
            indexQueue.push(i);
        }

        vector<int> result(static_cast<size_t>(n));

        for (int card : deck) {
            // Assign the smallest remaining card to the current active front index
            result[static_cast<size_t>(indexQueue.front())] = card;
            indexQueue.pop();

            // Simulate moving the next card to the bottom of the deck
            if (!indexQueue.empty()) {
                indexQueue.push(indexQueue.front());
                indexQueue.pop();
            }
        }

        return result;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Sorting the deck takes O(N log N) time. The queue-based 
 *                  index tracking simulation processes each index in O(1) time, scaling 
 *                  linearly to O(N) overall.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The index queue and output vector require linear 
 *                   space scaling with the input deck size.
 */
int main() {
    cout << "=== Simulated FIFO Deck Interleaving Rearrangement Engine ===\n";
    cout << "Enter the total number of cards in the deck: ";
    int count;
    if (!(cin >> count) || count <= 0) return 1;

    vector<int> deck(static_cast<size_t>(count));
    cout << "Enter the card values sequentially:\n";
    for (int i = 0; i < count; ++i) {
        cin >> deck[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nSimulating deck dealing transitions and ordering cards..." << endl;
    vector<int> orderedDeck = solver.deckRevealedIncreasing(deck);

    cout << "Optimized Deck Arrangement Ledger:\n[";
    for (size_t i = 0; i < orderedDeck.size(); ++i) {
        cout << orderedDeck[i] << (i + 1 < orderedDeck.size() ? ", " : "");
    }
    cout << "]\n";

    return 0;
}