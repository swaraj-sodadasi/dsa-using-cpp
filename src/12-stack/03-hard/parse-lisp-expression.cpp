#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

using namespace std;

class Solution {
private:
    /*
     * -------------------------------------------------------------------------------------
     * SYSTEM DATA REPRESENTATION: NESTED SCOPE MAPPING DICTIONARY ARCHITECTURE
     * -------------------------------------------------------------------------------------
     * Scoping is handled by a vector of hash maps representing layered runtime execution frames.
     */
    vector<unordered_map<string, int>> scopeStack;

    int evaluateToken(const string& token) {
        if (token.empty()) return 0;
        if (isdigit(token[0]) || token[0] == '-') {
            return stoi(token);
        }
        for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
            if (it->count(token)) {
                return (*it)[token];
            }
        }
        return 0;
    }

    // Safely reads a single discrete structural token sequence block
    string readNextToken(const string& expression, int& index) {
        int length = static_cast<int>(expression.length());
        while (index < length && expression[static_cast<size_t>(index)] == ' ') {
            index++;
        }
        if (index >= length) return "";
        
        if (expression[static_cast<size_t>(index)] == '(' || expression[static_cast<size_t>(index)] == ')') {
            string token(1, expression[static_cast<size_t>(index)]);
            index++;
            return token;
        }

        string token = "";
        while (index < length && expression[static_cast<size_t>(index)] != ' ' && 
               expression[static_cast<size_t>(index)] != '(' && expression[static_cast<size_t>(index)] != ')') {
            token += expression[static_cast<size_t>(index)];
            index++;
        }
        return token;
    }

    int parseInternal(const string& expression, int& index) {
        // Read the starting character command segment marker
        string starter = readNextToken(expression, index); // Should always resolve to "("
        
        scopeStack.push_back(unordered_map<string, int>());
        string operationType = readNextToken(expression, index);
        int evaluationOutcome = 0;

        if (operationType == "let") {
            while (true) {
                // Peek ahead to detect deep subsegment assignments or values
                int peekIndex = index;
                string nextToken = readNextToken(expression, peekIndex);

                if (nextToken == "(") {
                    // Trailing evaluation node is a complex child sub-expression block
                    evaluationOutcome = parseInternal(expression, index);
                    readNextToken(expression, index); // Absorb the closing ')' of the 'let'
                    break;
                } else if (nextToken == ")") {
                    // Syntax violation safety recovery fallback
                    readNextToken(expression, index);
                    break;
                }

                // Look past this token to confirm if it represents a variable mapping or a final variable lookup
                string targetVar = readNextToken(expression, index);
                int checkIndex = index;
                string valueToken = readNextToken(expression, checkIndex);

                if (valueToken == ")") {
                    // Final expression execution path; evaluate the literal target token variable
                    evaluationOutcome = evaluateToken(targetVar);
                    readNextToken(expression, index); // Absorb the closing ')' of the 'let'
                    break;
                }

                // Process standard assigned variable sequences
                int assignedValue = 0;
                if (valueToken == "(") {
                    assignedValue = parseInternal(expression, index);
                } else {
                    assignedValue = evaluateToken(valueToken);
                    index = checkIndex; // Advance the pointer context past the parsed literal value token
                }

                scopeStack.back()[targetVar] = assignedValue;
            }
        } else if (operationType == "add" || operationType == "mult") {
            int parameters[2] = {0, 0};
            for (int i = 0; i < 2; ++i) {
                int peekIndex = index;
                string token = readNextToken(expression, peekIndex);
                if (token == "(") {
                    parameters[i] = parseInternal(expression, index);
                } else {
                    parameters[i] = evaluateToken(token);
                    index = peekIndex;
                }
            }
            evaluationOutcome = (operationType == "add") ? (parameters[0] + parameters[1]) : (parameters[0] * parameters[1]);
            readNextToken(expression, index); // Absorb the closing ')' of the operation block
        }

        scopeStack.pop_back(); // Teardown the active local scope directory layer
        return evaluationOutcome;
    }

public:
    int evaluate(string expression) {
        scopeStack.clear();
        int initialTraversalIndex = 0;
        return parseInternal(expression, initialTraversalIndex);
    }
};

int main() {
    cout << "=== Parse Lisp Expression Scope Mapping Dictionary Stack Engine ===\n";
    cout << "Enter the fully balanced Lisp command sequence matrix:\n";
    
    string expressionsInput;
    if (!getline(cin, expressionsInput) || expressionsInput.empty()) return 0;

    Solution solver;
    cout << "\nCompiling scope memory records and running recursive expression analysis..." << endl;
    int systemOutput = solver.evaluate(expressionsInput);

    cout << "Final Computed Evaluation Balance Output: [" << systemOutput << "]\n";

    return 0;
}