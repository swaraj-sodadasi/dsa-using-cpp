#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (TWO-PASS GREEDY DISTRIBUTION):
- Every child must receive at least one candy. We initialize a distribution vector tracking candies per child with all 1s.
- Pass 1 (Left-to-Right): We iterate forward from index 1 to 'size - 1'. If the current child has a strictly higher rating than the left neighbor (`ratings[i] > ratings[i - 1]`), they must get more candies than the left neighbor: `candies[i] = candies[i - 1] + 1`.
- Pass 2 (Right-to-Left): We iterate backward from index 'size - 2' down to 0. If the current child has a strictly higher rating than the right neighbor (`ratings[i] > ratings[i + 1]`), we satisfy this constraint while preserving the forward pass results by taking the maximum: `candies[i] = max(candies[i], candies[i + 1] + 1)`.
- Summing up the elements in the final `candies` vector yields the minimum total candies needed.

TIME COMPLEXITY: O(N) - We perform two sequential linear scans over the ratings array of size N.
SPACE COMPLEXITY: O(N) - An auxiliary vector of size N is allocated to track the candy counts assigned to each child.
*/
int candy(const vector<int>& ratings) {
    size_t n = ratings.size();
    if (n == 0) return 0;
    
    vector<int> candies(n, 1);
    
    // Left-to-Right Pass
    for (size_t i = 1; i < n; ++i) {
        if (ratings[i] > ratings[i - 1]) {
            candies[i] = candies[i - 1] + 1;
        }
    }
    
    // Right-to-Left Pass
    for (int i = static_cast<int>(n) - 2; i >= 0; --i) {
        size_t idx = static_cast<size_t>(i);
        if (ratings[idx] > ratings[idx + 1]) {
            candies[idx] = max(candies[idx], candies[idx + 1] + 1);
        }
    }
    
    return accumulate(candies.begin(), candies.end(), 0);
}

int main() {
    int n;
    cout << "Enter the number of children: ";
    cin >> n;
    
    vector<int> ratings(n);
    cout << "Enter " << n << " ratings integers separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> ratings[i];
    }
    
    int result = candy(ratings);
    cout << "\nMinimum total candies required: " << result << endl;
    
    return 0;
}