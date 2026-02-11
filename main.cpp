#include "long_int.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

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

  auto start_time = std::chrono::high_resolution_clock::now();

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

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  for (const auto& n : square_root.digits()) std::cout << n;

  std::cout << '\n';

  double seconds = duration.count() / 1000000.0;
  std::cout << "\nPerformance: ";
  if (seconds < 0.001) {
    std::cout << std::fixed << std::setprecision(2) << (duration.count() / 1000.0) << " microseconds";
  } else if (seconds < 1.0) {
    std::cout << std::fixed << std::setprecision(2) << (duration.count() / 1000.0) << " milliseconds";
  } else if (seconds < 60.0) {
    std::cout << std::fixed << std::setprecision(3) << seconds << " seconds";
  } else {
    int minutes = static_cast<int>(seconds / 60);
    double remaining_seconds = seconds - (minutes * 60);
    std::cout << minutes << " minute(s) and " << std::fixed << std::setprecision(2) << remaining_seconds << " seconds";
  }
  std::cout << '\n';
}
