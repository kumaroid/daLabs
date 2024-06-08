#include "parsing.h"

#include <string>

Word::Word(std::string _symbol, long long _str_number, long long _position)
    : symbol(_symbol), str_number(_str_number), position(_position) {}

void ParsePattern(std::vector<std::string>& pattern, std::istream& is) {
    std::string symbol;
    while (true) {
        if (!is.good() || is.peek() == '\n') {
            break;
        } else {
            std::string current;
            is >> current;
            for (auto& c : current)
                c = std::tolower(c);  // convert to lowercase
            pattern.push_back(current);
        }
    }
}

void ParseText(std::vector<Word>& text, std::istream& is) {
    char symbol = is.peek();
    long long cur_str = 1, cur_pos = 1;
    while (!is.eof()) {
        if (symbol == ' ' || symbol == '\n' || symbol == '\t') {
            is.get();
            if (symbol == '\n') {
                cur_str++;
                cur_pos = 1;
            }
        } else {
            std::string current;
            is >> current;
            for (auto& c : current)
                c = std::tolower(c);  // convert to lowercase
            text.push_back(Word{current, cur_str, cur_pos});
            cur_pos++;
        }
        symbol = is.peek();
    }
}
