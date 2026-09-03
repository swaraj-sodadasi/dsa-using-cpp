#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: CHECK IF ARRAY PAIRS ARE DIVISIBLE BY K (REMAINDER MATCHING)
     * -------------------------------------------------------------------------------------
     * The objective is to determine if an array of even length can be paired up such 
     * that the sum of every pair is divisible by k.
     * 
     * 1. MODULO FREQUENCY COUNTING:
     *    For any integer x, its remainder modulo k is given by `((x % k) + k) % k` to handle 
     *    negative numbers properly. We count the frequency of each remainder in a frequency array.
     * 
     * 2. MATCHING CONDITIONS FOR PAIRING:
     *    - Remainder 0: Numbers with remainder 0 can only pair with other numbers with remainder 0. 
     *      Thus, `freq[0]` must be even.
     *    - Remainder r (where r > 0 and r != k - r): A number with remainder `r` must pair with a 
     *      number with remainder `k - r`. Thus, `freq[r]` must equal `freq[k - r]`.
     *    - Remainder k / 2 (when k is even): Numbers with remainder `k / 2` must pair with each other. 
     *      Thus, `freq[k / 2]` must be even.
     */
    bool canArrange(const vector<int>& arr, int k) {
        vector<int> freq(static_cast<size_t>(k), 0);

        for (int num : arr) {
            // Normalize remainder to handle negative integers correctly
            int rem = ((num % k) + k) % k;
            freq[static_cast<size_t>(rem)]++;
        }

        // Rule 1: Remainder 0 frequency must be even
        if (freq[0] % 2 != 0) {
            return false;
        }

        // Rule 2: Remainder r must match Remainder k - r
        for (int r = 1; r <= k / 2; ++r) {
            if (r == k - r) {
                // When k is even and r == k / 2, its frequency must be even
                if (freq[static_cast<size_t>(r)] % 2 != 0) {
                    return false;
                }
            } else {
                if (freq[static_cast<size_t>(r)] != freq[static_cast<size_t>(k - r)]) {
                    return false;
                }
            }
        }

        return true;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N + K) - Where N is the number of elements in arr and K is the divisor.
 *                  Single pass to compute frequencies, followed by K/2 checks.
 *
 * SPACE COMPLEXITY: O(K) Auxiliary - Frequency array of size K.
 */
int main() {
    cout << "=== Array Pairs Divisible by K Engine ===\n";
    cout << "Enter the number of elements (must be even): ";
    int n;
    if (!(cin >> n) || n <= 0 || n % 2 != 0) return 1;

    vector<int> arr(static_cast<size_t>(n));
    cout << "Enter the array elements sequentially:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[static_cast<size_t>(i)];
    }

    cout << "Enter the divisor k: ";
    int k;
    if (!(cin >> k) || k <= 0) return 1;

    Solution solver;
    bool result = solver.canArrange(arr, k);

    if (result) {
        cout << "Array can be paired such that each pair sum is divisible by k! Output: [true]\n";
    } else {
        cout << "Array CANNOT be paired appropriately. Output: [false]\n";
    }

    return 0;
}