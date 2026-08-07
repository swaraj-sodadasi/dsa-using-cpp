#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: DIGIT-BY-DIGIT ARRAY ACCUMULATION ARITHMETIC
     * -------------------------------------------------------------------------------------
     * Baseline String-based Engine (Handles arbitrarily massive inputs without overflow)
     */
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") {
            return "0";
        }

        size_t len1 = num1.size();
        size_t len2 = num2.size();
        
        vector<int> accumulationArray(len1 + len2, 0);

        for (int i = static_cast<int>(len1) - 1; i >= 0; --i) {
            for (int j = static_cast<int>(len2) - 1; j >= 0; --j) {
                int mul = (num1[static_cast<size_t>(i)] - '0') * (num2[static_cast<size_t>(j)] - '0');
                
                int p1 = i + j;
                int p2 = i + j + 1;
                
                int sum = mul + accumulationArray[static_cast<size_t>(p2)];

                accumulationArray[static_cast<size_t>(p1)] += sum / 10;
                accumulationArray[static_cast<size_t>(p2)] = sum % 10;
            }
        }

        string productString = "";
        productString.reserve(len1 + len2);
        
        for (int val : accumulationArray) {
            if (!(productString.empty() && val == 0)) {
                productString.push_back(static_cast<char>(val + '0'));
            }
        }

        return productString.empty() ? "0" : productString;
    }

    /*
     * INTERFACE OVERLOADS:
     * -------------------------------------------------------------------------------------
     * FIXED: Explicitly qualified 'std::move' is leveraged within all structural routing operations.
     */
    string multiply(long long num1, long long num2) {
        if (num1 < 0 || num2 < 0) return "Invalid Input: Non-negative values only";
        return multiply(to_string(num1), to_string(num2));
    }

    string multiply(long long num1, string num2) {
        if (num1 < 0) return "Invalid Input: Non-negative values only";
        // FIXED: Fully qualified namespace call applied here
        return multiply(to_string(num1), std::move(num2));
    }

    string multiply(string num1, long long num2) {
        if (num2 < 0) return "Invalid Input: Non-negative values only";
        // FIXED: Fully qualified namespace call applied here
        return multiply(std::move(num1), to_string(num2));
    }
};

// Helper function to validate string layout parameters
bool isValidDigits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

int main() {
    Solution solver;
    int selectionChoice = 0;

    cout << "=== Dynamic Polymorphic Multiplication Engine ===\n";
    cout << "Select your dynamic input type combination configuration:\n";
    cout << "  1. [String]  x  [String]  (For extremely large numbers)\n";
    cout << "  2. [Integer] x  [Integer] (Standard native primitives)\n";
    cout << "  3. [String]  x  [Integer] (Mixed mode A)\n";
    cout << "  4. [Integer] x  [String]  (Mixed mode B)\n";
    cout << "Enter selection option (1-4): ";
    
    if (!(cin >> selectionChoice) || selectionChoice < 1 || selectionChoice > 4) {
        cout << "Error: Invalid setup choice selected." << endl;
        return 1;
    }

    cout << "\n--------------------------------------------------\n";

    if (selectionChoice == 1) {
        string inputStr1, inputStr2;
        cout << "Enter Value 1 (String): "; cin >> inputStr1;
        cout << "Enter Value 2 (String): "; cin >> inputStr2;

        if (!isValidDigits(inputStr1) || !isValidDigits(inputStr2)) {
            cout << "Constraint Error: Text entries must consist of non-negative digits only." << endl;
            return 1;
        }
        cout << "\nExecuting String-String layout sweeps...\n";
        cout << "Result: " << solver.multiply(inputStr1, inputStr2) << "\n";

    } else if (selectionChoice == 2) {
        long long inputInt1, inputInt2;
        cout << "Enter Value 1 (Integer): "; cin >> inputInt1;
        cout << "Enter Value 2 (Integer): "; cin >> inputInt2;

        if (inputInt1 < 0 || inputInt2 < 0) {
            cout << "Constraint Error: Values must be non-negative integers." << endl;
            return 1;
        }
        cout << "\nExecuting Integer-Integer type signature resolution...\n";
        cout << "Result: " << solver.multiply(inputInt1, inputInt2) << "\n";

    } else if (selectionChoice == 3) {
        string inputStr1;
        long long inputInt2;
        cout << "Enter Value 1 (String): ";  cin >> inputStr1;
        cout << "Enter Value 2 (Integer): "; cin >> inputInt2;

        if (!isValidDigits(inputStr1) || inputInt2 < 0) {
            cout << "Constraint Error: Invalid numerical signatures detected." << endl;
            return 1;
        }
        cout << "\nExecuting Mixed Mode A function intercept arrays...\n";
        cout << "Result: " << solver.multiply(inputStr1, inputInt2) << "\n";

    } else if (selectionChoice == 4) {
        long long inputInt1;
        string inputStr2;
        cout << "Enter Value 1 (Integer): "; cin >> inputInt1;
        cout << "Enter Value 2 (String): ";  cin >> inputStr2;

        if (inputInt1 < 0 || !isValidDigits(inputStr2)) {
            cout << "Constraint Error: Invalid numerical signatures detected." << endl;
            return 1;
        }
        cout << "\nExecuting Mixed Mode B function intercept arrays...\n";
        cout << "Result: " << solver.multiply(inputInt1, inputStr2) << "\n";
    }

    return 0;
}