#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
CORE LOGIC (HASH MAP AND DYNAMIC VECTOR INTEGRATION):
- To achieve O(1) time complexity for insert, delete, and getRandom, we combine a dynamic vector and a hash map.
- The vector stores the actual elements, allowing O(1) random access by index for the `getRandom` operation.
- The hash map stores the element value as the key and its corresponding index in the vector as the value, enabling O(1) lookups.
- **Insert**: Check if the element exists in the map. If not, append it to the end of the vector and record its index in the map.
- **Delete**: Check if the element exists. To avoid O(N) array shifting during deletion, we swap the element to be deleted with the last element of the vector. We update the map with the new index of the swapped element, pop the last element from the vector, and erase the target element from the map.
- **GetRandom**: Generate a random index between 0 and `vector.size() - 1` and return the element at that index.

TIME COMPLEXITY: O(1) average time for insert, delete, and getRandom operations.
SPACE COMPLEXITY: O(N) - The vector and the hash map store up to N elements, where N is the number of unique elements inserted.
*/
class RandomizedSet {
private:
    vector<int> nums;
    unordered_map<int, int> numMap;

public:
    RandomizedSet() {
        // Initialize random seed
        srand(static_cast<unsigned int>(time(nullptr)));
    }
    
    bool insert(int val) {
        if (numMap.count(val)) {
            return false;
        }
        numMap[val] = static_cast<int>(nums.size());
        nums.push_back(val);
        return true;
    }
    
    bool remove(int val) {
        if (!numMap.count(val)) {
            return false;
        }
        int lastElement = nums.back();
        int idxToRemove = numMap[val];
        
        // Swap target element with the last element in the vector
        nums[idxToRemove] = lastElement;
        numMap[lastElement] = idxToRemove;
        
        // Remove the last element
        nums.pop_back();
        numMap.erase(val);
        return true;
    }
    
    int getRandom() {
        int randomIndex = rand() % nums.size();
        return nums[static_cast<size_t>(randomIndex)];
    }
};

int main() {
    RandomizedSet randSet;
    int choice, val;
    
    cout << "RandomizedSet Operations Menu:\n";
    cout << "1. Insert an element\n";
    cout << "2. Remove an element\n";
    cout << "3. Get a random element\n";
    cout << "4. Exit\n";
    
    while (true) {
        cout << "\nEnter your choice (1-4): ";
        cin >> choice;
        
        if (choice == 1) {
            cout << "Enter the integer value to insert: ";
            cin >> val;
            if (randSet.insert(val)) {
                cout << "Successfully inserted " << val << endl;
            } else {
                cout << val << " already exists in the set." << endl;
            }
        } else if (choice == 2) {
            cout << "Enter the integer value to remove: ";
            cin >> val;
            if (randSet.remove(val)) {
                cout << "Successfully removed " << val << endl;
            } else {
                cout << val << " does not exist in the set." << endl;
            }
        } else if (choice == 3) {
            // Check before calling random to prevent division by zero if empty
            try {
                int randomVal = randSet.getRandom();
                cout << "Randomly selected element: " << randomVal << endl;
            } catch (...) {
                cout << "Set is empty." << endl;
            }
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice. Please choose between 1 and 4." << endl;
        }
    }
    
    return 0;
}