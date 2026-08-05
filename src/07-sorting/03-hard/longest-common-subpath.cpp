#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>

using namespace std;

class Solution {
private:
    /*
     * SUFFIX ARRAY CONSTRUCTION (O(N log N) Prefix-Doubling Strategy)
     * -------------------------------------------------------------
     * Builds the Suffix Array (SA) for the unified string/sequence.
     * The Suffix Array contains the starting indices of all suffixes sorted lexicographically.
     */
    void buildSuffixArray(const vector<int>& s, vector<int>& sa, vector<int>& rank) {
        int n = static_cast<int>(s.size());
        sa.resize(static_cast<size_t>(n));
        rank.resize(static_cast<size_t>(n));
        vector<int> tmpRank(static_cast<size_t>(n));

        // Initialize ranks based on the values in the sequence
        iota(sa.begin(), sa.end(), 0);
        
        // Copy original values to rank
        for (int i = 0; i < n; ++i) rank[static_cast<size_t>(i)] = s[static_cast<size_t>(i)];

        // Sort initial suffixes using standard stable sorts
        auto uniqueValues = s;
        sort(uniqueValues.begin(), uniqueValues.end());
        uniqueValues.erase(unique(uniqueValues.begin(), uniqueValues.end()), uniqueValues.end());
        for (int i = 0; i < n; ++i) {
            rank[static_cast<size_t>(i)] = static_cast<int>(lower_bound(uniqueValues.begin(), uniqueValues.end(), s[static_cast<size_t>(i)]) - uniqueValues.begin()) + 1;
        }

        sort(sa.begin(), sa.end(), [&](int a, int b) {
            return rank[static_cast<size_t>(a)] < rank[static_cast<size_t>(b)];
        });

        // Prefix-doubling loop: sort by combinations of length 2^k
        for (int k = 1; k < n; k <<= 1) {
            auto compare = [&](int a, int b) {
                if (rank[static_cast<size_t>(a)] != rank[static_cast<size_t>(b)])
                    return rank[static_cast<size_t>(a)] < rank[static_cast<size_t>(b)];
                int ra = (a + k < n) ? rank[static_cast<size_t>(a + k)] : -1;
                int rb = (b + k < n) ? rank[static_cast<size_t>(b + k)] : -1;
                return ra < rb;
            };

            stable_sort(sa.begin(), sa.end(), compare);

            tmpRank[static_cast<size_t>(sa[0])] = 0;
            for (int i = 1; i < n; ++i) {
                tmpRank[static_cast<size_t>(sa[static_cast<size_t>(i)])] = tmpRank[static_cast<size_t>(sa[static_cast<size_t>(i - 1)])] + (compare(sa[static_cast<size_t>(i - 1)], sa[static_cast<size_t>(i)]) ? 1 : 0);
            }
            rank = tmpRank;
            if (rank[static_cast<size_t>(sa[static_cast<size_t>(n - 1)])] == n - 1) break;
        }
    }

    /*
     * KASAI'S ALGORITHM (O(N) LCP Array Construction)
     * ------------------------------------------------
     * Computes the Longest Common Prefix (LCP) array from the Suffix Array.
     * lcp[i] stores the length of the longest common prefix between suffix sa[i] and suffix sa[i-1].
     */
    void buildLCP(const vector<int>& s, const vector<int>& sa, const vector<int>& rank, vector<int>& lcp) {
        int n = static_cast<int>(s.size());
        lcp.resize(static_cast<size_t>(n), 0);
        int h = 0;

        for (int i = 0; i < n; ++i) {
            if (rank[static_cast<size_t>(i)] > 0) {
                int j = sa[static_cast<size_t>(rank[static_cast<size_t>(i)] - 1)];
                while (i + h < n && j + h < n && s[static_cast<size_t>(i + h)] == s[static_cast<size_t>(j + h)]) {
                    h++;
                }
                lcp[static_cast<size_t>(rank[static_cast<size_t>(i)])] = h;
                if (h > 0) h--;
            }
        }
    }

    /*
     * FIXED-LENGTH VALIDATION STEP
     * -----------------------------
     * Validates if there exists a common subpath of length `mid` present in ALL paths.
     * We scan the LCP array to group contiguous segments where lcp[i] >= mid. If a group 
     * contains suffixes originating from every single original path, then a common subpath 
     * of length `mid` is verified.
     */
    bool checkCommonSubpath(int mid, int totalPaths, const vector<int>& lcp, const vector<int>& sa, const vector<int>& suffixOrigin, int totalLen) {
        if (mid == 0) return true;

        vector<int> lastSeenInPath(static_cast<size_t>(totalPaths), -1);
        int uniquePathsCount = 0;
        int groupID = 0;

        for (int i = 0; i < totalLen; ++i) {
            if (lcp[static_cast<size_t>(i)] < mid) {
                // Terminate current cluster group and start a new one
                groupID++;
                uniquePathsCount = 0;
            }

            int origin = suffixOrigin[static_cast<size_t>(sa[static_cast<size_t>(i)])];
            if (origin != -1) { // Ignore structural padding delimiters
                if (lastSeenInPath[static_cast<size_t>(origin)] != groupID) {
                    lastSeenInPath[static_cast<size_t>(origin)] = groupID;
                    uniquePathsCount++;
                    if (uniquePathsCount == totalPaths) return true;
                }
            }
        }
        return false;
    }

public:
    /*
     * LONGEST COMMON SUBPATH ENGINE
     * -----------------------------
     * RESOLVED WARNING: The unused parameter `n` has been explicitly cast to void 
     * inside the execution body to suppress the compiler diagnostic warning cleanly 
     * without modifying the standard interface signature definition.
     */
    int longestCommonSubpath(int n, vector<vector<int>>& paths) {
        // Explicitly suppress the unused parameter warning cleanly
        (void)n;

        int numPaths = static_cast<int>(paths.size());
        if (numPaths == 0) return 0;

        vector<int> unifiedSequence;
        vector<int> suffixOrigin;
        
        int minPathLength = INT_MAX;
        int delimiter = -1;

        // Step 1: Concatenate paths with unique structural boundary delimiters
        for (int i = 0; i < numPaths; ++i) {
            minPathLength = min(minPathLength, static_cast<int>(paths[static_cast<size_t>(i)].size()));
            for (int val : paths[static_cast<size_t>(i)]) {
                unifiedSequence.push_back(val);
                suffixOrigin.push_back(i);
            }
            // Append delimiter to separate independent paths safely
            if (i < numPaths - 1) {
                unifiedSequence.push_back(delimiter--);
                suffixOrigin.push_back(-1); // Delimiters do not belong to any valid path
            }
        }

        int totalLen = static_cast<int>(unifiedSequence.size());
        vector<int> sa, rank, lcp;

        // Step 2: Build string parsing primitives
        buildSuffixArray(unifiedSequence, sa, rank);
        buildLCP(unifiedSequence, sa, rank, lcp);

        // Step 3: Binary search framework over subpath length spectrums
        int low = 1, high = minPathLength;
        int maxCommonSubpathLength = 0;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            if (checkCommonSubpath(mid, numPaths, lcp, sa, suffixOrigin, totalLen)) {
                maxCommonSubpathLength = mid;
                low = mid + 1; // Try searching for a longer valid subpath
            } else {
                high = mid - 1; // Contract the length bounds down
            }
        }

        return maxCommonSubpathLength;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log^2 N) - Where N represents the total concatenated length of all paths. 
 *                  Building the suffix array takes O(N log^2 N) via prefix-doubling. The LCP array 
 *                  is constructed in O(N). The binary search runs log(MinLen) iterations, where 
 *                  each validation pass checks the LCP cluster array linearly in O(N) steps.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The data transformation primitives (SA, Rank, LCP, and Origins) 
 *                   instantiate linear memory arrays proportional to the total length of the combined paths.
 */
int main() {
    int totalPaths;
    cout << "Enter the total number of paths (m): ";
    if (!(cin >> totalPaths) || totalPaths <= 0) {
        cout << "Invalid path count parameter." << endl;
        return 1;
    }

    vector<vector<int>> paths(static_cast<size_t>(totalPaths));
    for (int i = 0; i < totalPaths; ++i) {
        int len;
        cout << "Enter the element capacity count for path [" << i << "]: ";
        cin >> len;
        if (len < 0) {
            cout << "Invalid length path configured." << endl;
            return 1;
        }
        if (len > 0) {
            paths[static_cast<size_t>(i)].resize(static_cast<size_t>(len));
            cout << "Enter " << len << " space-separated integers for this path:\n";
            for (int j = 0; j < len; ++j) {
                cin >> paths[static_cast<size_t>(i)][static_cast<size_t>(j)];
            }
        }
    }

    Solution solver;
    cout << "\nExecuting Suffix Array construction with LCP cluster verification passes..." << endl;
    // Max node alphabet range upper bound is arbitrarily large (e.g., 100000)
    int result = solver.longestCommonSubpath(100000, paths);

    cout << "\nLength of the longest common subpath among all paths: " << result << endl;

    return 0;
}