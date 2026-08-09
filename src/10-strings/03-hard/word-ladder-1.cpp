#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

class Solution {
private:
    // Directed graph adjacency mapping tracking shortest path transitions exclusively
    unordered_map<string, vector<string>> adjList;
    // Tracks the minimum layer depth distance from the beginWord to each visited node
    unordered_map<string, int> shortestDistances;
    // Stores the collective optimized shortest paths collected by the backtracking loop
    vector<vector<string>> allShortestPaths;

    /*
     * PHASE B: DEPTH-FIRST BACKTRACKING RESOLUTION
     * Recursively walks down the directed shortest path matrix constructed by the BFS phase.
     */
    void backtrackPathGraph(const string& currentWord, const string& targetWord, vector<string>& activePath) {
        if (currentWord == targetWord) {
            allShortestPaths.push_back(activePath);
            return;
        }

        // If the current word has no valid outgoing shortest path connections, return early
        if (adjList.find(currentWord) == adjList.end()) return;

        for (const string& nextWord : adjList[currentWord]) {
            activePath.push_back(nextWord);
            backtrackPathGraph(nextWord, targetWord, activePath);
            activePath.pop_back(); // Backtrack step
        }
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: BFS LAYER-BY-LAYER DIRECTED GRAPH BACKTRACKING
     * -------------------------------------------------------------------------------------
     * The objective is to identify ALL shortest transformation sequences from `beginWord` 
     * to `endWord`, where each adjacent word differs by exactly one character.
     * 
     * STRATEGY:
     * 1. A standard BFS storing the whole path at each node incurs massive memory overhead due to copying.
     * 2. Instead, we use a two-phase engine:
     *    - Phase A: A Layer-by-Layer BFS to find the absolute shortest distance from the start word 
     *      to all other reachable words. We dynamically build a directed graph (`adjList`) storing 
     *      only edges that form the optimal shortest paths.
     *    - Phase B: A precise DFS backtracking pass starting from `beginWord` down the `adjList` 
     *      to compile the actual path arrays, guaranteeing zero redundant paths are explored.
     */
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        // Convert the word list into a hash set for O(1) existence validation checks
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        
        // Base Boundary Condition: If the target endWord does not exist in the dictionary, no path is possible
        if (wordSet.find(endWord) == wordSet.end()) {
            return {};
        }

        // Clear local storage fields to ensure fresh tracking state parameters
        adjList.clear();
        shortestDistances.clear();
        allShortestPaths.clear();

        // Phase A: Initialize layer-by-layer BFS queuing structures
        queue<string> bfsQueue;
        bfsQueue.push(beginWord);
        shortestDistances[beginWord] = 0;

        bool targetReachedAtCurrentLevel = false;

        while (!bfsQueue.empty()) {
            size_t currentLevelSize = bfsQueue.size();
            
            // If the target word was discovered in the previous layer, terminate grid expansion early
            if (targetReachedAtCurrentLevel) break;

            for (size_t i = 0; i < currentLevelSize; ++i) {
                string currentWord = std::move(bfsQueue.front());
                bfsQueue.pop();

                int currentLayerDepth = shortestDistances[currentWord];
                string dynamicMutationWord = currentWord;

                // Mutate each character position systematically from 'a' through 'z'
                for (size_t pos = 0; pos < dynamicMutationWord.length(); ++pos) {
                    char originalChar = dynamicMutationWord[pos];

                    for (char replacementChar = 'a'; replacementChar <= 'z'; ++replacementChar) {
                        if (replacementChar == originalChar) continue;

                        dynamicMutationWord[pos] = replacementChar;

                        // Check if the mutated token string is an active part of our dictionary space
                        if (wordSet.find(dynamicMutationWord) != wordSet.end()) {
                            
                            // Scenario 1: First time discovering this word profile path
                            if (shortestDistances.find(dynamicMutationWord) == shortestDistances.end()) {
                                shortestDistances[dynamicMutationWord] = currentLayerDepth + 1;
                                bfsQueue.push(dynamicMutationWord);
                                adjList[currentWord].push_back(dynamicMutationWord);

                                if (dynamicMutationWord == endWord) {
                                    targetReachedAtCurrentLevel = true;
                                }
                            } 
                            // Scenario 2: Word already visited, check if it fits into a parallel shortest path layer
                            else if (shortestDistances[dynamicMutationWord] == currentLayerDepth + 1) {
                                adjList[currentWord].push_back(dynamicMutationWord);
                            }
                        }
                    }
                    // Restore original character frame state before moving to the next position index
                    dynamicMutationWord[pos] = originalChar;
                }
            }
        }

        // Phase B: If a path matrix was verified, execute recursive tracking vectors
        if (shortestDistances.find(endWord) != shortestDistances.end()) {
            vector<string> activeTrackingVector = {beginWord};
            backtrackPathGraph(beginWord, endWord, activeTrackingVector);
        }

        return allShortestPaths;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * 26 * L + V + E) - Where N represents the total words count inside wordList, 
 *                  and L represents the fixed character length of individual words. The mutation pass 
 *                  explores paths across the graph topology in structural linear tiers.
 *
 * SPACE COMPLEXITY: O(N * L + E) Auxiliary - Allocated matrix tracking storage scales with total 
 *                   dictionary words and the number of valid intermediate edge transitions.
 */
int main() {
    string sourceWord, destinationWord;
    int wordsListCount;

    cout << "=== Word Ladder II Layered Graph Backtracking Console ===\n";
    cout << "Enter the initial beginning word: ";
    if (!(cin >> sourceWord)) return 1;
    cout << "Enter the target destination word: ";
    if (!(cin >> destinationWord)) return 1;

    cout << "Enter the total word count inside your dictionary list: ";
    if (!(cin >> wordsListCount) || wordsListCount <= 0) {
        cout << "Invalid allocation metrics input." << endl;
        return 1;
    }

    vector<string> customDictionary(static_cast<size_t>(wordsListCount));
    cout << "Enter the individual dictionary words sequentially:\n";
    for (int i = 0; i < wordsListCount; ++i) {
        cin >> customDictionary[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nBuilding layered graph maps and processing shortest word tracks..." << endl;
    vector<vector<string>> resultingPathsMatrix = solver.findLadders(sourceWord, destinationWord, customDictionary);

    cout << "\nTotal Shortest Path Inversion Sets Extracted: " << resultingPathsMatrix.size() << "\n";
    for (size_t idx = 0; idx < resultingPathsMatrix.size(); ++idx) {
        cout << "Path [" << idx + 1 << "]: ";
        for (size_t step = 0; step < resultingPathsMatrix[idx].size(); ++step) {
            cout << resultingPathsMatrix[idx][step] << (step < resultingPathsMatrix[idx].size() - 1 ? " -> " : "");
        }
        cout << "\n";
    }

    return 0;
}