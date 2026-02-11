#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <compare>
#include <iostream>

class long_int
{
  private:
    std::vector<uint64_t> words;

    void trim() {
      while (words.size() > 1 && words.back() == 0) {
        words.pop_back();
      }
    }

  public:
    long_int() : words{0} {}

    long_int(int n) {
      if (n == 0) {
        words = {0};
      } else {
        words = {static_cast<uint64_t>(n)};
      }
    }

    long_int(const std::string& decimal_str) {
      *this = 0;

      for (char c : decimal_str) {
        if (c < '0' || c > '9') continue;

        *this = (*this << 3) + (*this << 1);
        *this = *this + long_int(c - '0');
      }
    }

    long_int operator<<(int shift) const {
      if (shift == 0) return *this;

      long_int result;
      int word_shift = shift / 64;
      int bit_shift = shift % 64;

      result.words.resize(words.size() + word_shift + 1, 0);

      if (bit_shift == 0) {
        std::copy(words.begin(), words.end(), result.words.begin() + word_shift);
      } else {
        uint64_t carry = 0;
        for (size_t i = 0; i < words.size(); i++) {
          uint64_t val = words[i];
          result.words[i + word_shift] = (val << bit_shift) | carry;
          carry = val >> (64 - bit_shift);
        }
        if (carry) {
          result.words[words.size() + word_shift] = carry;
        }
      }

      result.trim();
      return result;
    }

    long_int operator>>(int shift) const {
      if (shift == 0) return *this;

      int word_shift = shift / 64;
      int bit_shift = shift % 64;

      if (word_shift >= words.size()) return long_int(0);

      long_int result;
      result.words.resize(words.size() - word_shift, 0);

      if (bit_shift == 0) {
        std::copy(words.begin() + word_shift, words.end(), result.words.begin());
      } else {
        for (size_t i = word_shift; i < words.size(); i++) {
          result.words[i - word_shift] = words[i] >> bit_shift;
          if (i + 1 < words.size()) {
            result.words[i - word_shift] |= words[i + 1] << (64 - bit_shift);
          }
        }
      }

      result.trim();
      return result;
    }

    long_int operator*(const long_int& other) const {
      int bits_set = 0;
      int total_shift = 0;

      for (size_t i = 0; i < other.words.size(); i++) {
        bits_set += __builtin_popcountll(other.words[i]);
        if (bits_set > 1) {
          goto general_multiply;
        }
        if (bits_set == 1 && other.words[i] != 0) {
          total_shift = i * 64 + __builtin_ctzll(other.words[i]);
        }
      }

      if (bits_set == 0) return long_int(0);

      return *this << total_shift;

general_multiply:

      long_int result;
      result.words.resize(words.size() + other.words.size(), 0);

      for (size_t i = 0; i < words.size(); i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < other.words.size(); j++) {
          __uint128_t prod = (__uint128_t)words[i] * other.words[j] + 
            result.words[i + j] + carry;
          result.words[i + j] = prod & 0xFFFFFFFFFFFFFFFF;
          carry = prod >> 64;
        }
        if (carry) {
          result.words[i + other.words.size()] += carry;
        }
      }

      result.trim();
      return result;
    }

    long_int operator+(const long_int& other) const {
      size_t max_size = std::max(words.size(), other.words.size());
      long_int result;
      result.words.resize(max_size + 1, 0);

      uint64_t carry = 0;
      for (size_t i = 0; i < max_size; i++) {
        uint64_t a = i < words.size() ? words[i] : 0;
        uint64_t b = i < other.words.size() ? other.words[i] : 0;

        __uint128_t sum = (__uint128_t)a + b + carry;
        result.words[i] = sum & 0xFFFFFFFFFFFFFFFF;
        carry = sum >> 64;
      }

      if (carry) {
        result.words[max_size] = carry;
      }

      result.trim();
      return result;
    }

    long_int operator-(const long_int& other) const {
      long_int result;
      result.words.resize(words.size(), 0);

      uint64_t borrow = 0;
      for (size_t i = 0; i < words.size(); i++) {
        uint64_t a = words[i];
        uint64_t b = i < other.words.size() ? other.words[i] : 0;

        if (a >= b + borrow) {
          result.words[i] = a - b - borrow;
          borrow = 0;
        } else {
          result.words[i] = (UINT64_MAX - b - borrow + 1) + a;
          borrow = 1;
        }
      }

      result.trim();
      return result;
    }

    bool operator==(const long_int& other) const = default;

    std::strong_ordering operator<=>(const long_int& other) const {
      if (words.size() != other.words.size()) {
        return words.size() <=> other.words.size();
      }

      for (int i = words.size() - 1; i >= 0; i--) {
        if (words[i] != other.words[i]) {
          return words[i] <=> other.words[i];
        }
      }

      return std::strong_ordering::equal;
    }

    std::string to_string() const {
      if (words.size() == 1 && words[0] == 0) return "0";

      long_int num = *this;
      std::string result;

      long_int ten(10);
      long_int zero(0);

      while (num > zero) {
        long_int remainder = num;
        long_int quotient;
        quotient.words.resize(num.words.size(), 0);

        uint64_t r = 0;
        for (int i = num.words.size() - 1; i >= 0; i--) {
          __uint128_t dividend = ((__uint128_t)r << 64) | num.words[i];
          quotient.words[i] = dividend / 10;
          r = dividend % 10;
        }

        quotient.trim();
        result += ('0' + r);
        num = quotient;
      }

      std::reverse(result.begin(), result.end());
      return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const long_int& num) {
      os << num.to_string();
      return os;
    }
};
