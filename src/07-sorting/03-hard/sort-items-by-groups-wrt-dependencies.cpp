#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
private:
    /*
     * KAHN'S TOPOLOGICAL SORT SUBSYSTEM
     * ---------------------------------
     * Performs a standard topological sort on a directed acyclic graph (DAG)
     * represented by adjacency lists using an in-degree counting vector.
     * Returns an empty vector if a cycle is detected.
     */
    vector<int> topoSort(const vector<vector<int>>& graph, vector<int>& inDegree, const vector<int>& nodes) {
        queue<int> zeroInDegreeQueue;
        
        // Seed the queue with all candidate nodes that have no incoming dependencies
        for (int node : nodes) {
            if (inDegree[static_cast<size_t>(node)] == 0) {
                zeroInDegreeQueue.push(node);
            }
        }

        vector<int> sortedOrder;
        sortedOrder.reserve(nodes.size());

        // Process nodes layer by layer, peeling off completed dependencies
        while (!zeroInDegreeQueue.empty()) {
            int curr = zeroInDegreeQueue.front();
            zeroInDegreeQueue.pop();
            sortedOrder.push_back(curr);

            for (int neighbor : graph[static_cast<size_t>(curr)]) {
                inDegree[static_cast<size_t>(neighbor)]--;
                if (inDegree[static_cast<size_t>(neighbor)] == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }

        // If the sorted output size doesn't match the node set size, a cyclic dependency exists
        if (sortedOrder.size() != nodes.size()) {
            return {};
        }
        return sortedOrder;
    }

public:
    /*
     * DOUBLE-TIERED TOPOLOGICAL SORT ENGINE
     * -------------------------------------
     * The objective is to sort n items belonging to m groups such that items belonging 
     * to the same group are placed next to each other, while respecting both item-level 
     * and group-level dependency constraints.
     *
     * Algorithmic Strategy:
     * - Step 1: Pre-process non-assigned items. Items with group == -1 do not belong to 
     *   any explicit group. We isolate them by assigning each its own unique, independent 
     *   virtual group ID starting from index m up to m + n.
     * - Step 2: Establish two distinct, parallel dependency graph structures:
     *     - `itemGraph`: Controls standard node-to-node constraints between individual items.
     *     - `groupGraph`: Controls macro-level dependencies between entire groups.
     * - Step 3: Populate graphs by reading the `beforeItems` list. For an edge from `before` to `i`:
     *     - Add an item-level edge: `itemGraph[before].push_back(i)`.
     *     - If the items belong to different groups, add a group-level edge: 
     *       `groupGraph[group[before]].push_back(group[i])`.
     * - Step 4: Run Kahn's Topological Sort on the groups to get a valid group evaluation sequence.
     * - Step 5: Run Kahn's Topological Sort on individual items inside each group to get valid inner sequences.
     * - Step 6: Flatten the ordered item groups following the sorted group macro sequence.
     */
    vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
        // Step 1: Isolate independent unassigned items into unique standalone groups
        int groupCounter = m;
        for (int i = 0; i < n; ++i) {
            if (group[static_cast<size_t>(i)] == -1) {
                group[static_cast<size_t>(i)] = groupCounter++;
            }
        }

        // Step 2: Instantiate structural representation primitives for items and groups
        vector<vector<int>> itemGraph(static_cast<size_t>(n));
        vector<int> itemInDegree(static_cast<size_t>(n), 0);

        vector<vector<int>> groupGraph(static_cast<size_t>(groupCounter));
        vector<int> groupInDegree(static_cast<size_t>(groupCounter), 0);

        // Step 3: Map incoming input dependencies to populate graph adjacency constraints
        for (int i = 0; i < n; ++i) {
            int toGroup = group[static_cast<size_t>(i)];
            for (int before : beforeItems[static_cast<size_t>(i)]) {
                int fromGroup = group[static_cast<size_t>(before)];
                
                // Add item-level dependency edge
                itemGraph[static_cast<size_t>(before)].push_back(i);
                itemInDegree[static_cast<size_t>(i)]++;

                // Add macro group-level dependency edge if crossing boundaries
                if (fromGroup != toGroup) {
                    groupGraph[static_cast<size_t>(fromGroup)].push_back(toGroup);
                    groupInDegree[static_cast<size_t>(toGroup)]++;
                }
            }
        }

        // Compile universal membership clusters listing which items belong to which group
        vector<int> uniqueGroupIDs(groupCounter);
        vector<vector<int>> groupMembers(static_cast<size_t>(groupCounter));
        for (int i = 0; i < groupCounter; ++i) uniqueGroupIDs[static_cast<size_t>(i)] = i;
        for (int i = 0; i < n; ++i) groupMembers[static_cast<size_t>(group[static_cast<size_t>(i)])].push_back(i);

        // Step 4: Perform macro topological sort across group nodes
        vector<int> sortedGroups = topoSort(groupGraph, groupInDegree, uniqueGroupIDs);
        if (sortedGroups.empty()) return {};

        // Step 5 & 6: Perform inner topological sorts and flatten results
        vector<int> globallySortedItems;
        globallySortedItems.reserve(static_cast<size_t>(n));

        for (int grpID : sortedGroups) {
            const auto& members = groupMembers[static_cast<size_t>(grpID)];
            if (members.empty()) continue;

            vector<int> sortedMembers = topoSort(itemGraph, itemInDegree, members);
            // If internal sorting fails due to a deadlock cycle, abort execution
            if (sortedMembers.empty()) return {};

            for (int item : sortedMembers) {
                globallySortedItems.push_back(item);
            }
        }

        return globallySortedItems;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + E) - Where N represents the total number of items and E represents 
 *                  the total number of dependency constraints in the beforeItems matrix. 
 *                  Graph building takes linear O(N + E) time. The double-tiered topological sorting 
 *                  passes inspect every node and edge exactly once, avoiding quadratic blowups.
 *
 * SPACE COMPLEXITY: O(N + E) Auxiliary - To hold the structural adjacency lists, in-degree metrics, 
 *                   and dynamic grouping allocation trackers.
 */
int main() {
    int n, m;
    cout << "Enter total number of items (n): ";
    if (!(cin >> n) || n <= 0) return 1;
    cout << "Enter baseline group count threshold (m): ";
    if (!(cin >> m) || m < 0) return 1;

    vector<int> group(static_cast<size_t>(n));
    cout << "Enter group ID assignments for each item (-1 signifies no group assignment):\n";
    for (int i = 0; i < n; ++i) {
        cin >> group[static_cast<size_t>(i)];
    }

    vector<vector<int>> beforeItems(static_cast<size_t>(n));
    cout << "\nSetting up dependency maps. For each item, enter its dependency capacity followed by the source item IDs:\n";
    for (int i = 0; i < n; ++i) {
        int count;
        cout << "Item [" << i << "] dependency count: ";
        cin >> count;
        if (count > 0) {
            beforeItems[static_cast<size_t>(i)].resize(static_cast<size_t>(count));
            cout << "Enter " << count << " prerequisite item IDs separated by spaces: ";
            for (int j = 0; j < count; ++j) {
                cin >> beforeItems[static_cast<size_t>(i)][static_cast<size_t>(j)];
            }
        }
    }

    Solution solver;
    cout << "\nExecuting double-tiered topological processing pipelines..." << endl;
    vector<int> resultSequence = solver.sortItems(n, m, group, beforeItems);

    if (resultSequence.empty()) {
        cout << "A circular sorting contradiction dependency loop was detected. Sorting is impossible." << endl;
    } else {
        cout << "Valid double-tiered sorted order sequence layout matches:\n[ ";
        for (size_t i = 0; i < resultSequence.size(); ++i) {
            cout << resultSequence[i] << (i + 1 < resultSequence.size() ? ", " : "");
        }
        cout << " ]\n";
    }

    return 0;
}