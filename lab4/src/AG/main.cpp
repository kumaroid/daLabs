#include <iostream>
#include <vector>
#include "apostolico_giancarlo.h"
#include "parsing.h"

template <class T>
std::ostream& operator << (std::ostream& os, std::vector<T> container) {
    bool first = true;
    for (auto i : container) {
        if (!first) {
            os << " ";
        } else {
            first = false;
        }
        os << i;
    }
    return os;
}

void PrintResult(const std::vector<std::pair<long long, long long>>& res, std::ostream& os = std::cout) {
    for (const std::pair<long long, long long>& p : res) {
        os << p.first << ", " << p.second << "\n";
    }
}

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    std::vector<std::string> pattern;
    std::vector<Word> text;

    ParsePattern(pattern);
    ParseText(text);

    // Lowercasing the pattern
    std::transform(pattern.begin(), pattern.end(), pattern.begin(), [](const std::string &s) {
        std::string result;
        std::transform(s.begin(), s.end(), std::back_inserter(result),
                       [](unsigned char c){ return std::tolower(c); });
        return result;
    });

    // Lowercasing the words in text
    for(auto& word : text) {
        std::transform(word.symbol.begin(), word.symbol.end(), word.symbol.begin(),
                       [](unsigned char c){ return std::tolower(c); });
    }

    std::vector<std::pair<long long, long long>> result = ApostolicoGiancarlo(pattern, text);
    PrintResult(result);

    return 0;
}
