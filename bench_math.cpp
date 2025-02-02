// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "blenlib/BLI_math_vector.h"

#include "geo_quadsphere1.h"

//
// Baseline: dot_v3v3
//
static void BB_dot_v3v3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float d;

      d = dot_v3v3(verts[tris[i][0]], verts[tris[i][1]]);
      d += dot_v3v3(verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(d);
    }
  }
}

//
// Variant: dot_v3v3: implemented in terms of dot_m128m128
//
static void BB_dot_v3v3_pass(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float d;

      d = dot_v3v3_pass(verts[tris[i][0]], verts[tris[i][1]]);
      d += dot_v3v3_pass(verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(d);
    }
  }
}

//
// SSE Variant: dot_m128m128 load from f3
//
static void BB_dot_m128m128_lf3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float d;

      d = dot_m128m128(load_m128_f3(verts[tris[i][0]]), load_m128_f3(verts[tris[i][1]]));
      d += dot_m128m128(load_m128_f3(verts[tris[i][1]]), load_m128_f3(verts[tris[i][2]]));

      benchmark::DoNotOptimize(d);
    }
  }
}

//
// SSE Variant: dot_m128m128 load from f4
//
static void BB_dot_m128m128_lf4(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float d;

      d = dot_m128m128(load_m128_f4(verts[tris[i][0]]), load_m128_f4(verts[tris[i][1]]));
      d += dot_m128m128(load_m128_f4(verts[tris[i][1]]), load_m128_f4(verts[tris[i][2]]));

      benchmark::DoNotOptimize(d);
    }
  }
}

//
// SSE Speed-of-light: dot_v3v3 load from native
//
static void BB_dot_m128m128_native(benchmark::State &state)
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
      float d;

      d = dot_m128m128(xmmverts[tris[i][0]], xmmverts[tris[i][1]]);
      d += dot_m128m128(xmmverts[tris[i][1]], xmmverts[tris[i][2]]);

      benchmark::DoNotOptimize(d);
    }
  }
}

BENCHMARK(BB_dot_v3v3);
BENCHMARK(BB_dot_v3v3_pass);
BENCHMARK(BB_dot_m128m128_lf3);
BENCHMARK(BB_dot_m128m128_lf4);
BENCHMARK(BB_dot_m128m128_native);
