#include <iostream>
#include <vector>
#include <string>

using namespace std;

void trim(string& a);
string add(string a, string b);
int main(int argc, char** argv);
string subtract(string a, string b);
string multiply(string a, string b);
int compare(string a, string b);

int main(int argc, char** argv)
{
  string n, ans = "0";
  int decimal;
  cout << "Enter the number you wish to take the square root of:  ";
  getline(cin, n);
  cout << "Enter the number of decimal places you would like in your answer:  ";
  cin >> decimal;
  string zeros(decimal, '0');
  n += zeros + zeros;
  trim(n);
  std::vector<std::string> fours(n.size() * 2, "0");
  fours[0] = "0";

  int j = 0;
  if (compare(fours[0], n) != 0)
  {
    fours[0] = "1";
    while (compare(multiply(fours[j], "4"), n) < 0)
    {
      fours[j + 1] = multiply(fours[j], "4");
      j++;
    }

    while (j >= 0)
    {
      string num = multiply(fours[j], add("1", multiply("4", ans)));
      if (compare(num, n) < 1)
        n = subtract(n, num), ans = add(multiply(ans, "2"), "1");
      else
        ans = multiply(ans, "2");
      j--;
    }
  }
  if (compare(ans, "0") == 0)
    cout << n << "." << zeros << endl;
  else
    cout << ans.substr(0, ans.length() - decimal) << "." << ans.substr(ans.length() - decimal)
         << endl;
  return 0;
}

string add(string a, string b)
{
  trim(a);
  trim(b);
  int i = a.length() - 1, j = b.length() - 1;
  string ans = "";
  int carry = 0;
  while (i >= 0 || j >= 0 || carry > 0)
  {
    int sum = 0;
    if (i >= 0)
      sum += (a[i] - '0');
    i--;
    if (j >= 0)
      sum += (b[j] - '0');
    j--;
    sum += carry;
    carry = sum / 10;
    ans = (char)(sum % 10 + '0') + ans;
  }
  return ans;
}

string subtract(string a, string b)
{
  trim(a);
  trim(b);
  while (a.length() < b.length())
    a = "0" + a;
  while (b.length() < a.length())
    b = "0" + b;
  for (int i = 0; i < b.length(); i++)
    b[i] = (char)('9' - b[i] + '0');
  string diff = add(add(a, b), "1").substr(1);
  trim(diff);
  return diff;
}

void trim(string& a)
{
  while (a.length() > 1 && a[0] == '0')
    a = a.substr(1);
}

string multiply(string a, string b)
{
  trim(a);
  trim(b);
  if (a == "0" || b == "0")
    return "0";
  string prod(a.length() + b.length(), '0');
  for (int i = a.length() - 1; i >= 0; i--)
  {
    for (int j = b.length() - 1; j >= 0; j--)
    {
      int temp = (prod[i + j + 1] - '0') + (a[i] - '0') * (b[j] - '0');
      prod[i + j + 1] = (char)(temp % 10 + '0');
      prod[i + j] += temp / 10;
    }
  }
  trim(prod);
  return prod;
}

int compare(string a, string b)
{
  if (a.length() > b.length())
    return 1;
  if (b.length() > a.length())
    return -1;
  if (a < b)
    return -1;
  if (a > b)
    return 1;
  return 0;
}
