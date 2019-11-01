// Copyright(c) 2019 Jesse Yurkovich
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
// Variant: normal_tri_v3_internalsse: implemented in terms of normal_tri_v3_sse
//
static void BB_normal_tri_v3_internalsse(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[3];

      normal_tri_v3_internalsse(n, verts[tris[i][0]], verts[tris[i][1]], verts[tris[i][2]]);

      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Variant: normal_tri_v3 sse: load from f3, store to f3
//
static void BB_normal_tri_v3_sse_lf3sf3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[3];

      xmmvecf no;
      normal_tri_v3_sse(&no,
                        load_xmmvecf_f3(verts[tris[i][0]]),
                        load_xmmvecf_f3(verts[tris[i][1]]),
                        load_xmmvecf_f3(verts[tris[i][2]]));

      store_f3_xmmvecf(n, no);
      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Variant: normal_tri_v3 sse: load from f3, store to f4
//
static void BB_normal_tri_v3_sse_lf3sf4(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[4];

      xmmvecf temp;
      normal_tri_v3_sse(&temp,
                        load_xmmvecf_f3(verts[tris[i][0]]),
                        load_xmmvecf_f3(verts[tris[i][1]]),
                        load_xmmvecf_f3(verts[tris[i][2]]));

      store_f4_xmmvecf(n, temp);
      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Variant: normal_tri_v3 sse: load from f4, store to f4
//
static void BB_normal_tri_v3_sse_lf4sf4(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      float n[4];

      xmmvecf temp;
      normal_tri_v3_sse(&temp,
                        load_xmmvecf_f4(verts[tris[i][0]]),
                        load_xmmvecf_f4(verts[tris[i][1]]),
                        load_xmmvecf_f4(verts[tris[i][2]]));

      store_f4_xmmvecf(n, temp);
      benchmark::DoNotOptimize(n);
    }
  }
}

//
// SSE Speed-of-light: normal_tri_v3 sse: load from xmm, store to xmm
//
static void BB_normal_tri_v3_sse_lxmmsxmm(benchmark::State &state)
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
    for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
      xmmvecf n;

      normal_tri_v3_sse(&n, xmmverts[tris[i][0]], xmmverts[tris[i][1]], xmmverts[tris[i][2]]);

      benchmark::DoNotOptimize(n);
    }
  }
}


BENCHMARK(BB_normal_tri_v3);
BENCHMARK(BB_normal_tri_v3_internalsse);
BENCHMARK(BB_normal_tri_v3_sse_lf3sf3);
BENCHMARK(BB_normal_tri_v3_sse_lf3sf4);
BENCHMARK(BB_normal_tri_v3_sse_lf4sf4);
BENCHMARK(BB_normal_tri_v3_sse_lxmmsxmm);
