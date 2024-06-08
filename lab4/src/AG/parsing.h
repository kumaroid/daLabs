#pragma once

#include <iostream>
#include <string>
#include <vector>

struct Word {
    explicit Word(std::string _symbol, long long _str_number,
                  long long _position);

    std::string symbol;
    long long str_number;
    long long position;
};

void ParsePattern(std::vector<std::string>& pattern,
                  std::istream& is = std::cin);
void ParseText(std::vector<Word>& text, std::istream& is = std::cin);
