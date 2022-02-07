#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <chrono>

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
    temp.clear();
}

// Returns random word from 'words'.
// \param words std::vector of std::string containing all possible words remaining.
// \return std::string of the word.
std::string randomly_choose_word(std::vector<std::string>& words) {
    return words[std::rand() % words.size()];
}

// Finds and returns the word with the maximum entropy among 'words'.
// \param words std::vector of std::string containing all possible words remaining.
// \return std::string of the word.
std::string find_max_entropy(std::vector<std::string>& words) {
    // TODO
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
        words.push_back(word); 
    }

    // TEST SECTION
    auto start_time = std::chrono::steady_clock::now();
    std::vector<int> attempts;

    for (size_t n = 0; n < words.size(); n++) {
        std::vector<std::string> temp_list = words; 
        word = words[n];
        int tries = 1;

        if (n % 20 == 0)
            std::cout << "\rTrying " << word << ", word " << n << '/' << words.size();
        guess = "crane"; // initial guess

        while (true) {
            result = evaluate(guess, word);
            prune_word_list(guess, result, temp_list); 
            if (temp_list.size() == 1) {
                attempts.push_back(tries);
                break;
            }
            tries += 1;
            guess = randomly_choose_word(temp_list);
        }
    }

    auto stop_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count();
    auto seconds = float(elapsed_time) / 1000.0;
    std::cout << "\nElapsed time: " << seconds << 's' << std::endl;
    
    std::ofstream data("data.csv");
    for (auto i : attempts) {
        data << i << ',';
    }

    return 0;
}