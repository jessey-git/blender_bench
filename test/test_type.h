// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <cstdio>

#include "ULPCompare.h"

static int TestInt(int a, int b)
{
  if (abs(a - b) > 1) {
    printf("  Fail: %d vs %d\n", a, b);
    return 1;
  }
  else {
    printf("  Pass: %d vs %d\n", a, b);
    return 0;
  }
}

static int TestFloat(float a, float b)
{
  if (!AlmostEqualUlpsAndAbs(a, b)) {
    printf("  Fail: %1.15f vs %1.15f\n", a, b);
    return 1;
  }
  else {
    printf("  Pass: %1.15f vs %1.15f\n", a, b);
    return 0;
  }
}
