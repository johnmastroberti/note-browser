#pragma once

// Basic xy coordinate struct
struct Coords {
  int x, y;

  Coords() = default;

  Coords& operator+=(const Coords& o);
  Coords& operator-=(const Coords& o);
};

Coords operator+(const Coords& c1, const Coords& c2);
Coords operator-(const Coords& c1, const Coords& c2);
