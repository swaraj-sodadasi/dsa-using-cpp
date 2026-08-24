#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

// Definition for a Doubly-Linked List Node structure.
struct CacheNode {
    int key;
    int value;
    int frequency;
    CacheNode* prev;
    CacheNode* next;
    
    CacheNode(int k, int v) : key(k), value(v), frequency(1), prev(nullptr), next(nullptr) {}
};

// Definition for a Doubly-Linked List tracking nodes with the same frequency tier.
class FrequencyList {
public:
    CacheNode* head;
    CacheNode* tail;

    FrequencyList() {
        // Instantiate sentinel boundaries to eliminate structural edge cases
        head = new CacheNode(-1, -1);
        tail = new CacheNode(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    ~FrequencyList() {
        // Clean up sentinel structures
        delete head;
        delete tail;
    }

    // Check if the frequency tier list is empty (contains only sentinels)
    bool isEmpty() const {
        return head->next == tail;
    }

    /*
     * POINTER MANIPULATION PRIMITIVE: INSERT AT HEAD
     * Adds a node directly to the front of this frequency list tier (Most Recently Used within tier).
     */
    void insertAtHead(CacheNode* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    /*
     * POINTER MANIPULATION PRIMITIVE: REMOVE NODE
     * Unlinks an existing node from its current position in this doubly linked list.
     */
    void removeNode(CacheNode* node) {
        CacheNode* prevNode = node->prev;
        CacheNode* nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }
};

class LFUCache {
private:
    int capacity;
    int minimumFrequency;
    
    // O(1) Key-to-Node Reference Indexing Map
    unordered_map<int, CacheNode*> cacheMap;
    
    // O(1) Frequency-to-DoublyLinkedList Component Allocation Map
    unordered_map<int, FrequencyList*> frequencyMap;

    /*
     * STRATEGY MECHANICS: UPDATE NODE FREQUENCY STATE
     * Promotes a node's frequency tier when accessed or updated.
     * Moves it from its old frequency list to the updated (frequency + 1) list.
     */
    void updateNodeFrequency(CacheNode* node) {
        int oldFreq = node->frequency;
        FrequencyList* oldList = frequencyMap[oldFreq];
        oldList->removeNode(node);

        // If the current minimum frequency list becomes empty, increment the global tracking metric
        if (oldFreq == minimumFrequency && oldList->isEmpty()) {
            minimumFrequency++;
        }

        // Increment node frequency tier and register within the new frequency list mapping
        node->frequency++;
        int newFreq = node->frequency;
        
        if (frequencyMap.find(newFreq) == frequencyMap.end()) {
            frequencyMap[newFreq] = new FrequencyList();
        }
        frequencyMap[newFreq]->insertAtHead(node);
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: FREQUENCY MAP OF DOUBLY LINKED LISTS TRACKING EXPIRATIONS
     * -------------------------------------------------------------------------------------
     * The objective is to design a Least Frequently Used (LFU) Cache structure operating 
     * under O(1) time complexity constraints for both `get` and `put` operations.
     * 
     * 1. THE ARCHITECTURAL STRATEGY:
     *    - `cacheMap`: Maps keys directly to their node positions for instant access.
     *    - `frequencyMap`: Maps frequencies to dedicated `FrequencyList` doubly-linked chains.
     *      Each frequency chain orders its elements using an LRU policy (Least Recently Used 
     *      at the tail, Most Recently Used at the head).
     * 
     * 2. THE EXPIRATION MATRICES (TIE-BREAKING ALGORITHM):
     *    When the cache hits maximum capacity limits, the eviction candidate must satisfy two 
     *    conditions concurrently:
     *      - Condition A: It must belong to the lowest frequency tier (`minimumFrequency`).
     *      - Condition B: If multiple nodes share that minimal frequency, the one that is 
     *        least recently used (the tail node of that specific list) is evicted.
     */
    LFUCache(int cap) : capacity(cap), minimumFrequency(0) {}

    ~LFUCache() {
        // Deallocate all dynamically allocated lists and nodes safely
        for (auto& pair : cacheMap) {
            delete pair.second;
        }
        for (auto& pair : frequencyMap) {
            delete pair.second;
        }
    }

    int get(int key) {
        if (capacity <= 0) return -1;
        
        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            return -1;
        }

        CacheNode* targetNode = it->second;
        updateNodeFrequency(targetNode);
        return targetNode->value;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;

        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            // Key exists: update its value and increment its frequency tier
            CacheNode* existingNode = it->second;
            existingNode->value = value;
            updateNodeFrequency(existingNode);
        } else {
            // New entry: check capacity boundaries for potential evictions
            if (static_cast<int>(cacheMap.size()) >= capacity) {
                // Locate the eviction candidate at the tail of the lowest frequency list tier
                FrequencyList* minFreqList = frequencyMap[minimumFrequency];
                CacheNode* evictionCandidate = minFreqList->tail->prev;

                // Evict the node from the tracking maps
                minFreqList->removeNode(evictionCandidate);
                cacheMap.erase(evictionCandidate->key);
                delete evictionCandidate;
            }

            // Allocate a fresh node, register it, and insert it into the frequency 1 list
            CacheNode* freshNode = new CacheNode(key, value);
            cacheMap[key] = freshNode;
            
            // A new insertion naturally sets the lowest global frequency metric back to 1
            minimumFrequency = 1;
            if (frequencyMap.find(1) == frequencyMap.end()) {
                frequencyMap[1] = new FrequencyList();
            }
            frequencyMap[1]->insertAtHead(freshNode);
        }
    }

    // Helper visibility method to inspect the cache state layout
    void displayCacheContents() {
        cout << "--- LFU Cache Map Layout ---\n";
        bool empty = true;
        for (auto const& pair : frequencyMap) {
            FrequencyList* fList = pair.second;
            if (!fList->isEmpty()) {
                empty = false;
                cout << "Frequency Tier [" << pair.first << "] (MRU -> LRU): ";
                CacheNode* curr = fList->head->next;
                while (curr != fList->tail) {
                    cout << "{" << curr->key << ":" << curr->value << "} ";
                    curr = curr->next;
                }
                cout << "\n";
            }
        }
        if (empty) {
            cout << "[Empty Elements Matrix]\n";
        }
        cout << "Current Minimum Frequency Tracker: " << minimumFrequency << "\n----------------------------\n";
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Constant - Hash map lookups and node re-linking across frequency lists 
 *                  run instantly using fixed execution paths, completely avoiding linear scans.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Where N is the number of elements inside the cache. 
 *                   The hash map, frequency map, and doubly linked node allocations scale linearly 
 *                   with capacity limits.
 */
int main() {
    int cacheCapacityLimits;
    cout << "=== LFU Interlinked Map Frequency Cache Engine ===\n";
    cout << "Enter the maximum cache capacity: ";
    if (!(cin >> cacheCapacityLimits) || cacheCapacityLimits <= 0) {
        cout << "Invalid capacity constraints initialized." << endl;
        return 1;
    }

    LFUCache lfu(cacheCapacityLimits);
    cout << "\nCache running with capacity set to " << cacheCapacityLimits << ".\n";
    cout << "Commands: 'put [key] [val]', 'get [key]', 'display', 'exit'\n\n";

    string dynamicCommand;
    while (cout << "> ", cin >> dynamicCommand) {
        if (dynamicCommand == "put") {
            int k, v;
            if (cin >> k >> v) {
                lfu.put(k, v);
                cout << "Action Confirmed: Added/Updated {" << k << ":" << v << "}\n";
            }
        } else if (dynamicCommand == "get") {
            int k;
            if (cin >> k) {
                int lookupResult = lfu.get(k);
                if (lookupResult != -1) {
                    cout << "Match Found: Key [" << k << "] = " << lookupResult << "\n";
                } else {
                    cout << "Cache Miss: Key [" << k << "] not found (-1)\n";
                }
            }
        } else if (dynamicCommand == "display") {
            lfu.displayCacheContents();
        } else if (dynamicCommand == "exit") {
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