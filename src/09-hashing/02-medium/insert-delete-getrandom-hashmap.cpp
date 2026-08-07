#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class RandomizedSet {
private:
    // Dynamic vector array to hold element values for O(1) random index access
    vector<int> dataList;
    
    // Hash map pairing element values to their corresponding index location inside dataList
    // Map stores: {Element_Value -> Vector_Index}
    unordered_map<int, int> valueToIndexMap;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: HASH MAP CROSS-LINKED WITH DYNAMIC VECTOR
     * -------------------------------------------------------------------------------------
     * 1. THE ARCHITECTURAL COMPLEMENT PROPOSAL:
     *    - A hash map provides O(1) insertion and deletion lookups, but lacks continuous 
     *      index tracking, making O(1) random retrieval impossible.
     *    - A dynamic vector provides O(1) index access and rapid tail insertion, but incurs 
     *      an expensive O(N) shift cost when removing arbitrary internal elements.
     *    Cross-linking these two structures addresses the limitations of each.
     * 
     * 2. O(1) SWAP-AND-POP DELETION PATTERN:
     *    To delete an internal element from the vector in constant time without shifting elements, 
     *    we swap it with the last element in the vector. 
     *    - We locate the index of the element to delete using our hash map.
     *    - We overwrite that index location with the value of the final element.
     *    - We update the map registry for the moved element to point to its new index position.
     *    - We truncate the tail of the vector in O(1) time using pop_back() and erase the target from our map.
     */
    RandomizedSet() {
        // Seed the standard pseudo-random number generator machine
        srand(static_cast<unsigned int>(time(nullptr)));
    }
    
    /** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
    bool insert(int val) {
        if (valueToIndexMap.find(val) != valueToIndexMap.end()) {
            return false;
        }
        
        valueToIndexMap[val] = static_cast<int>(dataList.size());
        dataList.push_back(val);
        return true;
    }
    
    /** Removes a value from the set. Returns true if the set contained the specified element. */
    bool remove(int val) {
        auto it = valueToIndexMap.find(val);
        if (it == valueToIndexMap.end()) {
            return false;
        }
        
        int deleteIndex = it->second;
        int lastElementValue = dataList.back();
        
        if (deleteIndex != static_cast<int>(dataList.size()) - 1) {
            dataList[static_cast<size_t>(deleteIndex)] = lastElementValue;
            valueToIndexMap[lastElementValue] = deleteIndex;
        }
        
        dataList.pop_back();
        valueToIndexMap.erase(it);
        
        return true;
    }
    
    /** Get a random element from the set. */
    int getRandom() {
        if (dataList.empty()) {
            throw runtime_error("Cannot execute getRandom on an empty set instance.");
        }
        size_t randomIndex = static_cast<size_t>(rand()) % dataList.size();
        return dataList[randomIndex];
    }

    /** Helper function to return current size metrics */
    bool isEmpty() const {
        return dataList.empty();
    }

    /** Helper to print elements currently stored */
    void printSet() const {
        cout << "Current Internal Vector State: [ ";
        for (size_t i = 0; i < dataList.size(); ++i) {
            cout << dataList[i] << (i + 1 < dataList.size() ? ", " : "");
        }
        cout << " ]\n";
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: Amortized O(1) for insert, remove, and getRandom operations.
 * SPACE COMPLEXITY: O(N) Auxiliary memory allocation.
 */
int main() {
    RandomizedSet dynamicSet;
    string command;
    int value;

    cout << "=== RandomizedSet Dynamic Interactive Console ===\n";
    cout << "Available Operations:\n";
    cout << "  insert <val>  - Add a value to the set\n";
    cout << "  remove <val>  - Delete a value from the set\n";
    cout << "  random        - Fetch a random value from the set\n";
    cout << "  print         - Show internal tracking elements\n";
    cout << "  exit          - Terminate the simulation\n";
    cout << "=================================================\n\n";

    while (true) {
        cout << "\nEnter command: ";
        if (!(cin >> command)) break;

        if (command == "insert") {
            if (cin >> value) {
                bool outcome = dynamicSet.insert(value);
                cout << ">> Result: " << (outcome ? "True (Success)" : "False (Element already exists)") << "\n";
            } else {
                cout << ">> Error: Invalid integer value provided.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } 
        else if (command == "remove") {
            if (cin >> value) {
                bool outcome = dynamicSet.remove(value);
                cout << ">> Result: " << (outcome ? "True (Success)" : "False (Element not found)") << "\n";
            } else {
                cout << ">> Error: Invalid integer value provided.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } 
        else if (command == "random") {
            if (dynamicSet.isEmpty()) {
                cout << ">> Result: Operation rejected. Set container is empty.\n";
            } else {
                cout << ">> Random Element Isolated: " << dynamicSet.getRandom() << "\n";
            }
        } 
        else if (command == "print") {
            dynamicSet.printSet();
        } 
        else if (command == "exit") {
            cout << "Exiting dynamic environment. Goodbye!\n";
            break;
        } 
        else {
            cout << ">> Error: Unknown command syntax error. Try again.\n";
            cin.ignore(10000, '\n'); // Clear residual input stream buffer
        }
    }

    return 0;
}