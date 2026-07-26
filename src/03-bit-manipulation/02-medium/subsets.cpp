#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BITMASK-DRIVEN COMBINATORIAL POWER SET GENERATION):
- The objective is to generate the power set (all possible subsets) of a set of unique integers.
- **Bijective Binary Mapping**:
  - For a set containing $N$ elements, the total number of unique subsets is exactly $2^N$.
  - We can establish a direct mapping between each integer in the range $[0, 2^N - 1]$ and a unique subset.
  - The binary representation of each number acts as a bitmask where the $i$-th bit indicates whether the $i$-th element of the set is included (1) or excluded (0) from that particular subset.
- **Algorithmic Iteration**:
  - We calculate the total subset count: `totalSubsets = 1 << N` (which evaluates to $2^N$).
  - We run a loop from `0` to `totalSubsets - 1`.
  - For each mask, we inspect every bit position `i` from `0` to `N - 1`. If `(mask & (1 << i))` is non-zero, we add `nums[i]` to the current subset.

TIME COMPLEXITY: O(N * 2^N) - There are $2^N$ unique subsets generated, and we spend $O(N)$ time building each subset mask sequence.
SPACE COMPLEXITY: O(1) auxiliary - Beyond the storage array used to hold and return the generated power set matrix, the calculation runs in-place.
*/
vector<vector<int>> subsets(const vector<int>& nums) {
    size_t n = nums.size();
    size_t totalSubsets = 1ULL << n; // 2^N combinations mapped via bit-shifting
    vector<vector<int>> powerSet;
    powerSet.reserve(totalSubsets);

    // Iterate through every possible numeric bitmask state value combination
    for (size_t mask = 0; mask < totalSubsets; ++mask) {
        vector<int> currentSubset;
        
        for (size_t i = 0; i < n; ++i) {
            // Check if the i-th bit is set in the current state bitmask pattern
            if ((mask & (1ULL << i)) != 0) {
                currentSubset.push_back(nums[i]);
            }
        }
        powerSet.push_back(currentSubset);
    }

    return powerSet;
}

int main() {
    int n;
    cout << "Enter the number of unique elements: ";
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input sizing configuration." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the distinct integer elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    vector<vector<int>> result = subsets(nums);

    cout << "\nThe total generated combinatorial power set contains " << result.size() << " subsets:\n";
    for (const auto& subset : result) {
        cout << "[ ";
        for (int num : subset) {
            cout << num << " ";
        }
        cout << "]\n";
    }

    return 0;
}