#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

using namespace std;

class Solution {
private:
    // Base-62 character alphabet set representing all possible characters in our shortened URL key
    const string BASE62 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    // Reverse lookup registry mapping the unique shortened key to its original long URL
    // Map stores: {Short_Key -> Original_Long_URL}
    unordered_map<string, string> keyToUrlMap;
    
    // Forward lookup registry to handle duplicate URLs gracefully without re-encoding
    // Map stores: {Original_Long_URL -> Short_Key}
    unordered_map<string, string> urlToKeyMap;
    
    // Fixed length constraint for the generated short key code string
    const int KEY_LENGTH = 6;

    /*
     * Private helper to generate a randomized Base-62 key of fixed length.
     * With a length of 6 characters, this creates 62^6 (approx. 56.8 billion) 
     * unique key combinations, providing massive capacity and virtually eliminating collisions.
     */
    string generateRandomKey() {
        string randomKey = "";
        for (int i = 0; i < KEY_LENGTH; ++i) {
            size_t randomIndex = static_cast<size_t>(rand()) % 62;
            randomKey += BASE62[randomIndex];
        }
        return randomKey;
    }

public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: ENCODE AND DECODE TINYURL (BASE-62 RANDOM STRING ID MAPPING)
     * -------------------------------------------------------------------------------------
     * The objective is to design a service layer that compresses a long URL into a short URL, 
     * and accurately resolves that short URL back to the original long URL in O(1) time.
     * 
     * 1. THE REVERSE BINDING MAPPING ARCHITECTURE:
     *    Instead of applying mathematical hashing functions like MD5 or SHA-256 (which produce 
     *    unnecessarily long keys and require complex collision handling), we deploy a dual-map 
     *    Base-62 random string mapping approach.
     * 
     * 2. COLLISION-RESISTANT ENCODING PIPELINE:
     *    When `encode()` is triggered:
     *      - Duplicate Protection: We first query `urlToKeyMap`. If the long URL has already been 
     *        encoded, we immediately return its existing key, preventing redundant storage.
     *      - Unique Key Isolation: If it is a new URL, we enter a `while` loop that generates random 
     *        Base-62 keys until it finds one that doesn't exist in `keyToUrlMap`. 
     *      - Bidirectional Registration: We register the new key in both maps and return the short URL.
     * 
     * 3. INVERSE DECODING RESOLUTION:
     *    When `decode()` is triggered, we strip away the domain prefix to isolate the 6-character key. 
     *    We look up this key in `keyToUrlMap` to return the original long URL in amortized constant time.
     */
    Solution() {
        // Initialize the random number generator using the current time as a seed
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // Encodes a URL to a shortened URL.
    string encode(string longUrl) {
        // Step 2: Return early if this URL has already been processed
        if (urlToKeyMap.find(longUrl) != urlToKeyMap.end()) {
            return "http://tinyurl.com/" + urlToKeyMap[longUrl];
        }

        // Loop continuously until a completely unique short key is generated
        string uniqueShortKey = generateRandomKey();
        while (keyToUrlMap.find(uniqueShortKey) != keyToUrlMap.end()) {
            uniqueShortKey = generateRandomKey();
        }

        // Log the new bidirectional mappings inside our database registries
        keyToUrlMap[uniqueShortKey] = longUrl;
        urlToKeyMap[longUrl] = uniqueShortKey;

        return "http://tinyurl.com/" + uniqueShortKey;
    }

    // Decodes a shortened URL to its original URL.
    string decode(string shortUrl) {
        // Step 3: Extract the 6-character key from the end of the short URL
        // Example: "http://tinyurl.com/abc123" -> cut out "abc123"
        size_t lastSlashPosition = shortUrl.find_last_of('/');
        if (lastSlashPosition == string::npos || lastSlashPosition == shortUrl.length() - 1) {
            return ""; // Invalid shortened URL layout format
        }
        
        string shortKey = shortUrl.substr(lastSlashPosition + 1);

        // Fetch the matching long URL from the reverse mapping registry
        if (keyToUrlMap.find(shortKey) != keyToUrlMap.end()) {
            return keyToUrlMap[shortKey];
        }

        return ""; // Fallback return if the key is missing from our registry
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: Amortized O(1) for both encode and decode operations. 
 *                  Key lookups and map insertions scale independently of the string 
 *                  collection size. The random collision loop runs in O(1) expected time 
 *                  because the massive 56.8 billion key space keeps collisions incredibly rare.
 *
 * SPACE COMPLEXITY: O(N * L) Auxiliary - Where N represents the total count of unique URLs 
 *                   encoded, and L represents the average character length of the URLs. The 
 *                   system allocates memory proportionally to store strings in both hash maps.
 */
int main() {
    Solution tinyUrlSystem;
    string inputLongUrl;

    cout << "=== TinyURL System Simulation Console ===\n";
    cout << "Enter a long destination URL to encode: ";
    if (!getline(cin, inputLongUrl)) return 1;

    // Execute Encoding Pass
    cout << "\nExecuting collision-resistant Base-62 encoding..." << endl;
    string generatedShortUrl = tinyUrlSystem.encode(inputLongUrl);
    cout << "Generated TinyURL: " << generatedShortUrl << "\n";

    // Execute Decoding Pass
    cout << "\nExecuting inverse decoding address resolution..." << endl;
    string resolvedLongUrl = tinyUrlSystem.decode(generatedShortUrl);
    cout << "Resolved Long URL: " << resolvedLongUrl << "\n";

    // Verification check to confirm integrity
    if (resolvedLongUrl == inputLongUrl) {
        cout << "\nSystem Integrity Check: SUCCESS (Original URL matches the decoded output perfectly)." << endl;
    } else {
        cout << "\nSystem Integrity Check: FAILED." << endl;
    }

    return 0;
}