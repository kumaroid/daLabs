#include <iostream>
#include <string>
#include <vector>

std::vector<int> calculateZ(const std::string &input) {
  int n = input.length();
  std::vector<int> Z(n);
  Z[0] = 0;
  int left = 0;
  int right = 0;

  for (int k = 1; k < n; ++k) {
    if (k > right) {
      left = right = k;
      while (right < n && input[right] == input[right - left]) {
        right++;
      }
      Z[k] = right - left;
      right--;
    } else {
      int k1 = k - left;
      if (Z[k1] < right - k + 1) {
        Z[k] = Z[k1];
      } else {
        left = k;
        while (right < n && input[right] == input[right - left]) {
          right++;
        }
        Z[k] = right - left;
        right--;
      }
    }
  }
  return Z;
}

void searchPattern(const std::string &text, const std::string &pattern) {
  std::string concat = pattern + "$" + text;
  int pattern_size = pattern.size();
  std::vector<int> Z = calculateZ(concat);

  for (int i = 0; i < Z.size(); ++i) {
    if (Z[i] == pattern_size) {
      std::cout << i - pattern_size - 1 << '\n';
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  std::string text;
  std::string pattern;

  std::cin >> text;
  std::cin >> pattern;

  searchPattern(text, pattern);

  return 0;
}
