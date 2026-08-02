#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * BUCKET SORT FREQUENCY DISTRIBUTION MAPPING ENGINE
     * -------------------------------------------------
     * The objective is to compute a researcher's H-Index, defined as the maximum value 
     * 'h' such that the researcher has published at least 'h' papers that have each 
     * been cited at least 'h' times.
     *
     * Architectural Tracking Mechanics:
     * - A naive approach involves sorting the citations array in descending order, taking O(N log N) time.
     * - We optimize this to O(N) linear time using a modified Bucket Sort approach. Since an H-Index 
     *   for 'N' papers can never exceed 'N', any citation count greater than 'N' can be safely grouped 
     *   and treated exactly as if it were equal to 'N'.
     *
     * Distribution Mapping Sequence:
     * - Step 1: Create a frequency array (`citationBuckets`) of size `N + 1`. The index represents 
     *   the citation count, and the value stores the number of papers with that exact citation count.
     * - Step 2: Traverse the original `citations` array. If a paper's citation count is greater than 
     *   or equal to `N`, increment the bucket at index `N`. Otherwise, increment the bucket at index `citations[i]`.
     * - Step 3: Iterate backward through the buckets from index `N` down to 0, maintaining a running sum 
     *   of papers (`accumulatedPapersCount`). The first index `h` where `accumulatedPapersCount >= h` is 
     *   mathematically guaranteed to be the correct H-Index.
     */
    int hIndex(vector<int>& citations) {
        size_t n = citations.size();
        if (n == 0) return 0;

        // Allocate N + 1 buckets initialized to 0 to record citation distributions
        vector<int> citationBuckets(n + 1, 0);

        // Step 2: Populate frequency distributions into the bucket system
        for (int citation : citations) {
            if (static_cast<size_t>(citation) >= n) {
                citationBuckets[n]++;
            } else {
                citationBuckets[static_cast<size_t>(citation)]++;
            }
        }

        int accumulatedPapersCount = 0;

        // Step 3: Accumulate papers backward from the maximum possible h-index boundary
        for (int h = static_cast<int>(n); h >= 0; --h) {
            accumulatedPapersCount += citationBuckets[static_cast<size_t>(h)];

            // The moment total papers meet or exceed the active h tier, the boundary condition is fulfilled
            if (accumulatedPapersCount >= h) {
                return h;
            }
        }

        return 0;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the total number of papers in the dataset.
 *                  The logic completes within exactly two independent linear sweeps 
 *                  (one to populate the buckets, and one backward pass to compute the H-Index).
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization engine allocates a temporary frequency mapping vector 
 *                   of size N + 1 to keep count of paper citation categories.
 */
int main() {
    int n;
    cout << "Enter the total number of papers published: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. The paper pool count must be greater than 0." << endl;
        return 1;
    }

    vector<int> citations(static_cast<size_t>(n));
    cout << "Enter citation numbers for each paper separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> citations[static_cast<size_t>(i)];
        if (citations[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Citation values cannot be negative numbers." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting bucket sort frequency distribution mapping..." << endl;
    int result = solver.hIndex(citations);

    cout << "Calculated researcher H-Index output value: " << result << endl;

    return 0;
}