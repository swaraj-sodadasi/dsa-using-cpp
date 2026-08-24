#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

// Definition for a Doubly-Linked List Node structure.
struct CacheNode {
    int key;
    int value;
    CacheNode* prev;
    CacheNode* next;
    CacheNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    // O(1) Key-to-Node Reference Indexing Map
    unordered_map<int, CacheNode*> cacheMap;
    
    // Sentinel boundary nodes to eliminate edge cases during pointer manipulation
    CacheNode* head;
    CacheNode* tail;

    /*
     * POINTER MANIPULATION PRIMITIVE: REMOVE NODE
     * Unlinks an existing node from its current position in the doubly linked list.
     */
    void removeNode(CacheNode* node) {
        CacheNode* prevNode = node->prev;
        CacheNode* nextNode = node->next;
        
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    /*
     * POINTER MANIPULATION PRIMITIVE: INSERT AT HEAD
     * Adds a node directly to the front of the list, immediately after the dummy head sentinel.
     * This location marks the node as the Most Recently Used (MRU) element.
     */
    void insertAtHead(CacheNode* node) {
        node->next = head->next;
        node->prev = head;
        
        head->next->prev = node;
        head->next = node;
    }

    /*
     * STRATEGY MECHANICS: MOVE TO HEAD
     * Promotes a node to the front of the list when it is accessed or updated, 
     * confirming its status as the Most Recently Used item.
     */
    void moveToHead(CacheNode* node) {
        removeNode(node);
        insertAtHead(node);
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DOUBLY LINKED LIST INTERLINKED WITH A HASH MAP
     * -------------------------------------------------------------------------------------
     * The objective is to design a Least Recently Used (LRU) Cache tracking structure 
     * operating under O(1) time complexity metrics for both `get` and `put` actions.
     * 
     * 1. THE ARCHITECTURAL COMPONENT INTERACTION:
     *    - Hash Map (`unordered_map`): Maps integer keys directly to their corresponding 
     *      `CacheNode*` addresses inside heap space. This provides O(1) access to any node.
     *    - Doubly Linked List: Maintains the sequence of element usage. The front of the list 
     *      (right after `head`) represents the Most Recently Used items, while the back 
     *      (right before `tail`) stores the Least Recently Used items.
     * 
     * 2. THE GET EVOLUTIONARY PRIMITIVE:
     *    When retrieving a key, if the key does not exist in the map, we return -1. 
     *    If it exists, we find the node instantly via the hash map and call `moveToHead(node)` 
     *    to refresh its usage status before returning its value.
     * 
     * 3. THE PUT EVOLUTIONARY PRIMITIVE:
     *    - Key Already Exists: We update the node's value field and call `moveToHead(node)`.
     *    - Fresh Key Insertion: We check if the cache is at capacity. If it is full, we locate 
     *      the eviction candidate right before `tail` (`tail->prev`). We remove it from the list, 
     *      erase its key from the hash map, and delete the node to clear memory. 
     *      Finally, we create a new node, add it to the hash map, and insert it at the head.
     */
    LRUCache(int cap) : capacity(cap) {
        // Instantiate the sentinel structures to secure linear boundaries
        head = new CacheNode(-1, -1);
        tail = new CacheNode(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        // Clean up remaining allocations inside the linked structure
        CacheNode* curr = head;
        while (curr != nullptr) {
            CacheNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    int get(int key) {
        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            return -1;
        }
        
        CacheNode* targetNode = it->second;
        moveToHead(targetNode);
        return targetNode->value;
    }

    void put(int key, int value) {
        auto it = cacheMap.find(key);
        
        if (it != cacheMap.end()) {
            // Key exists: update its value and promote it to the front
            CacheNode* existingNode = it->second;
            existingNode->value = value;
            moveToHead(existingNode);
        } else {
            // New entry: check capacity limits for potential evictions
            if (static_cast<int>(cacheMap.size()) >= capacity) {
                CacheNode* lruNode = tail->prev; // The Least Recently Used element sits right before tail
                
                // Evict the candidate from the tracking components
                removeNode(lruNode);
                cacheMap.erase(lruNode->key);
                delete lruNode;
            }
            
            // Allocate a fresh node, map it, and insert it at the head position
            CacheNode* freshNode = new CacheNode(key, value);
            cacheMap[key] = freshNode;
            insertAtHead(freshNode);
        }
    }

    // Helper visibility method to inspect the cache state sequence
    void displayCacheContents() {
        cout << "Cache Memory State (MRU -> LRU): ";
        CacheNode* curr = head->next;
        if (curr == tail) {
            cout << "[Empty Elements Matrix]";
        }
        while (curr != tail) {
            cout << "{" << curr->key << ":" << curr->value << "} ";
            curr = curr->next;
        }
        cout << endl;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Constant - Hash map lookups, node unlinking, and front-of-list insertions 
 *                  run instantly using constant execution paths, avoiding linear array shifts.
 *
 * SPACE COMPLEXITY: O(C) Auxiliary - Where C represents the allocated cache capacity. 
 *                   The hash map and doubly linked list store at most C nodes concurrently.
 */
int main() {
    int cacheCapacityLimits;
    cout << "=== LRU Interlinked Map-List Cache Memory Engine ===\n";
    cout << "Enter the maximum cache capacity: ";
    if (!(cin >> cacheCapacityLimits) || cacheCapacityLimits <= 0) {
        cout << "Invalid capacity constraints initialized." << endl;
        return 1;
    }

    LRUCache lru(cacheCapacityLimits);
    cout << "\nCache running with capacity set to " << cacheCapacityLimits << ".\n";
    cout << "Commands: 'put [key] [val]', 'get [key]', 'display', 'exit'\n\n";

    string dynamicCommand;
    while (cout << "> ", cin >> dynamicCommand) {
        if (dynamicCommand == "put") {
            int k, v;
            if (cin >> k >> v) {
                lru.put(k, v);
                cout << "Action Confirmed: Added/Updated {" << k << ":" << v << "}\n";
            }
        } else if (dynamicCommand == "get") {
            int k;
            if (cin >> k) {
                int lookupResult = lru.get(k);
                if (lookupResult != -1) {
                    cout << "Match Found: Key [" << k << "] = " << lookupResult << "\n";
                } else {
                    cout << "Cache Miss: Key [" << k << "] not found (-1)\n";
                }
            }
        } else if (dynamicCommand == "display") {
            lru.displayCacheContents();
        } else if (dynamicCommand == "exit") {
            break;
        } else {
            cout << "Unknown operational command sequence. Retry." << endl;
            // Clear input buffer stream flags if corrupted
            cin.clear();
            string clearBuffer;
            getline(cin, clearBuffer);
        }
    }

    return 0;
}