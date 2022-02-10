#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>
#include "wordle-solver.cpp"

void usage(void);
std::string evaluate(const std::string& guess, const std::string& actual);
void prune_word_list(const std::string& guess, const std::string& result, std::vector<std::string>& words);
int convert_to_index(const std::string& result);
float get_entropy(const std::string& word, const std::vector<std::string>& words, std::vector<int>& partitions);
std::string find_max_entropy(const std::vector<std::string>& words);
std::string randomly_choose_word(const std::vector<std::string>& words);