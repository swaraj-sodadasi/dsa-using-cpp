#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (PREFIX MINIMUM APPROACH):
- We iterate through the array of stock prices while tracking the lowest price seen so far (prefix minimum).
- At each step, we calculate the potential profit if we were to sell the stock at the current price: `current_price - min_price_so_far`.
- We continuously update our maximum profit achieved so far if the calculated current profit is greater.
- If the current price is lower than our `min_price_so_far`, we update our minimum price marker.

TIME COMPLEXITY: O(N) - We perform a single linear traversal through the prices vector of size N.
SPACE COMPLEXITY: O(1) - Only a couple of tracking scalar variables are used, requiring no extra space.
*/
int maxProfit(const vector<int>& prices) {
    if (prices.empty()) return 0;
    
    int minPrice = prices[0];
    int maxProfitVal = 0;
    
    for (size_t i = 1; i < prices.size(); ++i) {
        if (prices[i] < minPrice) {
            minPrice = prices[i];
        } else {
            maxProfitVal = max(maxProfitVal, prices[i] - minPrice);
        }
    }
    
    return maxProfitVal;
}

int main() {
    int n;
    cout << "Enter the number of days (elements in the prices array): ";
    cin >> n;
    
    vector<int> prices(n);
    cout << "Enter " << n << " non-negative stock prices separating each with a space or newline: ";
    for (int i = 0; i < n; ++i) {
        cin >> prices[i];
    }
    
    int result = maxProfit(prices);
    cout << "\nThe maximum profit achievable from a single transaction is: " << result << endl;
    
    return 0;
}