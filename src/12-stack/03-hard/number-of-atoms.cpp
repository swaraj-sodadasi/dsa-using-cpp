#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CHEMICAL FORMULA TOKEN PARSING HASH MAPS STACKED WITH PARENTHESES LAYERS
     * -------------------------------------------------------------------------------------
     * The objective is to count the occurrences of each atom type inside a chemical formula
     * string and output a consolidated sorted profile string.
     * 
     * 1. STACKED HASH MAP SCOPES ARCHITECTURE:
     *    Parentheses alter multiplication factors across nested compound blocks. We maintain a
     *    stack of ordered maps (`std::map`), where each layer tracks localized atom-to-count registers.
     *    An ordered map ensures atom names automatically sort lexicographically.
     * 
     * 2. STATE TRANSITION RULES:
     *    We parse the formula string from left to right using an index cursor `i`:
     *      - `(`   : Push a fresh, blank count map onto the scope stack to isolate the sub-formula.
     *      - `)`   : Parse the trailing multiplier integer directly following the bracket. Pop the 
     *                top layer map, scale all its element counts by the multiplier, and merge them
     *                into the parent scope map layer below.
     *      - Atom  : Parse the element symbol (an uppercase letter followed optionally by lowercase
     *                letters), then parse its optional trailing quantity count integer (defaults to 1).
     *                Add the quantity to the map layer currently at the top of the stack.
     */
    string countOfAtoms(string formula) {
        int n = static_cast<int>(formula.length());
        vector<map<string, int>> scopeStack;
        scopeStack.push_back(map<string, int>()); // Base context layer
        
        int i = 0;
        while (i < n) {
            if (formula[static_cast<size_t>(i)] == '(') {
                scopeStack.push_back(map<string, int>());
                i++;
            } else if (formula[static_cast<size_t>(i)] == ')') {
                i++;
                // Parse the optional numeric multiplier that applies to the entire parenthesized block
                long long multiplier = 0;
                while (i < n && isdigit(formula[static_cast<size_t>(i)])) {
                    multiplier = multiplier * 10 + (formula[static_cast<size_t>(i)] - '0');
                    i++;
                }
                if (multiplier == 0) multiplier = 1; // Default multiplier is 1

                map<string, int> completedScope = scopeStack.back();
                scopeStack.pop_back();

                // Multiply inner contents and accumulate into the underlying parent scope layer
                for (const auto& [atomName, count] : completedScope) {
                    scopeStack.back()[atomName] += count * static_cast<int>(multiplier);
                }
            } else {
                // Parse an element symbol name (e.g., 'Mg', 'H')
                string atomName = "";
                atomName += formula[static_cast<size_t>(i)];
                i++;
                while (i < n && islower(formula[static_cast<size_t>(i)])) {
                    atomName += formula[static_cast<size_t>(i)];
                    i++;
                }

                // Parse the optional numeric atom quantity count parameter
                long long atomCount = 0;
                while (i < n && isdigit(formula[static_cast<size_t>(i)])) {
                    atomCount = atomCount * 10 + (formula[static_cast<size_t>(i)] - '0');
                    i++;
                }
                if (atomCount == 0) atomCount = 1; // Default count is 1

                scopeStack.back()[atomName] += static_cast<int>(atomCount);
            }
        }

        // Build the final lexicographically ordered text signature outcome matrix
        string balancedOutcome = "";
        for (const auto& [atomName, totalCount] : scopeStack.back()) {
            balancedOutcome += atomName;
            if (totalCount > 1) {
                balancedOutcome += to_string(totalCount);
            }
        }

        return balancedOutcome;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N * N) Worst-case / O(N log K) Avg - Where N represents formula length, and
 *                  K is the unique atom count. Sorting and map merging scale with depth loops.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Stack structures maintain discrete scope map frames tracking
 *                   unique subsegments across deep nesting layouts (e.g., K4(ON(SO3)2)2).
 */
int main() {
    cout << "=== Chemical Formula Token Parsing Stack Engine ===\n";
    cout << "Enter the chemical formula string matrix (e.g., Mg(OH)2 or K4(ON(SO3)2)2): ";
    
    string formulaInput;
    if (!(cin >> formulaInput)) return 1;

    Solution solver;
    cout << "Parsing element symbols and liquidating nested brackets..." << endl;
    string computedConsolidatedFormula = solver.countOfAtoms(formulaInput);

    cout << "Consolidated Atom Count Outcome: [" << computedConsolidatedFormula << "]\n";

    return 0;
}