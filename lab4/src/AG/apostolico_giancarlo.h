#pragma once
#include "functions.h"
#include "parsing.h"

int BadSymbol(const std::string& text_symbol, long long pattern_pos,
              const std::unordered_map<std::string, std::vector<long long>>& r);

int GoodSuffix(const std::vector<std::string>& pattern, int pattern_pos,
               const std::vector<long long>& l_big,
               const std::vector<long long>& l_little);

std::vector<std::pair<long long, long long>> ApostolicoGiancarlo(
    const std::vector<std::string>& pattern, const std::vector<Word>& text);