// Copyright(c) 2019 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <cstdio>

#include "blenlib/BLI_math_geom.h"
#include "blenlib/BLI_math_vector.h"
#include "draw/GPU_vertex_format.h"

#include "geo_quadsphere1.h"
#include "ULPCompare.h"

int TestInt(int a, int b)
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

int TestFloat(float a, float b)
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

int TestNormalTri()
{
  printf("TestNormalTri\n");
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  int failures = 0;
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    float normal_baseline[3];
    float length_baseline;
    length_baseline = normal_tri_v3(
        normal_baseline, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

    float normal_test[3];
    float length_test;
    length_test = normal_tri_v3_internalsse(
        normal_test, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

    printf("Testing tri %d\n", i);
    failures += TestFloat(length_baseline, length_test);
    failures += TestFloat(normal_baseline[0], normal_test[0]);
    failures += TestFloat(normal_baseline[1], normal_test[1]);
    failures += TestFloat(normal_baseline[2], normal_test[2]);
  }

  return failures;
}

int TestIsQuadFlip()
{
  printf("TestIsQuadFlip\n");
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  int failures = 0;
  for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
    int r_baseline = is_quad_flip_v3_first_third_fast(
        verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]], verts[tris[i + 1][2]]);

    int r_test = is_quad_flip_v3_first_third_fast_internalsse(
        verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]], verts[tris[i + 1][2]]);

    printf("Testing quad flip %d\n", i);
    failures += TestInt(r_baseline, r_test);
  }

  return failures;
}

int TestQuantize()
{
  printf("TestQuantize\n");
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  float normals[48][3];
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    xmmvecf n;
    normal_tri_v3_sse(&n,
                      load_xmmvecf_f3(verts[tris[i][0]]),
                      load_xmmvecf_f3(verts[tris[i][1]]),
                      load_xmmvecf_f3(verts[tris[i][2]]));

    store_f3_xmmvecf(normals[i], n);
  }

  int failures = 0;
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    GPUPackedNormal pn_baseline = GPU_normal_convert_i10_v3(normals[i]);

    GPUPackedNormal pn_test = GPU_normal_convert_i10_v3_sse(load_xmmvecf_f3(normals[i]));

    printf("Testing tri %d\n", i);
    failures += TestInt(pn_baseline.x, pn_test.x);
    failures += TestInt(pn_baseline.y, pn_test.y);
    failures += TestInt(pn_baseline.z, pn_test.z);
  }

  return failures;
}

int main()
{
  int failures = 0;

  failures += TestNormalTri();
  failures += TestIsQuadFlip();
  failures += TestQuantize();

  return failures;
}