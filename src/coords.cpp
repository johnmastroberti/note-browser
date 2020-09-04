#include "coords.hpp"
#include <cstdio>
using namespace std::string_literals;

Coords::Coords(const std::initializer_list<int>& l) {
  if (l.size() != 2) throw;
  auto it = l.begin();
  x = *it;
  ++it;
  y = *it;
}

Coords::operator std::string() const {
  char buff[200];
  sprintf(buff, "{%d, %d}", x, y);
  return std::string(buff);
}


Coords& Coords::operator+=(const Coords& o) {
  x += o.x;
  y += o.y;
  return *this;
}

Coords& Coords::operator-=(const Coords& o) {
  x -= o.x;
  y -= o.y;
  return *this;
}

Coords operator+(const Coords& c1, const Coords& c2) {
  Coords ans = c1;
  ans += c2;
  return ans;
}

Coords operator-(const Coords& c1, const Coords& c2) {
  Coords ans = c1;
  ans -= c2;
  return ans;
}
