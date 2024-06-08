#pragma once

#include <algorithm>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<long long> LBigFunction(const std::vector<std::string>& pattern,
                                    const std::vector<long long>& n_function);

std::unordered_map<std::string, std::vector<long long>> RFunction(
    const std::vector<std::string>& pattern);

std::vector<long long> NFunction(const std::vector<std::string>& pattern);

std::vector<long long> LLittleFunction(
    const std::vector<std::string>& pattern,
    const std::vector<long long>& n_function);

template <typename RandomAccessIterator>
std::vector<long long> ZFuncion(RandomAccessIterator begin,
                                RandomAccessIterator end);
