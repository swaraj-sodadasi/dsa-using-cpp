#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * DIVIDE AND CONQUER LINEAR TRANSFORMATION CONSTRUCTION ENGINE
     * ------------------------------------------------------------
     * An array `nums` of length N is considered "beautiful" if it is a permutation of 
     * integers from 1 to N such that for every pair of indices i < j, there does NOT exist 
     * an index k (with i < k < j) where: 2 * nums[k] == nums[i] + nums[j].
     *
     * Mathematical & Linearity Invariance Properties:
     * - The condition `2 * nums[k] == nums[i] + nums[j]` can never be satisfied if `nums[i]` 
     *   and `nums[j]` have different parities (one is odd, the other is even), because 
     *   an odd number plus an even number produces an odd result, which cannot equal `2 * nums[k]`.
     * - Linearity Property: If an array `A` is beautiful, then any linear transformation 
     *   `f(A) = c1 * A + c2` remains beautifully preserved. Specifically:
     *     - Odd Transformation: `2 * x - 1` maps a beautiful array to completely odd numbers.
     *     - Even Transformation: `2 * x` maps a beautiful array to completely even numbers.
     */
    vector<int> beautifulArray(int n) {
        vector<int> beautifulRegistry = {1};

        // Grow the beautiful seed array structure using parity-split linear scaling
        while (beautifulRegistry.size() < static_cast<size_t>(n)) {
            vector<int> scaledBuffer;
            scaledBuffer.reserve(beautifulRegistry.size() * 2);

            // Step 1: Generate the odd parity linear map section (2*x - 1)
            for (int x : beautifulRegistry) {
                if (x * 2 - 1 <= n) {
                    scaledBuffer.push_back(x * 2 - 1);
                }
            }

            // Step 2: Generate the even parity linear map section (2*x)
            for (int x : beautifulRegistry) {
                if (x * 2 <= n) {
                    scaledBuffer.push_back(x * 2);
                }
            }

            // Explicitly qualified std::move prevents ADL injection risks and naming collisions
            beautifulRegistry = std::move(scaledBuffer);
        }

        return beautifulRegistry;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - The size of the array roughly doubles at each iteration level, 
 *                  requiring log(N) total scaling steps. Each tier performs a linear filter 
 *                  pass bounded by O(N) operations.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The generation engine requires temporary layout vectors 
 *                   proportional to N to assemble structural configurations across parity boundaries.
 */
int main() {
    int n;
    cout << "Enter the targeted permutation array limit size (N): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter placement setup configuration bounds." << endl;
        return 1;
    }

    Solution solver;
    cout << "\nExecuting divide and conquer linear transformation loop construction..." << endl;
    vector<int> result = solver.beautifulArray(n);

    cout << "\nGenerated Beautiful Permutation Layout:\n[ ";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? ", " : "");
    }
    cout << " ]\n";

    // Verification check step confirming structural invariant properties
    bool isVerifiedBeautiful = true;
    for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = i + 2; j < result.size(); ++j) {
            for (size_t k = i + 1; k < j; ++k) {
                if (2 * result[k] == result[i] + result[j]) {
                    isVerifiedBeautiful = false;
                }
            }
        }
    }

    cout << "Arithmetic progression constraint validation: " 
         << (isVerifiedBeautiful ? "PASSED (No invalid triplets exist)" : "FAILED") << endl;

    return 0;
}