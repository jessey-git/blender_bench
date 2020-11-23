// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "blenlib/BLI_math_vector.h"
#include "blenlib/BLI_math_geom.h"

#include "geo_quadsphere1.h"

//
// Baseline: is_quad_flip_v3_first_third_fast
//
static void BB_is_quad_flip_v3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast(
          verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]], verts[tris[i + 1][2]]);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Variant: is_quad_flip_v3_sse: pass-through to is_quad_flip_v3_first_third_fast_m128
//
static void BB_is_quad_flip_v3_sse(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_pass(
          verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]], verts[tris[i + 1][2]]);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Variant: is_quad_flip_v3 load from f3
//
static void BB_is_quad_flip_m128_lf3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_m128(load_m128_f3(verts[tris[i][0]]),
                                                    load_m128_f3(verts[tris[i][1]]),
                                                    load_m128_f3(verts[tris[i][2]]),
                                                    load_m128_f3(verts[tris[i + 1][2]]));

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Variant: is_quad_flip_v3 load from f4
//
static void BB_is_quad_flip_m128_lf4(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_m128(load_m128_f4(verts[tris[i][0]]),
                                                    load_m128_f4(verts[tris[i][1]]),
                                                    load_m128_f4(verts[tris[i][2]]),
                                                    load_m128_f4(verts[tris[i + 1][2]]));

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE Speed-of-light: is_quad_flip_v3 load from xmm
//
static void BB_is_quad_flip_m128_native(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  // Setup: simulate __m128s already being available (not in hot path)...
  __m128 xmmverts[26];
  for (int i = 0; i < geo_quadsphehere1_numverts; i++) {
    xmmverts[i] = load_m128_f4(verts[i]);
  }

  // Benchmark
  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_m128(xmmverts[tris[i][0]],
                                                    xmmverts[tris[i][1]],
                                                    xmmverts[tris[i][2]],
                                                    xmmverts[tris[i + 1][2]]);

      benchmark::DoNotOptimize(r);
    }
  }
}

BENCHMARK(BB_is_quad_flip_v3);
BENCHMARK(BB_is_quad_flip_v3_sse);
BENCHMARK(BB_is_quad_flip_m128_lf3);
BENCHMARK(BB_is_quad_flip_m128_lf4);
BENCHMARK(BB_is_quad_flip_m128_native);
