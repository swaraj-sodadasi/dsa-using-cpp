#include <iostream>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

/*
 * INTERVAL TREE / SEGMENT INSERTION SORTING AND DELETION ENGINE
 * -------------------------------------------------------------
 * The objective is to design a data structure to track tracking segments of half-open intervals 
 * [left, right) and query or mutate them dynamically in logarithmic time.
 *
 * Structural Strategy (Disjoint Interval Coordination Mapping):
 * - We leverage a standard balanced BST mapping (`std::map<int, int>`) where each entry 
 *   represents a disjoint tracking segment in the format: `intervals[left_boundary] = right_boundary`.
 * - The map elements are automatically maintained in a sorted order based on their keys (`left`), 
 *   allowing for fast interval lookup operations via binary search (`upper_bound`).
 *
 * Algorithmic Mutation Protocols:
 * - addRange(left, right):
 *   1. Search the map using `upper_bound(left)` to isolate overlapping candidates.
 *   2. Merge all overlapping or contiguous ranges into a single unified tracking block.
 *   3. Erase the redundant intermediate sub-segments from the map and update the global boundaries.
 * 
 * - removeRange(left, right):
 *   1. Isolate overlapping sub-ranges within the targeted interval footprint.
 *   2. If a segment extends past our target borders, split the interval and slice out 
 *      the removed window, rewriting the remaining disjoint sub-segments back into the map.
 * 
 * - queryRange(left, right):
 *   1. Locate the closest preceding interval header using `upper_bound(left)`.
 *   2. Verify if the located interval fully covers the requested query window `[left, right)`.
 */
class RangeModule {
private:
    map<int, int> intervals;

public:
    RangeModule() {}

    void addRange(int left, int right) {
        // Step 1: Find the first interval starting strictly after 'left'
        auto it = intervals.upper_bound(left);
        
        // If there is a preceding interval that overlaps or touches 'left', merge it
        if (it != intervals.begin()) {
            auto prev = next(it, -1);
            if (prev->second >= left) {
                left = prev->first;
                right = max(right, prev->second);
                it = prev; // Move iterator back to capture the updated starting node position
            }
        }

        // Step 2: Merge all subsequent intervals that overlap or touch our expanding range
        while (it != intervals.end() && it->first <= right) {
            right = max(right, it->second);
            it = intervals.erase(it); // Remove the redundant sub-segment from the map
        }

        // Step 3: Insert the consolidated interval back into the sorted tracking map
        intervals[left] = right;
    }

    bool queryRange(int left, int right) {
        auto it = intervals.upper_bound(left);
        if (it == intervals.begin()) return false;
        
        auto prev = next(it, -1);
        // The query is satisfied if the preceding interval completely covers [left, right)
        return prev->second >= right;
    }

    void removeRange(int left, int right) {
        auto it = intervals.upper_bound(left);
        
        // Check if a preceding interval overlaps with the range to be removed
        if (it != intervals.begin()) {
            auto prev = next(it, -1);
            if (prev->second > left) {
                // If the interval extends past 'right', it splits into two disjoint fragments
                if (prev->second > right) {
                    intervals[right] = prev->second;
                }
                prev->second = left; // Shrink the right border of the left fragment
            }
        }

        // Wipe out all subsequent intervals fully submerged inside the removed segment window
        while (it != intervals.end() && it->first < right) {
            if (it->second > right) {
                // If it extends beyond 'right', shorten its left boundary and save it
                intervals[right] = it->second;
                intervals.erase(it);
                break;
            }
            it = intervals.erase(it);
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY:
 *   - addRange:    O(Log N + K) Amortized - Where N represents the count of tracking interval nodes 
 *                  in the map, and K is the count of overlapping items purged during convergence.
 *   - removeRange: O(Log N + K) Amortized - Looks up boundary parameters using binary search 
 *                  and updates internal tracking segments in logarithmic time.
 *   - queryRange:  O(Log N) - Standard lookup targeting a specific index element inside a balanced BST.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The internal coordination map allocates node objects scaling 
 *                   proportionally with the total number of disjoint segments stored.
 */
int main() {
    RangeModule rangeModule;
    cout << "Range Module Subsystem deployed successfully." << endl;
    cout << "Commands available: add [left] [right], query [left] [right], remove [left] [right], exit" << endl;
    
    string command;
    int left, right;
    
    while (true) {
        cout << "\nEnter command: ";
        if (!(cin >> command)) break;
        
        if (command == "exit") {
            break;
        }
        
        if (command == "add") {
            if (cin >> left >> right) {
                if (left >= right) {
                    cout << "Error: Left boundary must be strictly less than right boundary." << endl;
                } else {
                    rangeModule.addRange(left, right);
                    cout << "Successfully added interval [" << left << ", " << right << ")." << endl;
                }
            }
        } 
        else if (command == "query") {
            if (cin >> left >> right) {
                if (left >= right) {
                    cout << "Error: Left boundary must be strictly less than right boundary." << endl;
                } else {
                    bool res = rangeModule.queryRange(left, right);
                    cout << "Query [" << left << ", " << right << ") Result: " 
                         << (res ? "TRUE (Fully Covered)" : "FALSE (Not Fully Covered)") << endl;
                }
            }
        } 
        else if (command == "remove") {
            if (cin >> left >> right) {
                if (left >= right) {
                    cout << "Error: Left boundary must be strictly less than right boundary." << endl;
                } else {
                    rangeModule.removeRange(left, right);
                    cout << "Successfully removed interval [" << left << ", " << right << ")." << endl;
                }
            }
        } 
        else {
            cout << "Invalid command syntax. Use 'add', 'query', 'remove', or 'exit'." << endl;
            // Clear stream error states or bad input characters remaining in buffer lines
            cin.clear();
            string discard;
            getline(cin, discard);
        }
    }
    
    return 0;
}