#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<int> computeLCSLength(const std::vector<std::string>&A, const std::vector<std::string>& B) {
    std::vector<int> prev(B.size() + 1, 0), curr(B.size() + 1, 0);

    for (const auto& wordA : A) {
        std::swap(prev, curr);
        for (size_t j = 1; j <= B.size(); ++j) {
            if (wordA == B[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = std::max(curr[j - 1], prev[j]);
            }
        }   
    }
    return curr;
}

std::vector<std::string> hirschberg(const std::vector<std::string>& A, const std::vector<std::string>& B) {
    if (A.empty() || B.empty()) {
        return {};
    }

    if (A.size() == 1) {
        for (const auto& word : B) {
            if (word == A[0]) {
                return {A[0]};
            }
        }
    return {};
    }

    size_t mid = A.size() / 2;
    auto L1 = computeLCSLength({A.begin(), A.begin() + mid}, B);
    auto L2 = computeLCSLength({A.rbegin(), A.rbegin() + (A.size() - mid)}, {B.rbegin(), B.rend()});

    size_t split = 0, max_sum = 0;
    for (size_t i = 0; i <= B.size(); ++i) {
        if (L1[i] + L2[B.size() - i] > max_sum) {
            max_sum = L1[i] + L2[B.size() - i];
            split = i;
        }
    }
    auto leftLCS = hirschberg({A.begin(), A.begin() + mid}, {B.begin(), B.begin() + split});
    auto rightLCS = hirschberg({A.begin() + mid, A.end()}, {B.begin() + split, B.end()});

    leftLCS.insert(leftLCS.end(), rightLCS.begin(), rightLCS.end());
    return leftLCS;
}


int main() {
    std::string line1, line2;
    std::getline(std::cin, line1);
    std::getline(std::cin, line2);

    std::vector<std::string> A, B;
    std::istringstream iss1(line1), iss2(line2);
    std::string word;

    while (iss1 >> word) A.push_back(word);
    while (iss2 >> word) B.push_back(word);

    auto result = hirschberg(A, B);

    std::cout << result.size() << "\n";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i != result.size() - 1) std::cout << " ";
    }
    std::cout << "\n";
    
    return 0;
}