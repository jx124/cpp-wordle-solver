#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>
#include "wordle-solver.h"

// Prints usage instructions.
void usage(void) {
    std::cout << "Usage: ./wordle-solver.exe <filename of word list>" << std::endl;
}

// Returns the evaluation of 'guess' against 'actual' as if it were an actual Wordle game: 
// 'b' for a black box, 'y' for a yellow box, and 'g' for a green box.
// \param guess std::string of length 5 containing the guess.
// \param actual std::string of length 5 containing the actual word.
// \return std::string of length 5 containing the evaluation.
std::string evaluate(const std::string& guess, const std::string& actual) {
    std::string result = "aaaaa";
    std::unordered_map<char, int> char_count;
    for (auto c : actual) {
        char_count[c] += 1;
    }

    for (int i = 0; i < 5; i++) {
        if (guess[i] == actual[i]) {
            result[i] = 'g';
            char_count[guess[i]] -= 1;
        } 
    }
    
    for (int i = 0; i < 5; i++) { 
        if (result[i] == 'g') {
            continue;
        }
        if (guess[i] != actual[i] && char_count[guess[i]] > 0) {
            result[i] = 'y';
            char_count[guess[i]] -= 1;
        } else {
            result[i] = 'b';
        }
    }
    return result;
}

// Removes elements from 'words' that do not fit 'result'. 
// \param guess std::string of length 5 containing the guess.
// \param result std::string of length 5 containing the result. 'b' for a black box, 'y' for a yellow box, and 'g' for a green box.
// \param words std::vector of std::string containing all possible words remaining.
void prune_word_list(const std::string& guess, const std::string& result, std::vector<std::string>& words) {
    std::vector<std::string> temp;
    for (auto word : words) {
        if (evaluate(guess, word) == result) {
            temp.push_back(word);
        }
    }
    words = temp;
}

// Converts result string to index in partition. 
// \param std::string of length 5 containing the result.
// \return int of index
int convert_to_index(const std::string& result) { // TODO: update to accomodate arbitrary length games.
    int index = 0;
    for (int i = 0; i < 5; i++) {
        char c = result[i];
        if (c == 'b') {
            index += 2 * std::pow(3, 4 - i);
        } else if (c == 'y') {
            index += std::pow(3, 4 - i);
        }
    }
    return index;
}

// Returns the entropy of the result distribution for a given word.
// \param word A std::string of length 5 containing the guess.
// \param words A reference to std::vector of std::string containing all possible words remaining.
// \param partitions A reference to std::vector of int containing enough elements for every possible combination of game results. (243 for 5-word Wordle)
// \return float of entropy value.
float get_entropy(const std::string& word, const std::vector<std::string>& words, std::vector<int>& partitions) {
    std::fill(partitions.begin(), partitions.end(), 0);
    int total = words.size();

    for (auto other : words) {
        if (word == other) {
            continue;
        }
        int index = convert_to_index(evaluate(other, word));
        partitions[index] += 1;
    }

    float entropy = 0.0;
    for (int i = 0; i < 243; i++) {
        int members = partitions[i];
        if (members == 0) {
            continue;
        }
        float p = float(members) / total; //probability
        entropy -= p * std::log2(p);
    }

    return entropy;
}

// Finds and returns the word with the maximum entropy among 'words'.
// \param words std::vector of std::string containing all possible words remaining.
// \return std::string of word.
std::string find_max_entropy(const std::vector<std::string>& words) {
    std::string max_entropy_word;
    std::vector<int> partitions(243,0);
    float max_entropy = 0.0;
    float entropy = 0.0;

    for (auto word : words) {
        entropy = get_entropy(word, words, partitions);

        if (entropy > max_entropy) {
            max_entropy = entropy;
            max_entropy_word = word;
        }
    }
    return max_entropy_word;
}