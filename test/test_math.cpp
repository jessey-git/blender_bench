// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <cstdio>

#include "blenlib/BLI_math_geom.h"
#include "blenlib/BLI_math_matrix.h"
#include "blenlib/BLI_math_vector.h"
#include "draw/GPU_vertex_format.h"

#include "geo_quadsphere1.h"

#include "test_type.h"

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
    length_test = normal_tri_v3_pass(
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

    int r_test = is_quad_flip_v3_first_third_fast_pass(
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
    __m128 n;
    normal_tri_m128(&n,
                    load_m128_f3(verts[tris[i][0]]),
                    load_m128_f3(verts[tris[i][1]]),
                    load_m128_f3(verts[tris[i][2]]));

    store_f3_m128(normals[i], n);
  }

  int failures = 0;
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    GPUPackedNormal pn_baseline = GPU_normal_convert_i10_v3(normals[i]);

    GPUPackedNormal pn_test = GPU_normal_convert_i10_m128(load_m128_f3(normals[i]));

    printf("Testing tri %d\n", i);
    failures += TestInt(pn_baseline.x, pn_test.x);
    failures += TestInt(pn_baseline.y, pn_test.y);
    failures += TestInt(pn_baseline.z, pn_test.z);
  }

  return failures;
}

int TestMatrixAdd()
{
  printf("TestMatrixAdd\n");

  float r_baseline[3][3];
  float r_test[3][3];
  float m1[3][3] = {{0.1f, 0.2f, 0.3f}, {0.4f, 0.5f, 0.6f}, {0.7f, 0.8f, 0.9f}};
  float m2[3][3] = {{0.1f, 0.2f, 0.3f}, {0.1f, 0.2f, 0.3f}, {0.1f, 0.2f, 0.3f}};

  add_m3_m3m3(r_baseline, m1, m2);
  add_m3_m3m3_sse(r_test, m1, m2);

  int failures = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      failures += TestFloat(r_baseline[i][j], r_test[i][j]);
    }
  }

  return failures;
}

int TestMatrixSub()
{
  printf("TestMatrixSub\n");

  float r_baseline[3][3];
  float r_test[3][3];
  float m1[3][3] = {{0.1f, 0.2f, 0.3f}, {0.4f, 0.5f, 0.6f}, {0.7f, 0.8f, 0.9f}};
  float m2[3][3] = {{0.1f, 0.2f, 0.3f}, {0.1f, 0.2f, 0.3f}, {0.1f, 0.2f, 0.3f}};

  sub_m3_m3m3(r_baseline, m1, m2);
  sub_m3_m3m3_sse(r_test, m1, m2);

  int failures = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      failures += TestFloat(r_baseline[i][j], r_test[i][j]);
    }
  }

  return failures;
}

int TestMatrixMul()
{
  printf("TestMatrixMul\n");

  float r_baseline[3];
  float r_test[3];
  float m1[3][3] = {{0.1f, 0.2f, 0.3f}, {0.4f, 0.5f, 0.6f}, {0.7f, 0.8f, 0.9f}};
  float v1[3] = {0.1f, 0.2f, 0.3f};

  mul_v3_m3v3(r_baseline, m1, v1);
  mul_v3_m3v3_sse(r_test, m1, v1);

  int failures = 0;
  failures += TestFloat(r_baseline[0], r_test[0]);
  failures += TestFloat(r_baseline[1], r_test[1]);
  failures += TestFloat(r_baseline[2], r_test[2]);

  return failures;
}

int main()
{
  int failures = 0;

  failures += TestNormalTri();
  failures += TestIsQuadFlip();
  failures += TestQuantize();
  failures += TestMatrixAdd();
  failures += TestMatrixSub();
  failures += TestMatrixMul();

  return failures;
}