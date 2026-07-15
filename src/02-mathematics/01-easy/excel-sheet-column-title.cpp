#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BASE-26 BIJECTIVE NUMERATION CONVERSION):
- The mapping of Excel columns (A -> 1, B -> 2, ..., Z -> 26, AA -> 27) resembles a base-26 system, 
  but with a critical difference: it is a bijective base-26 numeration system, meaning there is no symbol for 0.
- In a standard positional number system, the digits range from 0 to base-1. Here, the symbols range from 1 to 26.
- To resolve this mapping misalignment during conversion, we decrement the current number `columnNumber` by 1 
  at the beginning of each extraction step. 
- After subtracting 1, the remainder `columnNumber % 26` shifts cleanly into a standard 0-indexed range [0, 25]. 
  This maps directly to ASCII characters where 0 corresponds to 'A', 1 to 'B', and 25 to 'Z'.
- We convert the remainder into its character representation: `char c = 'A' + (columnNumber % 26)`.
- We append this character to our accumulator string and update `columnNumber = columnNumber / 26`.
- Because we extract the digits from the least significant place value (rightmost) to the most significant 
  place value (leftmost), we reverse the accumulated string at the end before returning it.

TIME COMPLEXITY: O(log26(N)) - The number of loop steps is proportional to the number of digits in the 
                 base-26 column title string representation of the number N.
SPACE COMPLEXITY: O(1) auxiliary - The string manipulations take place within the returning result buffer, 
                 requiring no extra working dynamic memory allocations.
*/
string convertToTitle(int columnNumber) {
    string title = "";
    
    while (columnNumber > 0) {
        // Shift to a 0-indexed base-26 system to handle the lack of a '0' symbol
        columnNumber--;
        
        int remainder = columnNumber % 26;
        title.push_back(static_cast<char>('A' + remainder));
        
        columnNumber /= 26;
    }
    
    // Reverse the string since characters were extracted from right to left
    reverse(title.begin(), title.end());
    return title;
}

int main() {
    int columnNumber;
    cout << "Enter the Excel sheet column number: ";
    cin >> columnNumber;

    if (columnNumber <= 0) {
        cout << "Invalid input. Column number must be a positive integer." << endl;
        return 1;
    }

    string title = convertToTitle(columnNumber);
    cout << "The corresponding Excel sheet column title is: \"" << title << "\"" << endl;

    return 0;
}