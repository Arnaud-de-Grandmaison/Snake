#include "Coords.h"

#include <array>
#include <cassert>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace std {
// TODO: come up with a better / more robust hash function!
template <> struct hash<Snake::Coords> {
  size_t operator()(const Snake::Coords &C) const noexcept {
    size_t H = C.x;
    H += C.y * 1024;
    H += C.z * 1024 * 1024;
    return H;
  }
};
} // namespace std

namespace Snake {

// The Tmpl class represents the physical Tmpl elements. This is mostly a
// template and as such is constant through the whole puzzle solving.
template <unsigned Size> class Tmpl : public std::vector<unsigned> {

  unsigned computeNumElts() const {
    unsigned acc = 1;
    for (const auto &e : *this)
      acc += e;
    return acc - getNumSegments();
  }

public:
  Tmpl() = delete;
  Tmpl(const Tmpl &) = default;
  Tmpl(const std::initializer_list<unsigned> &il) : std::vector<unsigned>(il) {
    assert(computeNumElts() == getNumElts() &&
           "Unexpected element number in template !");
  }
  Tmpl &operator=(const Tmpl &) = default;

  unsigned getNumSegments() const { return size(); }
  unsigned getNumElts() const { return Size * Size * Size; }

  void dump(std::ostream &os) const {
    os << "Tmpl<" << getNumSegments() << ", " << getNumElts() << "> = {";
    for (const auto &e : *this)
      os << e << ", ";
    os << "}";
  }
};

// This represents the problem we are trying to solve.
template <unsigned Size> class Problem {
  struct Context {
    std::unordered_set<Coords> Occupied;
    Coords minXYZ;
    Coords maxXYZ;
    size_t eltNum;
    Snake::Axis axis;
    Snake::Direction dir;

    Context() : Occupied(), minXYZ(), maxXYZ(), eltNum(0), axis(), dir() {}
    Context(std::unordered_set<Coords> &&occ, Coords min, Coords max,
            size_t elt)
        : Occupied(occ), minXYZ(min), maxXYZ(max), eltNum(elt) {}

#ifndef NDEBUG
    void dump(std::ostream &os) const {
      os << "eltNum:" << eltNum;
      os << " minXYZ:" << minXYZ;
      os << " maxXYZ:" << maxXYZ;
      os << " move:" << axis << dir;
    }
#endif
  };

  struct Solution {
    std::array<Coords, Size * Size * Size> Locations;
    std::vector<std::pair<Snake::Axis, Snake::Direction>> Moves;

    Solution(const std::array<Coords, Size * Size * Size> &Locations,
             std::vector<std::pair<Snake::Axis, Snake::Direction>> &&Moves)
        : Locations(Locations), Moves(Moves) {}

    void dump(std::ostream &os) const {
      os << " - Moves: ";
      for (const auto &M : Moves)
        os << M.first << M.second << ", ";
      os << '\n';

      os << "   Locations: ";
      for (const auto &C : Locations)
        os << C << ", ";
      os << '\n';
    }
  };

  size_t cntPath = 0; // Count of explored path, i.e. leaves when a solution is
                      // found or a backtrack is done.
  size_t cnt = 0;     // Count of solve calls.

  const Snake::Tmpl<Size> &Snake;

  // We could be using the solver call stack instead, but this would make it much
  // harder to iterate thru the different contexts for debugging purposes, so use
  // separate dedicated stacks.
  std::vector<Context> Contexts;
  std::array<Coords, Size * Size * Size> TheSnake;

  std::vector<Solution> Solutions; // All solutions found by the solver.

#ifndef NDEBUG
  void dumpSnake(std::ostream &os) const {
    os << "Snake: ";
    if (Contexts.size())
      for (size_t i = 0; i <= Contexts.back().eltNum; i++) {
        os << TheSnake[i] << ", ";
      }
    os << '\n';
  }

  void dumpContext(std::ostream &os, bool dumpAllContexts = false) const {
    os << "Context<" << Contexts.size() << ">:\n";
    if (Contexts.size() == 0) {
      os << "(no contexts)\n";
      return;
    }

    if (dumpAllContexts)
      for (const auto &C : Contexts) {
        C.dump(os);
        os << '\n';
      }
    else {
      Contexts.back().dump(os);
      os << '\n';
    }
  }
#endif

  void solve(unsigned segNum, Snake::Axis axis, Snake::Direction dir) {
    cnt++;
    Context &ourContext = Contexts[segNum];
    ourContext.axis = axis;
    ourContext.dir = dir;

#ifndef NDEBUG
    std::cout << "\nsolve<" << segNum << ", " << ourContext.axis << ourContext.dir << ">\n";
    dumpContext(std::cout);
    dumpSnake(std::cout);
#endif

    size_t newEltNum = ourContext.eltNum;
    Coords newMin = ourContext.minXYZ;
    Coords newMax = ourContext.maxXYZ;
    std::unordered_set<Coords> newOcc = ourContext.Occupied;

    // Layout the snake as specified by the arguments
    for (unsigned i = 1; i < Snake[segNum]; i++) {
      switch (ourContext.dir) {
      case Snake::Plus:
        TheSnake[newEltNum + i] = inc(TheSnake[newEltNum + i - 1], ourContext.axis);
        break;
      case Snake::Minus:
        TheSnake[newEltNum + i] = dec(TheSnake[newEltNum + i - 1], ourContext.axis);
        break;
      }

      // Check the Snake next layout is within bounds.
      newMin = min(newMin, TheSnake[newEltNum + i]);
      newMax = max(newMax, TheSnake[newEltNum + i]);
      unsigned maxAbs = max(abs(newMax - newMin));
#ifndef NDEBUG
      std::cout << "C:" << TheSnake[newEltNum + i] << " newMin:" << newMin
                << " newMax:" << newMax << " maxabs:" << maxAbs << '\n';
#endif
      if (Size <= maxAbs) {
#ifndef NDEBUG
        std::cout << "Invalid -- out of bounding box !\n";
#endif
        cntPath++;
        return;
      }

      // Check for collision.
      if (!newOcc.insert(TheSnake[newEltNum + i]).second) {
#ifndef NDEBUG
        std::cout << "Invalid -- location not free!\n";
#endif
        cntPath++;
        return;
      }
    }
    newEltNum += Snake[segNum] - 1;

    // If all Snake segments / elements have been laid out, we're done !
    if (segNum == Snake.getNumSegments() - 1) {
      cntPath++;
      std::vector<std::pair<Snake::Axis, Snake::Direction>> TheMoves;
      for (const auto &C: Contexts)
        TheMoves.push_back(std::make_pair(C.axis, C.dir));
      Solutions.push_back(Solution(TheSnake, std::move(TheMoves)));
      return;
    }

    // Push next step.
    Contexts.push_back(Context(std::move(newOcc), newMin, newMax, newEltNum));

    // Call solve on orthogonal axises, for each direction
    switch (ourContext.axis) {
    case Snake::X:
      solve(segNum + 1, Snake::Y, Snake::Plus);
      solve(segNum + 1, Snake::Y, Snake::Minus);
      solve(segNum + 1, Snake::Z, Snake::Plus);
      solve(segNum + 1, Snake::Z, Snake::Minus);
      break;
    case Snake::Y:
      solve(segNum + 1, Snake::X, Snake::Plus);
      solve(segNum + 1, Snake::X, Snake::Minus);
      solve(segNum + 1, Snake::Z, Snake::Plus);
      solve(segNum + 1, Snake::Z, Snake::Minus);
      break;
    case Snake::Z:
      solve(segNum + 1, Snake::X, Snake::Plus);
      solve(segNum + 1, Snake::X, Snake::Minus);
      solve(segNum + 1, Snake::Y, Snake::Plus);
      solve(segNum + 1, Snake::Y, Snake::Minus);
      break;
    }

    Contexts.pop_back();
    return;
  }

public:
  Problem() = delete;
  Problem(const Problem &) = default;
  Problem(const Snake::Tmpl<Size> &Snake) : Snake(Snake) {}

  void solve() {
    TheSnake[0] = Coords(); // First Snake element is at Origin
    Context Ctxt;
    Ctxt.Occupied.insert(TheSnake[0]);
    Contexts.push_back(Ctxt);
    solve(0, Axis::X, Direction::Plus);
  }

  bool solved() const { return !Solutions.empty(); }

  void dumpStats(std::ostream &os) const {
    os << "Stats:\n";
    os << " - " << cntPath << " explored paths\n";
    os << " - " << cnt << " calls to solve()\n";
  }

  void dumpSolutions(std::ostream &os) const {
    os << "Solutions found: " << Solutions.size() << '\n';
    for (const auto &S : Solutions) {
      S.dump(os);
    }
  }
};

} // namespace Snake

using namespace Snake;

int main(int argc, char *argv[]) {
  // const Tmpl<3> S({3, 3, 3, 3, 2, 2, 2, 3, 3, 2, 2, 3, 2, 3, 2, 2, 3});
  const Tmpl<3> S({3, 2, 2, 3, 2, 3, 2, 2, 3, 3, 2, 2, 2, 3, 3, 3, 3});
  std::cout << "Using ";
  S.dump(std::cout);
  std::cout << "\n";
  Problem<3> P(S);
  P.solve();
  P.dumpStats(std::cout);
  if (P.solved()) {
    std::cout << "Solved !\n";
    P.dumpSolutions(std::cout);
  } else
    std::cout << "No solution.\n";
  return EXIT_SUCCESS;
}
