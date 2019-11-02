// Copyright(c) 2019 Jesse Yurkovich
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
// Variant: is_quad_flip_v3_internalsse: implemented in terms of is_quad_flip_v3_sse
//
static void BB_is_quad_flip_v3_internalsse(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_internalsse(
          verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]], verts[tris[i + 1][2]]);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Variant: is_quad_flip_v3 sse: load from f3
//
static void BB_is_quad_flip_v3_sse_lf3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_sse(load_xmmvecf_f3(verts[tris[i][0]]),
                                                   load_xmmvecf_f3(verts[tris[i][1]]),
                                                   load_xmmvecf_f3(verts[tris[i][2]]),
                                                   load_xmmvecf_f3(verts[tris[i + 1][2]]));

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Variant: is_quad_flip_v3 sse: load from f4
//
static void BB_is_quad_flip_v3_sse_lf4(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_sse(load_xmmvecf_f4(verts[tris[i][0]]),
                                                   load_xmmvecf_f4(verts[tris[i][1]]),
                                                   load_xmmvecf_f4(verts[tris[i][2]]),
                                                   load_xmmvecf_f4(verts[tris[i + 1][2]]));

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE Speed-of-light: is_quad_flip_v3 sse: load from xmm
//
static void BB_is_quad_flip_v3_sse_lxmm(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  // Setup: simulate xmmvecfs already being available (not in hot path)...
  xmmvecf xmmverts[26];
  for (int i = 0; i < geo_quadsphehere1_numverts; i++) {
    xmmverts[i] = load_xmmvecf_f4(verts[i]);
  }

  // Benchmark
  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i += 2) {
      int r = is_quad_flip_v3_first_third_fast_sse(xmmverts[tris[i][0]],
                                                   xmmverts[tris[i][1]],
                                                   xmmverts[tris[i][2]],
                                                   xmmverts[tris[i + 1][2]]);

      benchmark::DoNotOptimize(r);
    }
  }
}

BENCHMARK(BB_is_quad_flip_v3);
BENCHMARK(BB_is_quad_flip_v3_internalsse);
BENCHMARK(BB_is_quad_flip_v3_sse_lf3);
BENCHMARK(BB_is_quad_flip_v3_sse_lf4);
BENCHMARK(BB_is_quad_flip_v3_sse_lxmm);
