// Copyright(c) 2019 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstdint>
#include <cmath>
#include <cfloat>

union Float_t {
  Float_t(float num = 0.0f) : f(num)
  {
  }

  bool Negative() const
  {
    return i < 0;
  }

  int32_t i;
  float f;
};

bool AlmostEqualUlpsAndAbs(float A, float B)
{
  // Check if the numbers are really close
  // Needed when comparing numbers near zero.
  const float absDiff = std::fabs(A - B);
  if (absDiff <= 1e-5f) {
    return true;
  }

  // Check if the numbers are "relatively" close
  // Needed since accuracy decreases for larger floats...
  const float absA = std::fabs(A);
  const float absB = std::fabs(B);
  const float largest = (absB > absA) ? absB : absA;
  const float maxDiff = largest * FLT_EPSILON;
  if (absDiff <= maxDiff) {
    return true;
  }

  Float_t uA(A);
  Float_t uB(B);

  // Different signs means they do not match.
  if (uA.Negative() != uB.Negative()) {
    return false;
  }

  // Find the difference in ULPs.
  const int ulpsDiff = abs(uA.i - uB.i);
  if (ulpsDiff <= 2) {
    return true;
  }

  return false;
}
