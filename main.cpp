#include "long_int.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
  std::string radicand_string;
  long_int square_root = 0;

  int decimal;

  std::cout << "Enter the number you wish to take the square root of:  ";
  getline(std::cin, radicand_string);
  std::cout << "Enter the number of decimal places you would like in your answer:  ";
  std::cin >> decimal;

  std::string zeros(decimal, '0');
  radicand_string += zeros + zeros;

  std::vector<long_int> fours(radicand_string.size() * 2, 0);
  long_int radicand{radicand_string};

  int j = 0;

  if (radicand != 0)
  {
    fours[0] = 1;

    while ((fours[j] * 4) < radicand)
    {
      fours[j + 1] = fours[j] * 4;
      j++;
    }

    while (j >= 0)
    {
      long_int num = fours[j] * ((square_root * 4) + 1);

      if (num <= radicand)
      {
        radicand = radicand - num;
        square_root = (square_root * 2) + 1;
      }
      else
      {
        square_root = square_root * 2;
      }
      j--;
    }
  }

  for (const auto& n : square_root.digits()) std::cout << n;

  std::cout << '\n';
}
