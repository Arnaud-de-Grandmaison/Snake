#pragma once

#include <ostream>

namespace Snake {

enum Axis { X = 0, Y, Z };
enum Direction { Minus = -1, Plus = 1 };

// The Coords class represents 3D coordinates of the Snake's cubes.
template <class Ty> struct CoordsTmpl {
  typedef Ty value_type;

  Ty x;
  Ty y;
  Ty z;

  CoordsTmpl() : x(Ty()), y(Ty()), z(Ty()) {}
  CoordsTmpl(const CoordsTmpl &) = default;
  CoordsTmpl(Ty x, Ty y, Ty z) : x(x), y(y), z(z) {}

  CoordsTmpl &operator=(const CoordsTmpl &) = default;

  bool operator==(const CoordsTmpl &RHS) const {
    return x == RHS.x && y == RHS.y && z == RHS.z;
  }

  bool operator!=(const CoordsTmpl &RHS) const { return !(*this == RHS); }

  CoordsTmpl &operator+=(const CoordsTmpl &RHS) {
    x += RHS.x, y += RHS.y, z += RHS.z;
    return *this;
  }

  CoordsTmpl &operator-=(const CoordsTmpl &RHS) {
    x -= RHS.x, y -= RHS.y, z -= RHS.z;
    return *this;
  }

  CoordsTmpl &inc(Axis a) {
    switch (a) {
    case X: x += 1; break;
    case Y: y += 1; break;
    case Z: z += 1; break;
    }
    return *this;
  }
  CoordsTmpl &dec(Axis a) {
    switch (a) {
    case X: x -= 1; break;
    case Y: y -= 1; break;
    case Z: z -= 1; break;
    }
    return *this;
  }
};

template <class Ty>
CoordsTmpl<Ty> min(CoordsTmpl<Ty> &LHS, const CoordsTmpl<Ty> &RHS) {
  return CoordsTmpl<Ty>(std::min(LHS.x, RHS.x), std::min(LHS.y, RHS.y), std::min(LHS.z, RHS.z));
}

template <class Ty>
CoordsTmpl<Ty> max(CoordsTmpl<Ty> &LHS, const CoordsTmpl<Ty> &RHS) {
  return CoordsTmpl<Ty>(std::max(LHS.x, RHS.x), std::max(LHS.y, RHS.y), std::max(LHS.z, RHS.z));
}

template <class Ty>
CoordsTmpl<Ty> abs(const CoordsTmpl<Ty> &C) {
  return CoordsTmpl<Ty>(std::abs(C.x), std::abs(C.y), std::abs(C.z));
}

template <class Ty>
Ty max(const CoordsTmpl<Ty> &C) {
  return std::max(std::max(C.x, C.y), C.z);
}

template <class Ty>
CoordsTmpl<Ty> operator+(const CoordsTmpl<Ty> &LHS, const CoordsTmpl<Ty> &RHS) {
  CoordsTmpl<Ty> Result(LHS);
  Result += RHS;
  return Result;
}

template <class Ty>
CoordsTmpl<Ty> operator-(const CoordsTmpl<Ty> &LHS, const CoordsTmpl<Ty> &RHS) {
  CoordsTmpl<Ty> Result(LHS);
  Result -= RHS;
  return Result;
}

template <class Ty> CoordsTmpl<Ty> inc(const CoordsTmpl<Ty> &C, Axis a) {
  CoordsTmpl<Ty> Result(C);
  Result.inc(a);
  return Result;
}

template <class Ty> CoordsTmpl<Ty> dec(const CoordsTmpl<Ty> &C, Axis a) {
  CoordsTmpl<Ty> Result(C);
  Result.dec(a);
  return Result;
}

template <class Ty>
std::ostream &operator<<(std::ostream &os, const CoordsTmpl<Ty> &C) {
  os << '(' << C.x << ',' << C.y << ',' << C.z << ')';
  return os;
}

std::ostream &operator<<(std::ostream &os, Axis a) {
  const char AxisStr[3]{'X', 'Y', 'Z'};
  os << AxisStr[a];
  return os;
}

std::ostream &operator<<(std::ostream &os, Direction d) {
  switch (d) {
    case Direction::Plus: os << '+'; break;
    case Direction::Minus: os << '-'; break;
  }
  return os;
}

using Coords = CoordsTmpl<int>;

} // namespace Snake

