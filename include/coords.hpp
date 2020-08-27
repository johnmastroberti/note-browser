#pragma once
#include <initializer_list>

// Basic xy coordinate struct
struct Coords {
  int x, y;

  Coords() = default;
  Coords(const std::initializer_list<int>& l);

  Coords& operator+=(const Coords& o);
  Coords& operator-=(const Coords& o);
};

Coords operator+(const Coords& c1, const Coords& c2);
Coords operator-(const Coords& c1, const Coords& c2);
