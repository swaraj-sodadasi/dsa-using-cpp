#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
CORE LOGIC (BACKTRACKING WITH COMBINATORIAL PRUNING):
- The objective is to find the maximum score achievable by forming a subset of valid words from a limited pool of letters.
- Since the word pool is typically small (e.g., N <= 14), we can explore the entire combination space using a backtracking approach.
- For each word at the current `index`, we branch into two standard decisions:
  1. **Exclude**: Skip the current word and move directly to evaluate `index + 1`.
  2. **Include**: Verify if the remaining letters in our global pool can satisfy the character requirements of the current word.
- **Combinatorial Pruning**: During the inclusion check, if any letter's demand exceeds its current availability in our pool, we instantly abort that path (prune the branch), avoiding deep and illegal recursive steps.
- **Restoration Step (Backtracking)**: If a word is successfully included, we deduct its characters from the letter pool, add its score to our current running total, and dive deeper. Upon returning from the recursion, we restore those exact characters back to the pool to keep the state pristine for alternative combinations.
- We maintain a global `maxScore` variable that captures the peak score encountered when we successfully hit our base case (`index == words.size()`).

TIME COMPLEXITY: O(2^N * L) - Where N is the total number of words (yielding 2^N state choices) and L is the maximum length of a word processed during the validation pass.
SPACE COMPLEXITY: O(N) - The auxiliary memory is bound strictly by the depth of the recursive execution call stack.
*/

// Helper function to calculate the score of a single word based on the character value chart
int getWordScore(const string& word, const vector<int>& score) {
    int wordScore = 0;
    for (char c : word) {
        if (c >= 'a' && c <= 'z') {
            wordScore += score[static_cast<size_t>(c - 'a')];
        }
    }
    return wordScore;
}

void backtrack(size_t index, const vector<string>& words, const vector<int>& score, 
               vector<int>& letterCounts, int currentScore, int& maxScore) {
    // Base Case: If we have processed all available words, update the maximum global score
    if (index == words.size()) {
        maxScore = max(maxScore, currentScore);
        return;
    }

    // Choice 1: Exclude the current word and proceed to the next index
    backtrack(index + 1, words, score, letterCounts, currentScore, maxScore);

    // Choice 2: Attempt to include the current word
    const string& currentWord = words[index];
    bool canForm = true;
    
    // Track the letter counts required exclusively by this specific word
    vector<int> usedLetters(26, 0);
    for (char c : currentWord) {
        // Enforce strict character alignment bounds constraint validation
        if (c < 'a' || c > 'z') {
            canForm = false;
            break;
        }
        
        int idx = c - 'a';
        usedLetters[static_cast<size_t>(idx)]++;
        
        // Combinatorial Pruning: Stop processing immediately if demand exceeds availability
        if (usedLetters[static_cast<size_t>(idx)] > letterCounts[static_cast<size_t>(idx)]) {
            canForm = false; 
            break; 
        }
    }

    // If the word can be safely formed from our current pool, execute the recursive inclusion branch
    if (canForm) {
        // Deduct the word's letters from the main tracking pool
        for (int i = 0; i < 26; ++i) {
            letterCounts[static_cast<size_t>(i)] -= usedLetters[static_cast<size_t>(i)];
        }
        int earnedScore = getWordScore(currentWord, score);

        // Advance to the next word with the updated score state
        backtrack(index + 1, words, score, letterCounts, currentScore + earnedScore, maxScore);

        // Restoration Step (Backtrack): Re-add the letters to restore the state for subsequent branches
        for (int i = 0; i < 26; ++i) {
            letterCounts[static_cast<size_t>(i)] += usedLetters[static_cast<size_t>(i)];
        }
    }
}

int maxScoreWords(const vector<string>& words, const vector<char>& letters, const vector<int>& score) {
    // Generate the baseline availability signature map for the entire letter pool
    vector<int> letterCounts(26, 0);
    for (char c : letters) {
        if (c >= 'a' && c <= 'z') {
            letterCounts[static_cast<size_t>(c - 'a')]++;
        }
    }

    int maxScore = 0;
    backtrack(0, words, score, letterCounts, 0, maxScore);
    return maxScore;
}

int main() {
    int numWords;
    cout << "Enter the number of words: ";
    if (!(cin >> numWords) || numWords < 0) return 0;

    vector<string> words(static_cast<size_t>(numWords));
    cout << "Enter the words separated by spaces:\n";
    for (int i = 0; i < numWords; ++i) {
        cin >> words[static_cast<size_t>(i)];
    }

    int numLetters;
    cout << "Enter the total number of letters in the pool: ";
    if (!(cin >> numLetters) || numLetters < 0) return 0;

    vector<char> letters(static_cast<size_t>(numLetters));
    cout << "Enter the pool letters separated by spaces:\n";
    for (int i = 0; i < numLetters; ++i) {
        cin >> letters[static_cast<size_t>(i)];
    }

    vector<int> score(26);
    cout << "Enter 26 integer scores corresponding to letters 'a' through 'z':\n";
    for (int i = 0; i < 26; ++i) {
        cin >> score[static_cast<size_t>(i)];
    }

    int result = maxScoreWords(words, letters, score);
    cout << "\nThe maximum score possible from forming valid word combinations is: " << result << endl;

    return 0;
}