#include <iostream>
#include <vector>

using namespace std;

/*
CORE LOGIC (BITMASK BYTE-HEADER PATTERN TRACKING):
- The objective is to validate whether a given array of integers represents a valid UTF-8 encoding sequence.
- **UTF-8 Encoding Pattern Rules**:
  - A character can be encoded using 1 to 4 bytes.
  - **1-Byte Character**: Starts with a `0` bit (`0xxxxxxx`).
  - **2-Byte Character**: Starts with `110xxxxx`, followed by exactly 1 continuation byte.
  - **3-Byte Character**: Starts with `1110xxxx`, followed by exactly 2 continuation bytes.
  - **4-Byte Character**: Starts with `11110xxx`, followed by exactly 3 continuation bytes.
  - **Continuation Byte**: Must always start with a `10` pattern (`10xxxxxx`).
- **Algorithmic State Tracking**:
  - We maintain a `remainingBytes` counter to track how many continuation bytes are expected for the current character.
  - For each byte in the vector, we isolate its most significant 8 bits since the problem states each integer represents a single byte.
  - **Case 1: `remainingBytes == 0` (Expecting a new character header)**:
    - We check the leading bits to determine the character length:
      - If `(byte >> 5) == 0b110`, it is a 2-byte header $\rightarrow$ `remainingBytes = 1`.
      - If `(byte >> 4) == 0b1110`, it is a 3-byte header $\rightarrow$ `remainingBytes = 2`.
      - If `(byte >> 3) == 0b11110`, it is a 4-byte header $\rightarrow$ `remainingBytes = 3`.
      - If `(byte >> 7) == 0b0`, it is a valid 1-byte character $\rightarrow$ `remainingBytes = 0`.
      - Any other pattern means the sequence is invalid.
  - **Case 2: `remainingBytes > 0` (Expecting a continuation byte)**:
    - The byte must match the `10xxxxxx` pattern. We verify this using the check `(byte >> 6) == 0b10`. If true, we decrement `remainingBytes`. If false, it is invalid.
  - **Final Validation**: After checking all bytes, `remainingBytes` must be exactly 0. If it is greater than 0, it means the last character was cut short.

TIME COMPLEXITY: O(N) - Where N is the total number of integers in the input vector. We process each byte exactly once in constant time.
SPACE COMPLEXITY: O(1) auxiliary - State validation is tracked completely in-place using localized scalar registers.
*/
bool validUtf8(const vector<int>& data) {
    int remainingBytes = 0;

    for (int num : data) {
        // Mask the integer to isolate only the 8 bits of a standard byte
        int byte = num & 0xFF;

        if (remainingBytes == 0) {
            // Determine the number of bytes in this UTF-8 sequence from the header byte
            if ((byte >> 5) == 0b110) {
                remainingBytes = 1;
            } else if ((byte >> 4) == 0b1110) {
                remainingBytes = 2;
            } else if ((byte >> 3) == 0b11110) {
                remainingBytes = 3;
            } else if ((byte >> 7) != 0b0) {
                // If it doesn't start with 0, and didn't match the multi-byte prefixes, it is invalid
                return false;
            }
        } else {
            // Verify that the byte is a valid continuation byte starting with the prefix 10xxxxxx
            if ((byte >> 6) != 0b10) {
                return false;
            }
            remainingBytes--;
        }
    }

    // A valid sequence must not leave any trailing, unfulfilled character bytes
    return remainingBytes == 0;
}

int main() {
    int n;
    cout << "Enter the number of bytes in the dataset array: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid sizing configuration input bounds." << endl;
        return 1;
    }

    vector<int> data(static_cast<size_t>(n));
    cout << "Enter the data array integers (values representing bytes 0-255):\n";
    for (int i = 0; i < n; ++i) {
        cin >> data[static_cast<size_t>(i)];
    }

    if (validUtf8(data)) {
        cout << "\nThe input data sequence represents a valid UTF-8 encoding." << endl;
    } else {
        cout << "\nThe input data sequence is NOT a valid UTF-8 encoding." << endl;
    }

    return 0;
}