#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Solution {
private:
    /*
     * LOMUTO PARTITIONING SUBSYSTEM
     * -----------------------------
     * Rearranges elements around a chosen pivot inside the range [left, right] 
     * using Lomuto's scheme with uniform random pivot tracking.
     */
    int partition(vector<int>& nums, int left, int right) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(left, right);
        int pivotIndex = dis(gen);
        
        swap(nums[static_cast<size_t>(pivotIndex)], nums[static_cast<size_t>(right)]);
        int pivot = nums[static_cast<size_t>(right)];
        
        int i = left;
        for (int j = left; j < right; ++j) {
            if (nums[static_cast<size_t>(j)] <= pivot) {
                swap(nums[static_cast<size_t>(i)], nums[static_cast<size_t>(j)]);
                i++;
            }
        }
        
        swap(nums[static_cast<size_t>(i)], nums[static_cast<size_t>(right)]);
        return i;
    }

    /*
     * QUICK-SELECT MEDIAN EXTRACTION UTILITY
     * --------------------------------------
     * Isolates the exact numerical median of an unsorted collection in O(N) average time.
     */
    int findMedian(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int targetIndex = n / 2;
        int left = 0, right = n - 1;
        
        while (left <= right) {
            int pivotIdx = partition(nums, left, right);
            if (pivotIdx == targetIndex) {
                return nums[static_cast<size_t>(pivotIdx)];
            } else if (pivotIdx > targetIndex) {
                right = pivotIdx - 1;
            } else {
                left = pivotIdx + 1;
            }
        }
        return nums[static_cast<size_t>(targetIndex)];
    }

    /*
     * VIRTUAL INDEX MAPPING FUNCTION
     * ------------------------------
     * Maps standard index locations to alternating odd and even slots.
     * This places the first half of elements into even slots from right to left, 
     * and the second half into odd slots from right to left, preventing identical 
     * median values from overlapping at adjacent boundaries.
     * 
     * Formula: (1 + 2 * i) % (n | 1)
     */
    size_t virtualIndex(int i, int n) {
        return static_cast<size_t>((1 + 2 * i) % (n | 1));
    }

public:
    /*
     * VIRTUAL INDEX MAPPING WITH THREE-WAY ELEMENT PARTITIONING ENGINE
     * -----------------------------------------------------------------
     * The objective is to rearrange an unsorted array such that:
     * nums[0] < nums[1] > nums[2] < nums[3]...
     * The reordering must be executed in O(N) time and O(1) auxiliary space constraints.
     *
     * Algorithmic Strategy:
     * - Step 1: Compute the median of the array using an O(N) Quick-Select framework.
     * - Step 2: Perform a Dutch National Flag three-way partition centered around the median. 
     *   Instead of writing to normal linear array cells, we redirect our operations using a 
     *   Virtual Index Mapping macro function.
     * - The Virtual Index Mapping function `(1 + 2 * i) % (n | 1)` distributes indices such that:
     *   - Values larger than the median settle into odd indices (1, 3, 5, ...).
     *   - Values smaller than the median settle into even indices (0, 2, 4, ...).
     *   - Values equal to the median fill out the remaining available spaces.
     * - This separation spaces out identical values, satisfying the strict alternative inequalities.
     */
    void wiggleSort(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        if (n <= 1) return;

        // Step 1: Isolate the true central value profile bounds
        int median = findMedian(nums);

        // Step 2: Three-Way Dutch National Flag Partitioning bound to virtual coordinates
        int low = 0;
        int mid = 0;
        int high = n - 1;

        while (mid <= high) {
            size_t vMid = virtualIndex(mid, n);
            
            if (nums[vMid] > median) {
                size_t vLow = virtualIndex(low, n);
                swap(nums[vLow], nums[vMid]);
                low++;
                mid++;
            } 
            else if (nums[vMid] < median) {
                size_t vHigh = virtualIndex(high, n);
                swap(nums[vMid], nums[vHigh]);
                high--;
            } 
            else {
                mid++;
            }
        }
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) Average Case - Quick-Select yields the median in linear time, 
 *                  and the subsequent virtual three-way partitioning pass processes each 
 *                  index exactly once.
 *
 * SPACE COMPLEXITY: O(1) Auxiliary - The virtual coordinate translations compute values 
 *                   inline, running completely in-place without copying data blocks.
 */
int main() {
    int n;
    cout << "Enter the total number of elements in the array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Array size must be a positive integer value." << endl;
        return 1;
    }

    vector<int> nums(static_cast<size_t>(n));
    cout << "Enter the integer elements separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> nums[static_cast<size_t>(i)];
    }

    Solution solver;
    cout << "\nExecuting inline virtual index three-way partitioning transformations..." << endl;
    solver.wiggleSort(nums);

    cout << "\nWiggle Sorted (II) sequence validation output:\n[ ";
    for (size_t i = 0; i < nums.size(); ++i) {
        cout << nums[i] << (i + 1 < nums.size() ? ", " : "");
    }
    cout << " ]\n";

    // Continuous runtime sanity verification check
    bool isValid = true;
    for (size_t i = 1; i < nums.size(); ++i) {
        if (i % 2 == 1) {
            if (nums[i] <= nums[i - 1]) isValid = false;
        } else {
            if (nums[i] >= nums[i - 1]) isValid = false;
        }
    }
    cout << "Inequality structure validation check status: " << (isValid ? "PASS" : "FAIL") << endl;

    return 0;
}