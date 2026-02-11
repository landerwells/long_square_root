#include <string>

#include <compare>
#include <vector>

using namespace std;

class long_int
{
  public:
  long_int();
  long_int(int);
  long_int(const std::string& str);
  long_int(const long_int&);
  long_int(long_int&&) noexcept;

  ~long_int() = default;

  long_int& operator=(const long_int&);
  long_int& operator=(long_int&&) noexcept;

  std::vector<int> digits();

  long_int operator+(const long_int&) const;
  long_int operator-(const long_int&) const;
  long_int operator*(const long_int&) const;
  std::strong_ordering operator<=>(const long_int& other) const;
  bool operator==(const long_int& other) const = default;

  friend std::ostream& operator<<(std::ostream& os, const long_int& num);

  private:
  std::vector<int> data{};
};
