#include "apostolico_giancarlo.h"

#include <algorithm>
#include <cctype>

std::string toLowerCase2(const std::string& str) {
    std::string result;
    std::transform(str.begin(), str.end(), std::back_inserter(result),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

int BadSymbol(
    const std::string& text_symbol, long long pattern_pos,
    const std::unordered_map<std::string, std::vector<long long>>& r) {
    std::string lower_text_symbol = toLowerCase2(text_symbol);
    if (r.count(lower_text_symbol)) {
        for (long long i : r.at(lower_text_symbol)) {
            if (i < pattern_pos) {
                return pattern_pos - i;
            }
        }
    }
    return pattern_pos + 1;
}

int GoodSuffix(const std::vector<std::string>& pattern, int pattern_pos,
               const std::vector<long long>& l_big,
               const std::vector<long long>& l_little) {
    if (pattern_pos == -1) {
        if (pattern.size() > 1) {
            return pattern.size() - l_little[1];
        } else {
            return 1;
        }
    }
    if (pattern_pos == pattern.size() - 1) {
        return 1;
    }
    if (l_big[pattern_pos + 1] > 0) {
        return pattern.size() - l_big[pattern_pos + 1];
    }
    return pattern.size() - l_little[pattern_pos + 1];
}

std::vector<std::pair<long long, long long>> ApostolicoGiancarlo(
    const std::vector<std::string>& pattern, const std::vector<Word>& text) {
    std::vector<long long> n_function = NFunction(pattern);
    std::vector<long long> big_l_function = LBigFunction(pattern, n_function);
    std::vector<long long> little_l_function =
        LLittleFunction(pattern, n_function);
    std::unordered_map<std::string, std::vector<long long>> r_function =
        RFunction(pattern);

    std::vector<std::pair<long long, long long>> entries;
    int k = pattern.size() - 1;
    std::vector<int> m(text.size());
    while (k < text.size()) {
        int i = pattern.size() - 1;
        int h = k;
        while (true) {
            if (m[h] == 0) {
                if (toLowerCase2(text[h].symbol) == toLowerCase2(pattern[i])) {
                    if (i == 0) {
                        int entry_position = k - pattern.size() + 1;
                        entries.emplace_back(
                            text[entry_position].str_number,
                            text[entry_position]
                                .position);  // сообщить о вхождении
                        m[k] =
                            pattern.size();  // подстрока [k - pattern.size() +
                                             // 1 .. k] совпадает с образцом
                        // такой сдвиг, что префикс образца должен совпасть с
                        // суффиксом
                        if (pattern.size() > 2) {
                            k += pattern.size() - little_l_function[1];
                        } else {
                            k++;
                        }
                        break;  // конец фазы
                    } else {    // если  i > 0
                        h--;
                        i--;
                    }
                } else {  // несовпадение
                    m[k] = k - h;
                    // нахождение хорошего суффикса
                    int maxSuff = GoodSuffix(pattern, i, big_l_function,
                                             little_l_function);
                    // нахождение плохого символа
                    int maxSymb =
                        BadSymbol(toLowerCase2(text[h].symbol), i, r_function);
                    k += std::max(maxSuff, maxSymb);
                    break;  // конец фазы
                }
            } else if (m[h] < n_function[i]) {
                i -= m[h];
            } else if (m[h] >= n_function[i] && n_function[i] >= i) {
                int entry_position = k - pattern.size() + 1;
                entries.emplace_back(
                    text[entry_position].str_number,
                    text[entry_position].position);  // сообщить о вхождении
                m[k] = k - h;
                // такой сдвиг, что префикс образца должен совпасть с суффиксом
                if (pattern.size() > 2) {
                    k += pattern.size() - little_l_function[1];
                } else {
                    k++;
                }
                break;  // конец фазы
            } else if (m[h] > n_function[i] && n_function[i] < i) {
                m[k] = k - h;
                // несовпадение в i - n[i]
                // нахождение хорошего суффикса
                int pattern_pos = i - n_function[i];
                std::string text_word =
                    toLowerCase2(text[h - n_function[i]].symbol);
                int maxSuff = GoodSuffix(pattern, pattern_pos, big_l_function,
                                         little_l_function);
                int maxSymb = BadSymbol(text_word, pattern_pos, r_function);
                k += std::max(maxSuff, maxSymb);
                break;  // конец фазы
            } else if (m[h] == n_function[i] && n_function[i] < i) {
                i -= m[h];
                h -= m[h];
            }
        }
    }

    return entries;
}
