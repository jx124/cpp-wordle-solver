#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

// Prints usage instructions.
void usage(void) {
    std::cout << "Usage: ./wordle-solver.exe <filename of word list>" << std::endl;
}

// Returns the evaluation of 'guess' against 'actual' as if it were an actual Wordle game: 
// 'b' for a black box, 'y' for a yellow box, and 'g' for a green box.
// \param guess std::string of length 5 containing the guess.
// \param actual std::string of length 5 containing the actual word.
// \return std::string of length 5 containing the evaluation.
std::string evaluate(const std::string guess, const std::string actual) {
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
// \param result std::string of length 5  containing the result. 'b' for a black box, 'y' for a yellow box, and 'g' for a green box.
// \param words std::vector of std::string containing all possible words remaining.
void prune_word_list(const std::string guess, const std::string result, std::vector<std::string>& words) {
    std::vector<std::string> temp;
    for (auto word : words) {
        if (evaluate(guess, word) == result) {
            temp.push_back(word);
        }
    }
    words = temp;
}

// Finds and returns the word with the maximum entropy among 'words'.
// \param words std::vector of std::string containing all possible words remaining.
// \return std::string of the word.
std::string find_max_entropy(std::vector<std::string>& words) {
    // TODO: implement partition algorithm
    return words[std::rand() % words.size()];
}

int main(int argc, const char** argv) {
    if (argc != 2) {
        usage();
        return 0;
    } 

    std::ifstream word_file(argv[1]);

    if (!word_file.is_open()) {
        std::cout << "File not found/invalid. Please place word list file in the same directory as solver.\n";
        usage();
        return 0;
    }
    
    std::string word, guess, result;
    std::vector<std::string> words;
    while (word_file >> word) { 
        words.push_back(word); // copies words from the list into a vector
    }

    std::cout << "Enter guess: ";
    std::cin >> guess;

    while (true) {
        
        std::cout << "Enter result string: ";
        std::cin >> result;
        std::cout << "Pruning list..." << '\n';
        prune_word_list(guess, result, words); 
        std::cout << "Words remaining: " << words.size() << '\n';

        if (words.size() == 1) {
            std::cout << "Found solution: " << words[0] << std::endl;
            return 0;
        }
        
        word = find_max_entropy(words);
        std::cout << "Try: " << word << '\n';
        guess = word;
    }

    return 0;
}