#include <iostream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

/*
CORE LOGIC (TWO-POINTER FREQUENCY-COUNT SLIDING WINDOW):
- We maintain two frequency maps (or fixed-size arrays for ASCII characters): one for the target string `t` (`targetCounts`), 
  and one for the active sliding window in `s` (`windowCounts`).
- We keep track of a `formed` variable, which counts how many unique characters in `t` have met their required frequencies within the current window.
- The `required` variable represents the total number of unique characters in `t` that must be matched.
- We expand the window by moving the `right` pointer forward. If the newly added character matches the frequency requirement in `t`, we increment `formed`.
- While `formed == required`, we try to contract the window by moving the `left` pointer forward to find the smallest valid window. Before shrinking, we update our minimum window bounds if the current window is smaller than the previous minimum.
- When we remove the character at the `left` pointer, if its count drops below the required frequency in `t`, we decrement `formed`, meaning the window is no longer valid, and we must resume expanding `right`.

TIME COMPLEXITY: O(|S| + |T|) - In the worst case, both the left and right pointers traverse the string `s` at most once. Initializing the frequency map for `t` takes O(|T|) time.
SPACE COMPLEXITY: O(1) auxiliary - The frequency arrays use a constant size of 128 to cover standard ASCII characters, fulfilling the O(1) space constraint regardless of string sizes.
*/
string minWindow(const string& s, const string& t) {
    if (s.empty() || t.empty() || s.length() < t.length()) {
        return "";
    }

    vector<int> targetCounts(128, 0);
    vector<int> windowCounts(128, 0);

    int required = 0;
    for (char c : t) {
        if (targetCounts[static_cast<size_t>(c)] == 0) {
            required++;
        }
        targetCounts[static_cast<size_t>(c)]++;
    }

    int left = 0;
    int right = 0;
    int formed = 0;

    int minLen = INT_MAX;
    int startIdx = 0;

    while (right < static_cast<int>(s.length())) {
        char c = s[static_cast<size_t>(right)];
        windowCounts[static_cast<size_t>(c)]++;

        if (targetCounts[static_cast<size_t>(c)] > 0 && 
            windowCounts[static_cast<size_t>(c)] == targetCounts[static_cast<size_t>(c)]) {
            formed++;
        }

        while (formed == required && left <= right) {
            int currentLen = right - left + 1;
            if (currentLen < minLen) {
                minLen = currentLen;
                startIdx = left;
            }

            char leftChar = s[static_cast<size_t>(left)];
            windowCounts[static_cast<size_t>(leftChar)]--;

            if (targetCounts[static_cast<size_t>(leftChar)] > 0 && 
                windowCounts[static_cast<size_t>(leftChar)] < targetCounts[static_cast<size_t>(leftChar)]) {
                formed--;
            }
            left++;
        }
        right++;
    }

    return (minLen == INT_MAX) ? "" : s.substr(static_cast<size_t>(startIdx), static_cast<size_t>(minLen));
}

int main() {
    string s, t;
    cout << "Enter the source string s: ";
    cin >> s;
    cout << "Enter the target string t: ";
    cin >> t;

    string result = minWindow(s, t);

    if (result.empty()) {
        cout << "\nNo valid window substring found." << endl;
    } else {
        cout << "\nThe minimum window substring is: \"" << result << "\"" << endl;
    }

    return 0;
}