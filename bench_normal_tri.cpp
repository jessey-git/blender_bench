// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "blenlib/BLI_math_vector.h"
#include "blenlib/BLI_math_geom.h"

#include "geo_quadsphere1.h"

//
// Baseline: normal_tri_v3
//
static void BB_normal_tri_v3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[3];

      normal_tri_v3(n, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(n);
    }
  }
}

//
// Variant: normal_tri_v3: pass-through to normal_tri_m128
//
static void BB_normal_tri_v3_pass(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[3];

      normal_tri_v3_pass(n, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(n);
    }
  }
}

//
// Variant: normal_tri_v3: re-implemented as normal_tri_m128
//
static void BB_normal_tri_v3_sse(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[3];

      normal_tri_v3_sse(n, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Variant: normal_tri_v3 load from f3, store to f3
//
static void BB_normal_tri_m128_lf3sf3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[3];

      __m128 no;
      normal_tri_m128(&no,
                      load_m128_f3(verts[tris[i][0]]),
                      load_m128_f3(verts[tris[i][1]]),
                      load_m128_f3(verts[tris[i][2]]));

      store_f3_m128(n, no);
      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Variant: normal_tri_v3 load from f3, store to f4
//
static void BB_normal_tri_m128_lf3sf4(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[4];

      __m128 temp;
      normal_tri_m128(&temp,
                      load_m128_f3(verts[tris[i][0]]),
                      load_m128_f3(verts[tris[i][1]]),
                      load_m128_f3(verts[tris[i][2]]));

      store_f4_m128(n, temp);
      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Variant: normal_tri_v3 load from f4, store to f4
//
static void BB_normal_tri_m128_lf4sf4(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[4];

      __m128 temp;
      normal_tri_m128(&temp,
                      load_m128_f4(verts[tris[i][0]]),
                      load_m128_f4(verts[tris[i][1]]),
                      load_m128_f4(verts[tris[i][2]]));

      store_f4_m128(n, temp);
      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Speed-of-light: normal_tri_v3 native: load from native, store to native
//
static void BB_normal_tri_m128_native(benchmark::State &state)
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
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      __m128 n;

      normal_tri_m128(&n, xmmverts[tris[i][0]], xmmverts[tris[i][1]], xmmverts[tris[i][2]]);

      benchmark::DoNotOptimize(n);
    }
  }
}

BENCHMARK(BB_normal_tri_v3);
BENCHMARK(BB_normal_tri_v3_pass);
BENCHMARK(BB_normal_tri_v3_sse);
BENCHMARK(BB_normal_tri_m128_lf3sf3);
BENCHMARK(BB_normal_tri_m128_lf3sf4);
BENCHMARK(BB_normal_tri_m128_lf4sf4);
BENCHMARK(BB_normal_tri_m128_native);
