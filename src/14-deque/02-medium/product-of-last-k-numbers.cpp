#include <iostream>
#include <vector>

using namespace std;

class ProductOfNumbers {
private:
    vector<int> prefixProducts;

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: PRODUCT OF THE LAST K NUMBERS (PREFIX PRODUCT ARRAY STREAM OPTIMIZER)
     * -------------------------------------------------------------------------------------
     * The objective is to calculate the product of the last k numbers added to a stream 
     * in O(1) time complexity.
     * 
     * 1. PREFIX PRODUCT STREAM DECOMPOSITION:
     *    We maintain a running prefix product vector. `prefixProducts[i]` holds the product 
     *    of all non-zero numbers since the last encountered 0.
     * 
     * 2. ZERO RESET MECHANISM:
     *    - When a 0 is added, any product including this 0 will result in 0.
     *    - We clear the entire `prefixProducts` array and re-initialize it with a base value 
     *      of 1.
     *    - Any future query requesting `k` numbers that spans past this reset point immediately 
     *      returns 0 because a 0 must have been part of those last `k` numbers.
     * 
     * 3. O(1) DIVISION QUERY:
     *    - If `k < prefixProducts.size()`, the product of the last `k` numbers is computed as:
     *      `prefixProducts.back() / prefixProducts[prefixProducts.size() - 1 - k]`
     *    - If `k >= prefixProducts.size()`, it means a 0 was encountered within the last `k` 
     *      numbers, so the product is 0.
     */
    ProductOfNumbers() {
        prefixProducts = {1};
    }

    void add(int num) {
        if (num == 0) {
            prefixProducts = {1};
        } else {
            prefixProducts.push_back(prefixProducts.back() * num);
        }
    }

    int getProduct(int k) const {
        int n = static_cast<int>(prefixProducts.size());
        if (k >= n) {
            return 0;
        }
        return prefixProducts.back() / prefixProducts[static_cast<size_t>(n - 1 - k)];
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) time complexity for both `add(num)` and `getProduct(k)` operations.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - Where N is the number of elements added to the stream.
 */
int main() {
    cout << "=== Product of the Last K Numbers Stream Engine ===\n";
    ProductOfNumbers stream;

    cout << "Enter total number of dynamic operations to perform: ";
    int numOps;
    if (!(cin >> numOps) || numOps <= 0) return 1;

    cout << "\nOperations format:\n";
    cout << "  1 [val] : Add value to stream\n";
    cout << "  2 [k]   : Get product of last k numbers\n\n";

    for (int i = 0; i < numOps; ++i) {
        cout << "Operation [" << i + 1 << "/" << numOps << "]: ";
        int op;
        cin >> op;

        if (op == 1) {
            int val;
            cin >> val;
            stream.add(val);
            cout << "  -> Added: " << val << "\n";
        } else if (op == 2) {
            int k;
            cin >> k;
            int prod = stream.getProduct(k);
            cout << "  -> Product of last " << k << " numbers: " << prod << "\n";
        } else {
            cout << "  -> Invalid operation option.\n";
        }
    }

    return 0;
}