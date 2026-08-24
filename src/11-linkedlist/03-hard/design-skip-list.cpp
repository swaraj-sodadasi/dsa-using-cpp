#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// Definition for a Skip List Node supporting dynamic index lanes.
struct SkipNode {
    int key;
    // A forward index array where forward[i] stores the pointer to the next node at level i.
    // Level indices are 0-indexed, ranging from 0 up to maxLevel - 1.
    vector<SkipNode*> forward;

    SkipNode(int k, int level) : key(k), forward(level, nullptr) {}
};

class Skiplist {
private:
    int maxLevel;
    float probability;
    int currentLevel;
    SkipNode* head;

    /*
     * HELPER FUNCTION: COIN-FLIP PROBABILISTIC LEVEL GENERATION
     * Simulates consecutive geometric distributions via coin tosses to assign 
     * structural forward tier indexes to new data values.
     */
    int generateRandomLevel() {
        int lvl = 1;
        // Keep increasing the level based on the probability parameter threshold check
        while ((static_cast<float>(rand()) / RAND_MAX) < probability && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: MULTI-LEVEL PROBABILISTIC NODE FORWARD INDEXES LAYOUT
     * -------------------------------------------------------------------------------------
     * The objective is to design a Skip List structure that supports search, addition, 
     * and deletion operations in average O(log N) time without needing complex balanced trees.
     * 
     * 1. THE PROBABILISTIC SEARCH SYSTEM:
     *    A Skip List balances itself using probability instead of strict structural rebalancing. 
     *    It runs multiple layers of forward nodes where level 0 contains the full, sorted 
     *    linked list, and higher levels skip elements to act as express lanes.
     * 
     * 2. ELEMENT LOOKUP ENGINE:
     *    We start searching from the highest active level down to level 0. At each level, 
     *    we slide horizontally as long as the next node's value is strictly smaller than our 
     *    target. Once we hit a value that is larger or equal, we drop down one level and 
     *    repeat. When we reach level 0 and stop, the node right next to us is our candidate.
     * 
     * 3. THE UPDATE INDEX MATRIX (INSERT/DELETE):
     *    To modify links correctly across levels, we track the inflection points using an 
     *    `update` pointer array. This array saves the last node visited at each level before 
     *    dropping down. 
     *      - For `add`: We fetch a random level, create the new node, and use the `update` array 
     *        to stitch its links into each active level.
     *      - For `erase`: We check if the target node exists at level 0. If found, we trace back 
     *        through the `update` array to bypass the node at all levels, then delete it.
     */
    Skiplist() {
        maxLevel = 16;       // Upper index level capacity limit bound configurations
        probability = 0.5f;  // Balanced fractional coin toss factor parameters
        currentLevel = 1;    // Base initialization level track metric
        
        // Instantiate the sentinel dummy head pointer containing maximum level lanes
        head = new SkipNode(-1, maxLevel);
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    ~Skiplist() {
        // Clean up allocated heap memory nodes sequentially along the base level 0 track
        SkipNode* curr = head;
        while (curr != nullptr) {
            SkipNode* temp = curr;
            curr = curr->forward[0];
            delete temp;
        }
    }

    bool search(int target) {
        SkipNode* curr = head;
        
        // Sweep horizontally across index lanes from top to bottom
        for (int i = currentLevel - 1; i >= 0; --i) {
            while (curr->forward[i] != nullptr && curr->forward[i]->key < target) {
                curr = curr->forward[i];
            }
        }
        
        // Move to the actual candidate position at level 0
        curr = curr->forward[0];
        return (curr != nullptr && curr->key == target);
    }

    void add(int num) {
        // An update array saves the nodes where we change links at each level
        vector<SkipNode*> update(static_cast<size_t>(maxLevel), nullptr);
        SkipNode* curr = head;

        for (int i = currentLevel - 1; i >= 0; --i) {
            while (curr->forward[i] != nullptr && curr->forward[i]->key < num) {
                curr = curr->forward[i];
            }
            update[static_cast<size_t>(i)] = curr;
        }

        // Generate a probabilistic level value for the new element
        int randomLvl = generateRandomLevel();

        // If the generated level is higher than our current level, pad the update array
        if (randomLvl > currentLevel) {
            for (int i = currentLevel; i < randomLvl; ++i) {
                update[static_cast<size_t>(i)] = head;
            }
            currentLevel = randomLvl;
        }

        // Allocate the new node and stitch it into all active levels
        SkipNode* freshNode = new SkipNode(num, randomLvl);
        for (int i = 0; i < randomLvl; ++i) {
            freshNode->forward[static_cast<size_t>(i)] = update[static_cast<size_t>(i)]->forward[static_cast<size_t>(i)];
            update[static_cast<size_t>(i)]->forward[static_cast<size_t>(i)] = freshNode;
        }
    }

    bool erase(int num) {
        vector<SkipNode*> update(static_cast<size_t>(maxLevel), nullptr);
        SkipNode* curr = head;

        for (int i = currentLevel - 1; i >= 0; --i) {
            while (curr->forward[i] != nullptr && curr->forward[i]->key < num) {
                curr = curr->forward[i];
            }
            update[static_cast<size_t>(i)] = curr;
        }

        // Move to the target candidate position at level 0
        curr = curr->forward[0];

        // Target found: bypass the node at all levels and clean up its memory allocation
        if (curr != nullptr && curr->key == num) {
            for (int i = 0; i < currentLevel; ++i) {
                if (update[static_cast<size_t>(i)]->forward[static_cast<size_t>(i)] != curr) {
                    break; // Terminate early if the node doesn't exist at higher levels
                }
                update[static_cast<size_t>(i)]->forward[static_cast<size_t>(i)] = curr->forward[static_cast<size_t>(i)];
            }
            
            delete curr;

            // Recalculate active level heights if the highest layers become empty
            while (currentLevel > 1 && head->forward[static_cast<size_t>(currentLevel - 1)] == nullptr) {
                currentLevel--;
            }
            return true;
        }
        
        return false; // Element not found
    }

    // Helper visibility method to inspect multi-level index layout lanes
    void displaySkiplistStructure() {
        cout << "\n--- Multi-Level Forward Index Layout Matrix ---\n";
        for (int i = currentLevel - 1; i >= 0; --i) {
            SkipNode* curr = head->forward[static_cast<size_t>(i)];
            cout << "Lane Layer [" << i << "]: Head -> ";
            while (curr != nullptr) {
                cout << "[" << curr->key << "] -> ";
                curr = curr->forward[static_cast<size_t>(i)];
            }
            cout << "NULL\n";
        }
        cout << "-----------------------------------------------\n";
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(log N) Average / O(N) Worst-Case - Search, insertion, and deletion scale 
 *                  logarithmically by bypassing large sections of the list via express lanes. 
 *                  The worst-case runtime degrades to linear time only if coin flips yield highly 
 *                  unbalanced, flat configurations.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Each node stores a variable-sized array of forward pointers. 
 *                   The total number of pointers across the entire structure scales linearly 
 *                   with the number of nodes.
 */
int main() {
    cout << "=== Probabilistic Multi-Level Index Skip List Console ===\n";
    Skiplist skipList;

    cout << "Commands: 'add [val]', 'search [val]', 'erase [val]', 'display', 'exit'\n\n";

    string operationalCommand;
    while (cout << "> ", cin >> operationalCommand) {
        if (operationalCommand == "add") {
            int value;
            if (cin >> value) {
                skipList.add(value);
                cout << "Action Confirmed: Added key value [" << value << "]\n";
            }
        } else if (operationalCommand == "search") {
            int value;
            if (cin >> value) {
                if (skipList.search(value)) {
                    cout << "Match Found: Key [" << value << "] exists inside structure.\n";
                } else {
                    cout << "Not Found: Key [" << value << "] does not exist.\n";
                }
            }
        } else if (operationalCommand == "erase") {
            int value;
            if (cin >> value) {
                if (skipList.erase(value)) {
                    cout << "Success: Key [" << value << "] successfully evicted from index maps.\n";
                } else {
                    cout << "Erase Failed: Key [" << value << "] not found.\n";
                }
            }
        } else if (operationalCommand == "display") {
            skipList.displaySkiplistStructure();
        } else if (operationalCommand == "exit") {
            break;
        } else {
            cout << "Unknown operational command sequence. Retry." << endl;
            cin.clear();
            string clearBuffer;
            getline(cin, clearBuffer);
        }
    }

    return 0;
}