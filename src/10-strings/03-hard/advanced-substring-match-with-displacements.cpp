#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

class AhoCorasickDisplacements {
private:
    /*
     * The Aho-Corasick Automaton tracks transition states using an internal Trie structure 
     * expanded with automated failure links and character index displacement maps.
     */
    struct TrieNode {
        // Explicit transition mapping from character key to downstream state indices
        unordered_map<char, int> transitions;
        
        // Failure link pointer tracking the longest proper suffix state configuration
        int failureLink = 0;
        
        // Dictionary suffix link pointing to the nearest downstream match state node
        int dictSuffixLink = -1;
        
        // Keeps indices of all strings whose literal matching parameters terminate here
        vector<int> matchedStringIndices;
    };

    // Global state registry array tracking allocated node configurations
    vector<TrieNode> stateMachine;

public:
    /*
     * CONSTRUCTOR
     * Initializes the baseline matching architecture with a primary Root node frame at state 0.
     */
    AhoCorasickDisplacements() {
        stateMachine.emplace_back(); // Initialize Root Node at State Index 0
    }

    /*
     * TRIE STRING INSERTION
     * Sequentially feeds a key word into the trie state matrix structure.
     */
    void insertPattern(const string& pattern, int patternIndex) {
        int currentState = 0;
        for (char c : pattern) {
            // If the transition branch doesn't exist, create a new node tracking profile
            if (stateMachine[static_cast<size_t>(currentState)].transitions.find(c) == 
                stateMachine[static_cast<size_t>(currentState)].transitions.end()) {
                
                int nextStateIndex = static_cast<int>(stateMachine.size());
                stateMachine[static_cast<size_t>(currentState)].transitions[c] = nextStateIndex;
                stateMachine.emplace_back();
            }
            currentState = stateMachine[static_cast<size_t>(currentState)].transitions[c];
        }
        // Affix pattern termination ID metrics onto the leaf node structure
        stateMachine[static_cast<size_t>(currentState)].matchedStringIndices.push_back(patternIndex);
    }

    /*
     * AUTOMATED STATE SPACE COMPILATION (BFS FAILURE & DICTIONARY LINK CONSTRUCTION)
     * Compiles the complete state graph paths by mapping out failure and dictionary lines 
     * via level-order Breadth-First search queue sweeps.
     */
    void buildAutomaton() {
        queue<int> bfsQueue;
        int rootState = 0;

        // Step 1: Initialize Level 1 child transitions bypassing the root state failure loops
        for (auto const& [ch, childState] : stateMachine[static_cast<size_t>(rootState)].transitions) {
            stateMachine[static_cast<size_t>(childState)].failureLink = rootState;
            stateMachine[static_cast<size_t>(childState)].dictSuffixLink = -1;
            bfsQueue.push(childState);
        }

        // Step 2: Propagate failure metrics down remaining deep path combinations
        while (!bfsQueue.empty()) {
            int parentState = bfsQueue.front();
            bfsQueue.pop();

            for (auto const& [ch, childState] : stateMachine[static_cast<size_t>(parentState)].transitions) {
                int failureTrace = stateMachine[static_cast<size_t>(parentState)].failureLink;

                // Trace back along failure links until a valid fallback node is discovered
                while (failureTrace != rootState && 
                       stateMachine[static_cast<size_t>(failureTrace)].transitions.find(ch) == 
                       stateMachine[static_cast<size_t>(failureTrace)].transitions.end()) {
                    failureTrace = stateMachine[static_cast<size_t>(failureTrace)].failureLink;
                }

                // If found, update the target failure link state mapping to the matched node index
                if (stateMachine[static_cast<size_t>(failureTrace)].transitions.find(ch) != 
                    stateMachine[static_cast<size_t>(failureTrace)].transitions.end()) {
                    stateMachine[static_cast<size_t>(childState)].failureLink = 
                        stateMachine[static_cast<size_t>(failureTrace)].transitions[ch];
                } else {
                    stateMachine[static_cast<size_t>(childState)].failureLink = rootState;
                }

                // Compress output paths via dictionary suffix links to fetch matching steps in O(1)
                int failNode = stateMachine[static_cast<size_t>(childState)].failureLink;
                if (!stateMachine[static_cast<size_t>(failNode)].matchedStringIndices.empty()) {
                    stateMachine[static_cast<size_t>(childState)].dictSuffixLink = failNode;
                } else {
                    stateMachine[static_cast<size_t>(childState)].dictSuffixLink = 
                        stateMachine[static_cast<size_t>(failNode)].dictSuffixLink;
                }

                bfsQueue.push(childState);
            }
        }
    }

    /*
     * DISPLACEMENT MATCH SEARCH PIPELINE
     * Searches through the text stream to find keywords. Employs offset subtraction maps 
     * to accurately output match indices adjusted back to the pattern's absolute starting point.
     */
    void searchInText(const string& text, const vector<string>& patterns) {
        int currentState = 0;
        int rootState = 0;

        cout << "\nParsing character text sequences through automated state states...\n";

        for (int i = 0; i < static_cast<int>(text.length()); ++i) {
            char ch = text[static_cast<size_t>(i)];

            // Handle path mismatches by tracking back through the failure links
            while (currentState != rootState && 
                   stateMachine[static_cast<size_t>(currentState)].transitions.find(ch) == 
                   stateMachine[static_cast<size_t>(currentState)].transitions.end()) {
                currentState = stateMachine[static_cast<size_t>(currentState)].failureLink;
            }

            // Transition to the next valid state if an edge exists
            if (stateMachine[static_cast<size_t>(currentState)].transitions.find(ch) != 
                stateMachine[static_cast<size_t>(currentState)].transitions.end()) {
                currentState = stateMachine[static_cast<size_t>(currentState)].transitions[ch];
            }

            // Internal evaluation lambda to process pattern index displacements
            auto processMatches = [&](int stateIndex) {
                for (int patternIdx : stateMachine[static_cast<size_t>(stateIndex)].matchedStringIndices) {
                    int matchedPatternLength = static_cast<int>(patterns[static_cast<size_t>(patternIdx)].length());
                    
                    // DISPLACEMENT ARITHMETIC: Convert the current cursor end index back to the starting offset
                    int startingDisplacementIndex = i - matchedPatternLength + 1;
                    
                    cout << "  Pattern Match Found: \"" << patterns[static_cast<size_t>(patternIdx)] 
                         << "\" | Absolute String Displacement Index: " << startingDisplacementIndex << "\n";
                }
            };

            // Evaluate matches directly linked to the current terminal node state
            if (!stateMachine[static_cast<size_t>(currentState)].matchedStringIndices.empty()) {
                processMatches(currentState);
            }

            // Traverse the dictionary suffix link chains to catch any nested patterns ending here
            int dictionaryTraceNode = stateMachine[static_cast<size_t>(currentState)].dictSuffixLink;
            while (dictionaryTraceNode != -1) {
                processMatches(dictionaryTraceNode);
                dictionaryTraceNode = stateMachine[static_cast<size_t>(dictionaryTraceNode)].dictSuffixLink;
            }
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + M + Z) - Where N represents the total length of the target search text, 
 *                  M represents the sum of lengths of all patterns to be matched, and Z is the count 
 *                  of matching occurrences. Trie instantiation runs in O(M), failure graph generation 
 *                  runs in O(M), and text stream analysis scales smoothly in linear O(N + Z) steps.
 *
 * SPACE COMPLEXITY: O(M * K) Auxiliary - Graph mapping space expands linearly based on the cumulative 
 *                   character metrics (M) of configured keyword configurations across the alphabet size (K).
 */
int main() {
    int patternsCount;
    cout << "=== Aho-Corasick Automated State Space Processing Console ===\n";
    cout << "Enter the total number of dictionary patterns to load: ";
    if (!(cin >> patternsCount) || patternsCount <= 0) {
        cout << "Invalid structural size allocation parameter inputs." << endl;
        return 1;
    }

    vector<string> patternsList(static_cast<size_t>(patternsCount));
    AhoCorasickDisplacements matcherEngine;

    cout << "Enter the distinct pattern text phrases sequentially below:\n";
    for (int i = 0; i < patternsCount; ++i) {
        cin >> patternsList[static_cast<size_t>(i)];
        matcherEngine.insertPattern(patternsList[static_cast<size_t>(i)], i);
    }

    // Compile state vectors and populate structural optimization links
    matcherEngine.buildAutomaton();

    string textSourceInput;
    cout << "\nEnter the target text stream body to search against:\n";
    cin >> textSourceInput;

    // Execute matching sweeps across text streams using displacement parameters
    matcherEngine.searchInText(textSourceInput, patternsList);

    return 0;
}