#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

/*
CORE LOGIC (DIVIDE AND CONQUER EXPRESSION PARSING):
- The objective is to compute all possible results from computing the given expression string after grouping numbers and operators with parentheses in every valid combination.
- **Divide and Conquer Framework**:
  - We can break the main expression down into smaller chunks by using the operators (`+`, `-`, `*`) as splitting anchors.
  - As we scan the string `expression` from left to right, whenever we hit an operator at index `i`:
    1. **Divide**: Split the string into two independent sub-expressions:
       - Left part: `expression.substr(0, i)`
       - Right part: `expression.substr(i + 1)`
    2. **Conquer**: Recursively call `diffWaysToCompute` on both parts. This returns a list of all possible computed values for the left side (`leftResults`) and right side (`rightResults`).
    3. **Combine**: Run a nested loop over every combination of values from `leftResults` and `rightResults`, evaluate them using the operator at index `i`, and save the combined results.
- **Base Case**:
  - If a sub-expression contains no operators, it is simply a pure number. We convert the string to an integer and return a list containing just that single value.

TIME COMPLEXITY: O(4^N / (N * sqrt(N))) worst-case - The total number of unique ways to parse and parenthesize the expression corresponds to the $N$-th Catalan Number, where N is the total number of operators present in the string.
SPACE COMPLEXITY: O(4^N / (N * sqrt(N))) auxiliary - Space allocated on the heap to collect intermediate computation list segments, alongside call stack frames bounded by $O(N)$.
*/

class Solution {
public:
    vector<int> diffWaysToCompute(string expression) {
        vector<int> results;
        bool hasOperator = false;

        // Scan the expression to look for operator splitting anchors
        for (size_t i = 0; i < expression.length(); ++i) {
            char c = expression[i];
            
            if (c == '+' || c == '-' || c == '*') {
                hasOperator = true;

                // Divide: Break down the expression into independent left and right sub-problems
                string leftSub = expression.substr(0, i);
                string rightSub = expression.substr(i + 1);

                // Conquer: Recursively solve both segments
                vector<int> leftResults = diffWaysToCompute(leftSub);
                vector<int> rightResults = diffWaysToCompute(rightSub);

                // Combine: Merge results across the active operator anchor
                for (int leftVal : leftResults) {
                    for (int rightVal : rightResults) {
                        if (c == '+') {
                            results.push_back(leftVal + rightVal);
                        } else if (c == '-') {
                            results.push_back(leftVal - rightVal);
                        } else if (c == '*') {
                            results.push_back(leftVal * rightVal);
                        }
                    }
                }
            }
        }

        // Base Case: If no operator is found, the sub-expression is a standalone integer literal
        if (!hasOperator) {
            results.push_back(stoi(expression));
        }

        return results;
    }
};

int main() {
    string expression;
    cout << "Enter the mathematical expression string (composed of digits and +, -, * only):\n";
    cout << "Example: 2*3-4*5\n";
    cout << "Input: ";
    if (!(cin >> expression)) {
        cout << "Invalid input expression reading parameters." << endl;
        return 1;
    }

    // Input verification check to filter out illegal structural tokens
    for (char c : expression) {
        if (!isdigit(c) && c != '+' && c != '-' && c != '*') {
            cout << "Constraint Error: Expression contains illegal characters. Only digits, '+', '-', and '*' are allowed." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting divide and conquer expression parsing..." << endl;
    vector<int> results = solver.diffWaysToCompute(expression);

    cout << "\nAll potential evaluation outcomes:\n[ ";
    for (size_t i = 0; i < results.size(); ++i) {
        cout << results[i];
        if (i + 1 < results.size()) cout << ", ";
    }
    cout << " ]\n";
    cout << "Total computation permutation variants found: " << results.size() << endl;

    return 0;
}