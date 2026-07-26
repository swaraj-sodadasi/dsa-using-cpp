#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BITMASK-ENCODED STRING CHARACTER SETS):
- The objective is to find the maximum product of lengths of two words from an array such that the two words do not share any common characters.
- **Bitmask Conversion**:
  - Since the words contain only lowercase English letters ('a' through 'z'), we can represent the unique character set of each word using a single 32-bit integer (a bitmask).
  - For each character `c` in a word, we set the corresponding bit at position `c - 'a'` to 1 using the bitwise expression: `mask |= (1 << (c - 'a'))`.
- **Overlap Validation via Bitwise AND**:
  - To check if two words share any common letters, we perform a bitwise AND operation on their precomputed masks: `(masks[i] & masks[j])`.
  - If the result is exactly 0, it guarantees that the two words have no letters in common.
- **Optimization Strategy**:
  - We precompute the bitmasks and lengths for all words. Then, we use a nested loop to check every unique pair `(i, j)`.
  - If a pair shares no characters, we compute the product of their lengths and track the maximum product found.

TIME COMPLEXITY: O(L + N^2) - Where L is the sum of the lengths of all words combined (to construct the bitmasks) and N is the number of words in the vector (for the nested pair evaluation).
SPACE COMPLEXITY: O(N) auxiliary - Storage allocated to hold the `masks` vector for all N elements.
*/
int maxProduct(const vector<string>& words) {
    size_t n = words.size();
    vector<int> masks(n, 0);

    // Step 1: Compute bitmask signatures for each word
    for (size_t i = 0; i < n; ++i) {
        for (char c : words[i]) {
            if (c >= 'a' && c <= 'z') {
                masks[i] |= (1 << (c - 'a'));
            }
        }
    }

    int maxProd = 0;

    // Step 2: Evaluate products of pairs that do not share any letters
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            // If the bitwise AND is 0, no characters are shared between words[i] and words[j]
            if ((masks[i] & masks[j]) == 0) {
                int currentProduct = static_cast<int>(words[i].length() * words[j].length());
                maxProd = max(maxProd, currentProduct);
            }
        }
    }

    return maxProd;
}

int main() {
    int numWords;
    cout << "Enter the total number of words: ";
    if (!(cin >> numWords) || numWords <= 0) {
        cout << "Invalid input sizing configuration." << endl;
        return 1;
    }

    vector<string> words(static_cast<size_t>(numWords));
    cout << "Enter the words separated by spaces:\n";
    for (int i = 0; i < numWords; ++i) {
        cin >> words[static_cast<size_t>(i)];
    }

    int result = maxProduct(words);
    cout << "\nThe maximum product of word lengths without shared characters is: " << result << endl;

    return 0;
}