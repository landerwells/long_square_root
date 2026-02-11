#pragma once

#include <compare>
#include <string> 
#include <vector>

class long_int
{
  public:
  long_int();
  long_int(int);
  long_int(const std::string& str);

  std::vector<int> digits();

  long_int operator+(const long_int&) const;
  long_int operator-(const long_int&) const;
  long_int operator*(const long_int&) const;
  std::strong_ordering operator<=>(const long_int& other) const;
  bool operator==(const long_int& other) const = default;

  private:
  std::vector<int> data{};
};
