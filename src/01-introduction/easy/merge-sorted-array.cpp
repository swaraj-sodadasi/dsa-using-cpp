#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (RIGHT-TO-LEFT IN-PLACE MERGE):
- We are given two sorted arrays, nums1 and nums2. nums1 has a total capacity of m + n, where the first m elements are initialized and the trailing n elements are 0.
- Merging from left to right would require shifting elements or allocating extra space to avoid overwriting elements in nums1.
- To achieve O(1) auxiliary space, we merge from right to left (back to front), starting from the last available index of the combined array ('m + n - 1').
- We maintain three indices:
  - 'p1': tracking the last valid initialized element in nums1 ('m - 1')
  - 'p2': tracking the last element in nums2 ('n - 1')
  - 'pWrite': tracking the position where the next largest element should be written ('m + n - 1')
- In each step, we compare 'nums1[p1]' and 'nums2[p2]', write the larger element to 'nums1[pWrite]', and decrement both 'pWrite' and the pointer of the array from which the element was taken.
- If 'p1' becomes less than 0 before 'p2', any remaining elements in nums2 are copied over directly into nums1. If 'p2' becomes less than 0 first, the remaining elements of nums1 are already in their correct sorted positions.

TIME COMPLEXITY: O(m + n) - We perform a linear scan backwards filling up the combined layout of size m + n.
SPACE COMPLEXITY: O(1) - The merge occurs directly inside the extra capacity layout provided by nums1, requiring no extra buffer.
*/
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    int p1 = m - 1;
    int p2 = n - 1;
    int pWrite = m + n - 1;
    
    while (p2 >= 0) {
        if (p1 >= 0 && nums1[p1] > nums2[p2]) {
            nums1[pWrite] = nums1[p1];
            p1--;
        } else {
            nums1[pWrite] = nums2[p2];
            p2--;
        }
        pWrite--;
    }
}

int main() {
    int m, n;
    cout << "Enter the number of initial valid elements in nums1 (m): ";
    cin >> m;
    cout << "Enter the number of elements in nums2 (n): ";
    cin >> n;
    
    vector<int> nums1(m + n);
    cout << "Enter " << m << " sorted integers for nums1 separating each with a space or newline: ";
    for (int i = 0; i < m; ++i) {
        cin >> nums1[i];
    }
    
    vector<int> nums2(n);
    cout << "Enter " << n << " sorted integers for nums2 separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums2[i];
    }
    
    merge(nums1, m, nums2, n);
    
    cout << "\nMerged sorted array: ";
    for (int i = 0; i < m + n; ++i) {
        cout << nums1[i] << " ";
    }
    cout << endl;
    
    return 0;
}