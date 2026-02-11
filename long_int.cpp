#include "long_int.h"

#include <algorithm>

long_int::long_int() : data{} {}

long_int::long_int(int n)
{
  if (n == 0)
  {
    data.push_back(0);
    return;
  }

  while (n > 0)
  {
    data.push_back(n % 10);
    n /= 10;
  }
  std::reverse(data.begin(), data.end());
}

long_int::long_int(const std::string& n)
{
  if (n.empty() || n == "0")
  {
    data.push_back(0);
    return;
  }

  for (char c : n)
  {
    if (c < '0' || c > '9')
    {
      data.clear();
      data.push_back(0);
      return;
    }
    data.push_back(c - '0');
  }

  // Trim leading zeros
  while (data.size() > 1 && data[0] == 0)
  {
    data.erase(data.begin());
  }
}

std::strong_ordering long_int::operator<=>(const long_int& other) const
{
  if (data.size() != other.data.size())
  {
    return data.size() <=> other.data.size();
  }

  for (int i = data.size() - 1; i >= 0; --i)
  {
    if (data[i] != other.data[i])
    {
      return data[i] <=> other.data[i];
    }
  }

  return std::strong_ordering::equal;
}

long_int long_int::operator+(const long_int& other) const
{
  int i = data.size() - 1;
  int j = other.data.size() - 1;
  long_int sum;
  int carry = 0;

  while (i >= 0 || j >= 0 || carry > 0)
  {
    int inner_sum = carry;

    if (i >= 0)
    {
      inner_sum += data[i];
      i--;
    }

    if (j >= 0)
    {
      inner_sum += other.data[j];
      j--;
    }

    carry = inner_sum / 10;
    sum.data.push_back(inner_sum % 10);
  }

  std::reverse(sum.data.begin(), sum.data.end());

  return sum;
}

long_int long_int::operator-(const long_int& other) const
{
  long_int a = *this;  // Make a copy we can modify
  long_int b = other;  // Make a copy we can modify

  // Pad with leading zeros to same length
  while (a.data.size() < b.data.size())
  {
    a.data.insert(a.data.begin(), 0);
  }
  while (b.data.size() < a.data.size())
  {
    b.data.insert(b.data.begin(), 0);
  }

  // Nine's complement of b
  for (size_t i = 0; i < b.data.size(); i++)
  {
    b.data[i] = 9 - b.data[i];
  }

  // Add a + b + 1, then remove leading digit
  long_int one(1);
  long_int sum1 = a + b;
  long_int sum2 = sum1 + one;

  // Remove the leading digit (equivalent to substr(1))
  long_int diff;
  if (sum2.data.size() > 1)
  {
    diff.data.assign(sum2.data.begin() + 1, sum2.data.end());
  }
  else
  {
    diff.data = {0};
  }

  while (diff.data.size() > 1 && diff.data[0] == 0)
  {
    diff.data.erase(diff.data.begin());
  }

  return diff;
}

long_int long_int::operator*(const long_int& other) const
{
  long_int zero(0);
  if (*this == zero || other == zero)
  {
    return zero;
  }

  long_int product;
  product.data.resize(data.size() + other.data.size(), 0);

  for (int i = data.size() - 1; i >= 0; i--)
  {
    for (int j = other.data.size() - 1; j >= 0; j--)
    {
      int temp = product.data[i + j + 1] + data[i] * other.data[j];
      product.data[i + j + 1] = temp % 10;
      product.data[i + j] += temp / 10;
    }
  }

  while (product.data.size() > 1 && product.data[0] == 0)
  {
    product.data.erase(product.data.begin());
  }

  return product;
}

std::vector<int> long_int::digits()
{
  return data;
}

