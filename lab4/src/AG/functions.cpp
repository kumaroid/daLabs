#include "functions.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

std::string toLowerCase(const std::string& str) {
    std::string result;
    std::transform(str.begin(), str.end(), std::back_inserter(result),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<long long> LBigFunction(const std::vector<std::string>& pattern,
                                    const std::vector<long long>& n_function) {
    if (pattern.size() != n_function.size()) {
        throw std::logic_error("N function must have same size with pattern");
    }
    std::vector<long long> l_big_function(pattern.size(), 0);
    for (int j = 0; j < pattern.size() - 1; j++) {
        if (n_function[j] == 0) {
            continue;
        }
        l_big_function[pattern.size() - n_function[j]] = j + 1;
    }
    return l_big_function;
}

std::vector<long long> LLittleFunction(
    const std::vector<std::string>& pattern,
    const std::vector<long long int>& n_function) {
    if (pattern.size() != n_function.size()) {
        throw std::logic_error("N function must have same size with pattern");
    }
    std::vector<long long> l_little_function(pattern.size(), 0);
    for (int i = pattern.size() - 1; i >= 0; i--) {
        int j = (pattern.size() - 1) - i;
        if (n_function[j] == j + 1) {
            l_little_function[i] = j + 1;
        } else {
            if (i == pattern.size() - 1) {
                l_little_function[i] = 0;
            } else {
                l_little_function[i] = l_little_function[i + 1];
            }
        }
    }
    return l_little_function;
}

std::vector<long long> NFunction(const std::vector<std::string>& pattern) {
    std::vector<long long> z_function =
        ZFuncion(pattern.rbegin(), pattern.rend());
    std::reverse(z_function.begin(), z_function.end());
    return z_function;
}

std::unordered_map<std::string, std::vector<long long>> RFunction(
    const std::vector<std::string>& pattern) {
    std::unordered_map<std::string, std::vector<long long>> result;
    for (int i = pattern.size() - 1; i >= 0; i--) {
        result[toLowerCase(pattern[i])].push_back(i);
    }
    return result;
}

std::vector<long long> ZFunction(const std::vector<std::string>& pattern) {
    std::vector<long long> z_function(pattern.size(), 0);
    for (int i = 1, l = 0, r = 0; i < pattern.size(); i++) {
        if (i <= r)
            z_function[i] =
                (r - i + 1 < z_function[i - l]) ? r - i + 1 : z_function[i - l];
        while (i + z_function[i] < pattern.size() &&
               pattern[z_function[i]] == pattern[i + z_function[i]])
            ++z_function[i];
        if (i + z_function[i] - 1 > r) l = i, r = i + z_function[i] - 1;
    }
    return z_function;
}