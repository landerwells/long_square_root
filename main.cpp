#include "long_int.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

int main()
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

  long_int radicand{radicand_string};
  long_int fours = 1;

  if (radicand != 0)
  {
    while ((fours << 2) < radicand)
    {
      fours = fours << 2;
    }

    while (fours >= 1)
    {
      // Need to focus on this line, must get the last multiplication out
      long_int num = ((square_root << 4) + 1) * fours;

      if (num <= radicand)
      {
        radicand = radicand - num;
        square_root = (square_root << 1) + 1;
      }
      else
      {
        square_root = square_root << 1;
      }
      fours = fours >> 2;
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << square_root << std::endl;

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
