#include "wordle-solver.h"

// Prints usage instructions.
void usage(void) {
    std::cout << "Usage: ./wordle-solver.exe <filename of word list>" << std::endl;
}

// Returns the evaluation of 'guess' against 'actual' as if it were an actual Wordle game: 
// 'b' for a black box, 'y' for a yellow box, and 'g' for a green box.
// \param guess std::string of length 5 containing the guess.
// \param actual std::string of length 5 containing the actual word.
// \param char_count std::vector of int of length 26 for the character counts.
// \return std::string of length 5 containing the evaluation.
std::string evaluate(const std::string& guess, const std::string& actual, std::vector<int>& char_count) {
    std::string result = "aaaaa";
    std::fill(char_count.begin(), char_count.end(), 0);
    for (auto c : actual) {
        char_count[int(c)%26] += 1;
    }

    for (int i = 0; i < 5; i++) {
        if (guess[i] == actual[i]) {
            result[i] = 'g';
            char_count[guess[i]%26] -= 1;
        } 
    }
    
    for (int i = 0; i < 5; i++) { 
        if (result[i] == 'g') {
            continue;
        }
        if (guess[i] != actual[i] && char_count[guess[i]%26] > 0) {
            result[i] = 'y';
            char_count[guess[i]%26] -= 1;
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
    std::vector<int> char_count(26,0);
    for (auto word : words) {
        if (evaluate(guess, word, char_count) == result) {
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
        // index += (c == 'b') * (2 * std::pow(3, 4 - i)) + (c == 'y') * std::pow(3, 4 - i);
        if (c == 'b') {
            index += 2 * std::pow(3, 4 - i);
        } else if (c == 'y') {
            index += std::pow(3, 4 - i);
        }
    }
    return index;
}

// Returns the entropy of the result distribution for a given word.
// \param word std::string of length 5 containing the guess.
// \param words std::vector of std::string containing all possible words remaining.
// \param partitions std::vector of int containing enough elements for every possible combination of game results. (243 for 5-word Wordle)
// \return float of entropy value.
float get_entropy(const std::string& word, const std::vector<std::string>& words, std::vector<int>& partitions) {
    // TODO: implement a way to choose differentiate between similar words within a single partition
    std::fill(partitions.begin(), partitions.end(), 0);
    std::vector<int> char_count(26,0);
    int total = words.size();

    for (auto& other : words) {
        if (word == other) {
            continue;
        }
        int index = convert_to_index(evaluate(other, word, char_count));
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

static std::vector<std::string> bad_guesses {"weird", "price", "antic", "corny", "baste"};

// Finds and returns the word with the maximum entropy among 'words'.
// \param words std::vector of std::string containing all possible words remaining.
// \return std::string of word.
std::string find_max_entropy(const std::vector<std::string>& words) {
    std::string max_entropy_word;
    std::vector<int> partitions(243,0);
    float max_entropy = 0.0;
    float entropy = 0.0;

    for (auto& word : words) {
        entropy = get_entropy(word, words, partitions);

        if (entropy > max_entropy &&
            !(std::any_of(bad_guesses.begin(), bad_guesses.end(), 
              [&](const std::string& bad_guess){ return bad_guess == word; }) &&
            words.size() > 10)) // exclude word if it is part of bad_guesses and words.size() > 10
        {
            max_entropy = entropy;
            max_entropy_word = word;
        }
    }
    return max_entropy_word;
}

std::string randomly_choose_word(const std::vector<std::string>& words) {
    return words[std::rand() % words.size()];
}