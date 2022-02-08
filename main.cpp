#include "wordle-solver.h"

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

    std::cout << "Enter guess: ";
    std::cin >> guess;

    while (true) {
        
        std::cout << "Enter result string: ";
        std::cin >> result;
        prune_word_list(guess, result, words); 
        std::cout << "Words remaining: " << words.size() << '\n';

        if (words.size() == 1) {
            std::cout << "Found solution: " << words[0] << std::endl;
            return 0;
        } else if (words.size() < 1) {
            std::cout << "You probably entered something wrongly. Exiting." << std::endl;
            return 0;
        }
        
        word = find_max_entropy(words);
        std::cout << "Try: " << word << '\n';
        guess = word;
    }

    return 0;
}