#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
 * ONLINE CHUNKED SORTING STREAM ARCHITECTURE ENGINE
 * -------------------------------------------------
 * The objective is to design an Ordered Stream data structure that accepts 
 * (idKey, value) pairs in an arbitrary order and returns a chunk of values 
 * sorted by their idKeys in an incremental, non-blocking online fashion.
 *
 * Structural Strategy:
 * - We allocate a fixed-size internal buffer array `streamBuffer` matching the maximum 
 *   capacity `n + 1` to accommodate 1-based indexing for fast direct lookups.
 * - A pointer `ptr` is maintained to point to the next expected continuous value 
 *   idKey key slot (initially set to 1).
 *
 * Stream Processing Protocol:
 * - Step 1: Upon receiving an insertion query via `insert(idKey, value)`, we map and 
 *   store the string `value` directly into `streamBuffer[idKey]` in O(1) time.
 * - Step 2: Check if `idKey` matches the current expected stream checkpoint `ptr`.
 *   - If `idKey != ptr`, the incoming block is out of order. We do not advance the stream; 
 *     we return an empty chunk vector immediately.
 *   - If `idKey == ptr`, the contiguous sequence is maintained. We scan forward from `ptr`, 
 *     extracting all consecutive stored string blocks into a chunk result container until 
 *     we encounter an unpopulated slot or hit the boundary limitation.
 * - Step 3: Advance the pointer `ptr` to this newly discovered boundary index and return the 
 *   extracted chunk.
 */
class OrderedStream {
private:
    vector<string> streamBuffer;
    size_t ptr;

public:
    // Initialize the stream framework layout to comfortably host N tracking components
    OrderedStream(int n) {
        streamBuffer.resize(static_cast<size_t>(n + 2), "");
        ptr = 1;
    }
    
    vector<string> insert(int idKey, string value) {
        size_t index = static_cast<size_t>(idKey);
        streamBuffer[index] = value;
        
        vector<string> continuousChunk;
        
        // If the inserted element matches our active checkpoint tracker, collect contiguous blocks
        if (index == ptr) {
            while (!streamBuffer[ptr].empty()) {
                continuousChunk.push_back(streamBuffer[ptr]);
                ptr++;
            }
        }
        
        return continuousChunk;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(1) Amortized Per Insertion - Inserting an element into the fixed matrix index 
 *                  costs O(1). While the forward scanning loop inside a single `insert` call can run 
 *                  up to K steps, the index `ptr` can only advance at most N times across the entire 
 *                  lifetime of the object instance. This yields a steady O(1) amortized cost per call.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The stream architecture instantiates an internal vector container 
 *                   proportional to the maximum data capacity `n` configured during setup.
 */
int main() {
    int capacity;
    cout << "Enter the total tracking capacity size threshold for the Ordered Stream (N): ";
    if (!(cin >> capacity) || capacity <= 0) {
        cout << "Invalid parameter. Capacity must be a positive value." << endl;
        return 1;
    }

    OrderedStream os(capacity);
    cout << "\nOrdered Stream Engine initialized successfully." << endl;
    cout << "Enter " << capacity << " insertion operations in format [idKey] [string_value]:\n";

    for (int i = 0; i < capacity; ++i) {
        int idKey;
        string value;
        cout << "\nOperation [" << i + 1 << "/" << capacity << "] - Enter idKey and Value: ";
        cin >> idKey >> value;

        if (idKey < 1 || idKey > capacity) {
            cout << "Constraint Error: idKey parameter falls outside boundary limits {1 to N}." << endl;
            return 1;
        }

        vector<string> chunkResult = os.insert(idKey, value);

        cout << "Stream output chunk response matrix: [ ";
        for (size_t j = 0; j < chunkResult.size(); ++j) {
            cout << "\"" << chunkResult[j] << "\"" << (j + 1 < chunkResult.size() ? ", " : "");
        }
        cout << " ]" << endl;
    }

    return 0;
}