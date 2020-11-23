// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "blenlib/BLI_math_geom.h"

#include "geo_quadsphere1.h"

//
// Baseline: closest_on_tri_to_point_v3
//
static void BB_closest_on_tri_to_point_v3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  float p1[3] = {0, 0, 0};
  float p2[3] = {1, 1, 1};
  float p3[3] = {0.1f, 0.2f, 0.3f};

  // Test 3 different points against all tris of a quadsphere
  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float r1[3], r2[3], r3[3];

      closest_on_tri_to_point_v3(r1, p1, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);
      closest_on_tri_to_point_v3(r2, p2, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);
      closest_on_tri_to_point_v3(r3, p3, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(r1);
      benchmark::DoNotOptimize(r2);
      benchmark::DoNotOptimize(r3);
    }
  }
}

//
// Hybrid: closest_on_tri_to_point_v3: pass-through to closest_on_tri_to_point_m128
//
static void BB_closest_on_tri_to_point_v3_pass(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  float p1[3] = {0, 0, 0};
  float p2[3] = {1, 1, 1};
  float p3[3] = {0.1f, 0.2f, 0.3f};

  // Test 3 different points against all tris of a quadsphere
  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float r1[3], r2[3], r3[3];

      closest_on_tri_to_point_v3_pass(
          r1, p1, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);
      closest_on_tri_to_point_v3_pass(
          r2, p2, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);
      closest_on_tri_to_point_v3_pass(
          r3, p3, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(r1);
      benchmark::DoNotOptimize(r2);
      benchmark::DoNotOptimize(r3);
    }
  }
}

//
// SSE: closest_on_tri_to_point_m128 native
//
static void BB_closest_on_tri_to_point_m128_native(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  float p1f[3] = {0, 0, 0};
  float p2f[3] = {1, 1, 1};
  float p3f[3] = {0.1f, 0.2f, 0.3f};

  // Setup: simulate __m128s already being available (not in hot path)...
  __m128 p1 = load_m128_f3(p1f);
  __m128 p2 = load_m128_f3(p2f);
  __m128 p3 = load_m128_f3(p3f);
  __m128 xmmverts[26];
  for (int i = 0; i < geo_quadsphehere1_numverts; i++) {
    xmmverts[i] = load_m128_f4(verts[i]);
  }

  // Test 3 different points against all tris of a quadsphere
  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      __m128 r1, r2, r3;

      closest_on_tri_to_point_m128(
          &r1, p1, xmmverts[tris[i][0]], xmmverts[tris[i][1]], xmmverts[tris[i][2]]);
      closest_on_tri_to_point_m128(
          &r2, p2, xmmverts[tris[i][0]], xmmverts[tris[i][1]], xmmverts[tris[i][2]]);
      closest_on_tri_to_point_m128(
          &r3, p3, xmmverts[tris[i][0]], xmmverts[tris[i][1]], xmmverts[tris[i][2]]);

      benchmark::DoNotOptimize(r1);
      benchmark::DoNotOptimize(r2);
      benchmark::DoNotOptimize(r3);
    }
  }
}

BENCHMARK(BB_closest_on_tri_to_point_v3);
BENCHMARK(BB_closest_on_tri_to_point_v3_pass);
BENCHMARK(BB_closest_on_tri_to_point_m128_native);
