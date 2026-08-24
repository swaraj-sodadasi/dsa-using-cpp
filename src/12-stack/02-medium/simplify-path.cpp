#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Solution {
public:
    /*
     * -------------------------------------------------------------------------------------
     * CORE LOGIC: UNIX DIRECTORY TOKENIZATION STACK TRAVERSAL
     * -------------------------------------------------------------------------------------
     * The objective is to convert an absolute Unix path into its simplified canonical form.
     * 
     * 1. STRINGSTREAM TOKENIZATION LANE:
     *    Unix path segments are delimited by slash '/' characters. We use `std::stringstream` 
     *    interleaved with `std::getline` to isolate directory tokens efficiently while bypassing
     *    consecutive empty slices caused by multiple slashes (e.g., "///").
     * 
     * 2. STATE TRANSITION RULES VIA VECTOR STACK:
     *    We parse tokens sequentially, matching three distinct operational constraints:
     *      - Empty token or "." : Current folder indicator or dead segment. Safe to ignore.
     *      - ".."              : Parent directory command. We pop the last added directory from 
     *                             our vector stack if it's not empty (moving up one level).
     *      - Any other name     : Valid subdirectory. We push the folder name onto the stack.
     * 
     * 3. CANONICAL STITCHING:
     *    Once the path is fully tokenized, we reconstruct the canonical path by joining the 
     *    elements remaining in our stack from bottom to top, separated by single slashes.
     *    If the stack is completely empty, we return the baseline root directory "/".
     */
    string simplifyPath(string path) {
        stringstream ss(path);
        string segment;
        vector<string> dirStack;

        // Split string into component segments bounded by '/' delimiters
        while (getline(ss, segment, '/')) {
            if (segment == "" || segment == ".") {
                continue;
            }
            if (segment == "..") {
                if (!dirStack.empty()) {
                    dirStack.pop_back();
                }
            } else {
                dirStack.push_back(segment);
            }
        }

        // Reconstruct canonical string path layout from structured directory indices
        string canonicalPath = "";
        for (const string& dir : dirStack) {
            canonicalPath += "/" + dir;
        }

        return canonicalPath.empty() ? "/" : canonicalPath;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N) - Where N represents the absolute character length of the raw input path string.
 *                  Tokenization and stack reductions evaluate each character slice in linear time.
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The vector structure holds directory name tokens that scale 
 *                   proportionally to the depth of the directory structure tree layout.
 */
int main() {
    cout << "=== Unix Directory Canonical Path Simplification Engine ===\n";
    cout << "Enter the absolute raw Unix path string (e.g., /home//foo/../bar/): ";
    
    string rawPath;
    if (!(cin >> rawPath)) return 1;

    Solution solver;
    cout << "\nTokenizing directory segments and evaluating navigation bounds..." << endl;
    string simplifiedOutcome = solver.simplifyPath(rawPath);

    cout << "Simplified Canonical Output Path: [" << simplifiedOutcome << "]\n";

    return 0;
}