#include "../wordle-solver.h"

// Tests a given strategy of picking words and creates a csv file with all the attempts + elapsed time.
// \param start_word std::string of length 5 of the initial guess.
// \param words reference to std::vector of std::string containing all possible words.
// \param outfile_name std::string of output csv file name.
// \param test_func function pointer to test function.
void test_strategy(const std::string& start_word, const std::vector<std::string>& words, 
                   const std::string& outfile_name, std::string (*test_func)(const std::vector<std::string>&)) {
    std::string word, guess, result;
    std::vector<int> char_count(26,0);
    std::ofstream data(outfile_name);

    auto start_time = std::chrono::steady_clock::now();
    for (size_t n = 0; n < words.size(); n++) { 
        int tries = 1;
        auto word_list_copy = words;
        auto test_word = words[n];
        guess = start_word;
        // data << guess;

        if (n%100 == 0) {
            std::cout << "\rTesting word " << n << '/' << words.size() << ": " << test_word << "     ";
        }

        while (true) {
            result = evaluate(guess, test_word, char_count);

            if (result == "ggggg") {
                data << tries << ',';
                // data << ' ' << tries << '\n';
                break;
            }

            prune_word_list(guess, result, word_list_copy); 

            if (word_list_copy.size() == 1) {
                data << ++tries << ',';
                // data << ',' << word_list_copy[0] << ' ' << ++tries << '\n';
                break;
            }
            
            guess = test_func(word_list_copy);
            // data << ',' << guess;
            ++tries;
        }
    }

    auto stop_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count();
    auto seconds = float(elapsed_time) / 1000.0;
    data << seconds;
    std::cout << "\nElapsed time: " << seconds << " s\n";
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

    test_strategy("slate", words, "./test_data/random_data.csv", randomly_choose_word);
    test_strategy("slate", words, "./test_data/entropy_data.csv", find_max_entropy);

    return 0;
}