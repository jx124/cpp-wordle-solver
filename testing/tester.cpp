#include "../wordle-solver.h"

std::string randomly_choose_word(const std::vector<std::string>& words) {
    return words[std::rand() % words.size()];
}

// Tests a given strategy of picking words and creates a csv file with all the attempts + elapsed time.
// \param start_word std::string of length 5 of the initial guess.
// \param words reference to std::vector of std::string containing all possible words.
// \param outfile_name std::string of output csv file name.
// \param test_func function pointer to test function.
void test_strategy(const std::string& start_word, const std::vector<std::string>& words, 
                   const std::string& outfile_name, std::string (*test_func)(const std::vector<std::string>&)) {
    std::vector<int> attempts;
    std::string word, guess, result;
    auto start_time = std::chrono::steady_clock::now();

    for (size_t n = 0; n < words.size(); n++) {
        std::vector<std::string> temp_list = words; 
        guess = start_word;
        word = words[n];
        int tries = 1;

        if (n % 20 == 0) {
            std::cout << "\rTrying " << word << ", word " << n << '/' << words.size();
        }

        while (true) {
            result = evaluate(guess, word);
            prune_word_list(guess, result, temp_list); 
            if (temp_list.size() == 1) {
                attempts.push_back(tries);
                break;
            }
            tries += 1;
            guess = test_func(temp_list);
        }
    }

    auto stop_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count();
    auto seconds = float(elapsed_time) / 1000.0;

    std::ofstream data(outfile_name);
    for (auto attempt : attempts) {
        data << attempt << ',';
    }
    data << seconds;
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
    
    std::string word;
    std::vector<std::string> words;
    while (word_file >> word) { 
        words.push_back(word); 
    }

    test_strategy("slate", words, "random_data.csv", randomly_choose_word);
    test_strategy("slate", words, "entropy_data.csv", find_max_entropy);

    return 0;
}