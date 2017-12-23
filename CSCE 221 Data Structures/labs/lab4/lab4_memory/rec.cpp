#include <iostream>

bool all_true(bool* _b, int _i, int _n){
  if (_i < _n)
    return _b[_i] && all_true(_b, _i+1, _n);
  else
    return true;
}

int main()
{
  const int n = 10000;

  bool* a = new bool[n];

  for (int i = 0; i < n; ++i)
    a[i] = true;

  bool x = all_true(a, 0, n);

  std::cout << std::boolalpha << x << std::endl;

  return 0;
}
