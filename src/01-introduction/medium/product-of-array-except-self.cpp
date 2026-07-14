#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (PREFIX AND SUFFIX ACCUMULATION WITHOUT DIVISION):
- Since division is strictly forbidden, we build the answer array by calculating two directional products: everything to the left of an element, and everything to its right.
- We allocate a result vector of the same size as the input array.
- In the first forward pass, we calculate the prefix products. `result[i]` stores the product of all elements to the left of index `i`. We maintain a running product variable initialized to 1.
- In the second backward pass, we compute the suffix products dynamically. We iterate from right to left, keeping track of a running suffix product variable. We multiply the existing value in `result[i]` by this suffix product.
- This results in `result[i]` containing `(product of all elements before i) * (product of all elements after i)`, which is exactly the product of the array except self.

TIME COMPLEXITY: O(N) - Two sequential linear passes are made over the array of size N.
SPACE COMPLEXITY: O(1) auxiliary - Beyond the output array container required by the problem, only a few primitive variables are maintained for tracking products.
*/
vector<int> productExceptSelf(const vector<int>& nums) {
    size_t n = nums.size();
    vector<int> result(n, 1);
    
    int prefixProduct = 1;
    for (size_t i = 0; i < n; ++i) {
        result[i] = prefixProduct;
        prefixProduct *= nums[i];
    }
    
    int suffixProduct = 1;
    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
        result[i] *= suffixProduct;
        suffixProduct *= nums[static_cast<size_t>(i)];
    }
    
    return result;
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    
    vector<int> nums(n);
    cout << "Enter " << n << " integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    vector<int> result = productExceptSelf(nums);
    
    cout << "\nProduct array except self: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;
    
    return 0;
}