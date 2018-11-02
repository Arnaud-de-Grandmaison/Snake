#include "Coords.h"
#include "gtest/gtest.h"

namespace {
using namespace Snake;
TEST(testCoords, Constructors) {
  Coords C0;
  EXPECT_EQ(C0, Coords(0, 0, 0));

  Coords C1{1, 2, 3};
  EXPECT_EQ(C1, Coords(1, 2, 3));

  Coords C2(C1);
  EXPECT_EQ(C2, Coords(1, 2, 3));
}

TEST(testCoords, Assignement) {
  Coords C{0, 0, 0};
  Coords C1{1, 2, 3};
  EXPECT_EQ(C, Coords(0, 0, 0));
  EXPECT_EQ(C1, Coords(1, 2, 3));

  C = C1;
  EXPECT_EQ(C, Coords(1, 2, 3));
  EXPECT_EQ(C1, Coords(1, 2, 3));

  C = {4, 5, 6};
  EXPECT_EQ(C, Coords(4, 5, 6));
}

TEST(testCoords, Compare) {
  Coords C{0, 0, 0};
  Coords C1{1, 2, 3};
  Coords C2{1, 2, 3};

  EXPECT_FALSE( C == C1 );
  EXPECT_TRUE( C != C1 );

  EXPECT_TRUE( C2 == C1 );
  EXPECT_FALSE( C2 != C1 );
}

TEST(testCoords, ConstAccessors) {
  Coords C{1, 2, 3};

  EXPECT_EQ(C.x, 1);
  EXPECT_EQ(C.y, 2);
  EXPECT_EQ(C.z, 3);
}

TEST(testCoords, Accessors) {
  Coords C{1, 2, 3};

  EXPECT_EQ(C, Coords(1, 2, 3));

  C.x = 6;
  C.y = 5;
  C.z = 4;
  EXPECT_EQ(C, Coords(6, 5, 4));
}

TEST(testCoords, IncDec) {
  Coords C{0, 0, 0};
  EXPECT_EQ(C, Coords(0, 0, 0));

  C.inc(X);
  EXPECT_EQ(C, Coords(1, 0, 0));
  C.inc(Y);
  EXPECT_EQ(C, Coords(1, 1, 0));
  C.inc(Z);
  EXPECT_EQ(C, Coords(1, 1, 1));

  C.dec(X);
  EXPECT_EQ(C, Coords(0, 1, 1));
  C.dec(Y);
  EXPECT_EQ(C, Coords(0, 0, 1));
  C.dec(Z);
  EXPECT_EQ(C, Coords(0, 0, 0));
}
} // namespace
